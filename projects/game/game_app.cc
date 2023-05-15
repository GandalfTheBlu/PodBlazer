#include "game_app.h"
#include "world_settings.h"
#include <chrono>
#include "file_manager.h"
#include "map_generator.h"
#include "resources.h"
#include "materials.h"

namespace Game
{
	App::App() :
		shouldClose(false),
		deltaTime(1.f/60.f),
		skybox(nullptr),
		player(nullptr),
		currentState(nullptr)
	{}

	App::~App(){}

	// helper methods
	bool LoadShader(const std::string& path, const std::string& name)
	{
		std::shared_ptr<Engine::Shader> shader = std::make_shared<Engine::Shader>();
		if (!shader->Init(path))
			return false;

		Engine::Resources::Instance().StoreShader(name, shader);
		return true;
	}

	bool LoadObjMeshNoMaterial(const std::string& path, const std::string& name)
	{
		std::shared_ptr<Engine::Mesh> mesh = std::make_shared<Engine::Mesh>();
		std::vector<Engine::ObjMaterialInfo> unused;
		if (!Engine::LoadOBJFile(*mesh, unused, path))
			return false;

		Engine::Resources::Instance().StoreMesh(name, mesh);

		return true;
	}

	bool LoadObjMeshAndMaterials(const std::string& path, const std::string& name, const std::string& shaderName)
	{
		std::shared_ptr<Engine::Mesh> mesh = std::make_shared<Engine::Mesh>();
		std::vector<Engine::ObjMaterialInfo> materialInfos;
		if (!Engine::LoadOBJFile(*mesh, materialInfos, path))
			return false;

		Engine::Resources::Instance().StoreMesh(name, mesh);

		int i = 0;
		for (auto& matInfo : materialInfos)
		{
			std::shared_ptr<FlatMaterial> material = std::make_shared<FlatMaterial>();
			material->shader = Engine::Resources::Instance().GetShader(shaderName);
			material->color = matInfo.color;
			Engine::Resources::Instance().StoreMaterial(name + std::to_string(i), material);
			i++;
		}

		return true;
	}

	bool LoadObjMeshAndMaterialsTextured(const std::string& path, const std::string& name, const std::string& shaderName, const std::string& textureName)
	{
		std::shared_ptr<Engine::Mesh> mesh = std::make_shared<Engine::Mesh>();
		std::vector<Engine::ObjMaterialInfo> materialInfos;
		if (!Engine::LoadOBJFile(*mesh, materialInfos, path))
			return false;

		Engine::Resources::Instance().StoreMesh(name, mesh);

		int i = 0;
		for (auto& matInfo : materialInfos)
		{
			std::shared_ptr<TextureMaterial> material = std::make_shared<TextureMaterial>();
			material->shader = Engine::Resources::Instance().GetShader(shaderName);
			material->texture = Engine::Resources::Instance().GetTexture(textureName);
			Engine::Resources::Instance().StoreMaterial(name + std::to_string(i), material);
			i++;
		}

		return true;
	}

	Prefab* CreatePrefab(const std::string& name)
	{
		Engine::Mesh* mesh = Engine::Resources::Instance().GetMesh(name);
		std::vector<Engine::Material*> materials;

		for (size_t i = 0; i < mesh->primitiveGroups.size(); i++)
		{
			materials.push_back(Engine::Resources::Instance().GetMaterial(name + std::to_string(i)));
		}

		return new Prefab(mesh, materials);
	}

	bool App::Open()
	{
		if (!window.Init(1400, 1000, "Pod Blazer"))
			return false;

		window.KeyCallback = [this](const Engine::KeyEvent& e)
		{
			if (e.action == GLFW_PRESS)
				this->keys[e.key] = { true, true, false };
			else if (e.action == GLFW_RELEASE)
				this->keys[e.key] = { false, false, true };
		};

		if (!renderer.Init(window.Width() / 4, window.Height() / 4))
			return false;

		// load all shaders
		if (!LoadShader("assets/shaders/phong", "phong") ||
			!LoadShader("assets/shaders/phong_tex", "phong_tex") ||
			!LoadShader("assets/shaders/skybox", "skybox") ||
			!LoadShader("assets/shaders/font", "font") ||
			!LoadShader("assets/shaders/particle", "flames"))
			return false;

		Engine::Resources& RS = Engine::Resources::Instance();

		// load all textures
		std::shared_ptr<Engine::Texture> roadTexture = std::make_shared<Engine::Texture>();
		roadTexture->Init("assets/textures/test.png");
		RS.StoreTexture("road", roadTexture);

		std::shared_ptr<Engine::Texture> fontTexture = std::make_shared<Engine::Texture>();
		fontTexture->Init("assets/textures/font64.png");
		RS.StoreTexture("font", fontTexture);

		std::shared_ptr<Engine::Texture> lindahliumTexture = std::make_shared<Engine::Texture>();
		lindahliumTexture->Init("assets/textures/lindahlium.jpg");
		RS.StoreTexture("lindahlium", lindahliumTexture);

		std::string objPath = "assets/kenney_space-kit/Models/OBJ format/";

		// load all meshes and materials from obj files
		if (!LoadObjMeshAndMaterials(objPath + "rock_crystals.obj", "rock1", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "rock_crystalsLargeA.obj", "rock2", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "rock_largeA.obj", "rock3", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "satelliteDish.obj", "parabola", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "terrain.obj", "ground", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "craft_speederC.obj", "ship", "phong") ||
			!LoadObjMeshAndMaterialsTextured("assets/custom_meshes/lindahlium.obj", "lindahlium", "phong_tex", "lindahlium") ||
			!LoadObjMeshNoMaterial("assets/custom_meshes/unit_cube.obj", "flames"))
			return false;

		// store custom materials
		std::shared_ptr<TextureMaterial> roadMaterial = std::make_shared<TextureMaterial>();
		roadMaterial->shader = RS.GetShader("phong_tex");
		roadMaterial->texture = RS.GetTexture("road");
		RS.StoreMaterial("road0", roadMaterial);

		std::shared_ptr<SkyboxMaterial> skyboxMaterial = std::make_shared<SkyboxMaterial>();
		skyboxMaterial->shader = RS.GetShader("skybox");
		RS.StoreMaterial("skybox0", skyboxMaterial);

		std::shared_ptr<ParticleMaterial> particleMaterial = std::make_shared<ParticleMaterial>();
		particleMaterial->shader = RS.GetShader("flames");
		RS.StoreMaterial("flames0", particleMaterial);
		

		// load road path
		if (!LoadMapFile("assets/map_data/map.txt", mapData))
			return false;

		GenerateObstaclePoints(mapData, obstacles);

		// store custom meshes
		RS.StoreMesh("road", GenerateRoadMesh(mapData));

		std::shared_ptr<Engine::Mesh> screenQuad = std::make_shared<Engine::Mesh>();
		screenQuad->ScreenQuad();
		RS.StoreMesh("skybox", screenQuad);

		// create prefabs
		prefabs["road"] = CreatePrefab("road");
		prefabs["ground"] = CreatePrefab("ground");
		prefabs["skybox"] = CreatePrefab("skybox");
		prefabs["rock1"] = CreatePrefab("rock1");
		prefabs["rock2"] = CreatePrefab("rock2");
		prefabs["rock3"] = CreatePrefab("rock3");
		prefabs["parabola"] = CreatePrefab("parabola");
		prefabs["ship"] = CreatePrefab("ship");
		prefabs["lindahlium"] = CreatePrefab("lindahlium");
		prefabs["flames"] = CreatePrefab("flames");

		// create gameobjects
		GameObject* road = new GameObject(prefabs["road"]);
		road->cullable = false;
		gameObjects.push_back(road);

		SpawnSideObjects(mapData, {
			prefabs["rock1"], 
			prefabs["rock2"], 
			prefabs["rock3"], 
			prefabs["parabola"]
			}, gameObjects
		);

		for (size_t i = 0; i < obstacles.size(); i++)
		{
			GameObject* obstacle = new GameObject(prefabs["lindahlium"]);
			obstacle->transform.position = glm::vec3(obstacles[i].x, 0.f, obstacles[i].y);
			obstacle->transform.scale *= 0.6f;
			gameObjects.push_back(obstacle);
		}

		GameObject* ground = new GameObject(prefabs["ground"]);
		ground->transform.scale *= 500.f;
		ground->transform.position.y = -0.1f;
		ground->cullable = false;
		gameObjects.push_back(ground);

		player = new Player(prefabs["ship"]);
		gameObjects.push_back(player);

		GameObject* flames = new GameObject(prefabs["flames"]);
		flames->transform.scale = glm::vec3(1.55f, 1.f, 1.f);
		player->exhaust = flames;
		gameObjects.push_back(flames);

		skybox = new GameObject(prefabs["skybox"]);

		// setup text renderer
		textRenderer.Init(RS.GetShader("font"), RS.GetTexture("font"), screenQuad.get(), glm::vec2(51.2f, 64.f));

		float w = (float)window.Width();
		float h = (float)window.Height();
		gameOverTextTransform.position = glm::vec3(-0.7f, 0.f, 0.f);
		gameOverTextTransform.scale = glm::vec3(w/h, 1.f, 1.f) * (64.f / h);
		fpsTextTransform.position = glm::vec3(-0.9f, 0.9f, 0.f);
		fpsTextTransform.scale = glm::vec3(w / h, 1.f, 1.f) * (0.8f * 64.f / h);
		startTextTransform.scale = glm::vec3(w / h, 1.f, 1.f);
		startTextTransform.position = glm::vec3(-5.f, 2.5f, 6.f);
		pointsTransform.position = glm::vec3(-0.9f, 0.75f, 0.f);
		pointsTransform.scale = glm::vec3(w / h, 1.f, 1.f) * (0.8f * 64.f / h);

		// setup camera
		camera.Init(70.f / 180.f * 3.1415f, (float)window.Width() / window.Height(), 0.1f, 200.f);
		cameraTransform.position = player->cameraOffset;

		// set start state
		ChangeState(new PlayGame());

		pointSystem.SetStartPoint(mapData, player->transform.position);
		pointSystem.LoadHighScore();

		return true;
	}

	void App::UpdateLoop()
	{
		float totalTime = 0.f;

		float sunAngle = 0.3f;
		float sunSpeed = 1.f;

		float maxRenderDist = 50.f;

		while (!shouldClose)
		{
			auto t0 = std::chrono::steady_clock::now();

			window.BeginUpdate();
			if (window.ShouldClose())
			{
				shouldClose = true;
				break;
			}

			if (keys[GLFW_KEY_1].held)
			{
				sunAngle += deltaTime * sunSpeed;
			}
			if (keys[GLFW_KEY_2].held)
			{
				sunAngle -= deltaTime * sunSpeed;
			}

			WorldSettings::Instance().directionalLight = glm::normalize(
				glm::quat(glm::vec3(sunAngle, 0.f, 0.f)) * glm::quat(glm::vec3(0.f, 0.2f, 0.f)) *
				glm::vec3(0.f, 0.f, 1.f));

			if (keys[GLFW_KEY_END].pressed)
			{
				bool success = Engine::Shader::ReloadAll();
				printf("[INFO] reloaded shaders, success: %s\n", (success ? "yes" : "no"));
			}

			currentState->Update(this);

			glm::vec3 forwardOffset = -cameraTransform.Forward() * -player->cameraOffset.z;
			forwardOffset.y += player->cameraOffset.y;
			glm::vec3 cameraPosition = player->transform.position + forwardOffset;
			glm::quat cameraRotation = player->transform.rotation;

			cameraTransform.position = glm::mix(cameraTransform.position, cameraPosition, 0.1f);
			cameraTransform.rotation = glm::mix(cameraTransform.rotation, cameraRotation, 0.1f);

			// render
			renderer.SetCamera(camera, cameraTransform);

			skybox->Draw(renderer);
			renderer.ExecuteDrawCalls();

			for (auto& obj : gameObjects)
			{
				glm::vec3 toObj = obj->transform.position - cameraTransform.position;

				// culling
				if (obj->cullable && (
					glm::dot(toObj, toObj) > maxRenderDist * maxRenderDist ||
					glm::dot(toObj, cameraTransform.Forward()) < 0.f
				))
					continue;

				obj->Draw(renderer);
			}
			renderer.ExecuteDrawCalls();

			// 3D text
			glm::mat4 VP = camera.CalcP() * camera.CalcV(cameraTransform);
			textRenderer.DrawText("Start!", VP * startTextTransform.CalcMatrix(), glm::vec3(1.f, 0.8f, 0.f), true);

			currentState->DrawUI(this);

			renderer.RenderToScreen(window.Width(), window.Height());

			window.EndUpdate();

			for (auto& k : keys)
			{
				k.second.pressed = false;
				k.second.released = false;
			}

			auto t1 = std::chrono::steady_clock::now();
			deltaTime = glm::min(1.f / 30.f, std::chrono::duration<float>(t1 - t0).count());
			totalTime += deltaTime;
			WorldSettings::Instance().currentTime = totalTime;
		}
	}

	void App::Close()
	{
		for (auto& e : prefabs)
			delete e.second;

		for (auto& e : gameObjects)
			delete e;

		delete skybox;

		delete currentState;

		Engine::Resources::Instance().CleanUp();

		pointSystem.SaveHighScore();

		renderer.Deinit();

		window.Deinit();
	}

	void App::ChangeState(GameState* newState)
	{
		if (currentState != nullptr)
		{
			delete currentState;
		}

		currentState = newState;
		currentState->Enter(this);
	}

	void App::PlayGame::Enter(App* app)
	{
		app->cameraTransform.position = app->player->cameraOffset;
		app->cameraTransform.rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
		app->player->transform.position *= 0.f;
		app->player->yRotation = 0.f;
		app->player->yAngularVelocity = 0.f;
		app->player->velocityVector *= 0.f;
	}

	void App::PlayGame::Update(App* app)
	{
		Player* player = app->player;

		player->acceleration = player->startAcceleration * (1.f + (float)app->pointSystem.lapsCompleted * 0.2f);

		int gamePadConnected = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (gamePadConnected == 1) {
			int axesCount;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

			//if (!(axes[1] > -0.05f && axes[1] < 0.05f))
			//	player->velocityVector -= player->transform.Forward() * player->acceleration * axes[1];
			
			if (!(axes[0] > -0.05f && axes[0] < 0.05f))
				player->velocityVector += player->transform.Right() * player->acceleration * axes[0];

			if (!(axes[4] > -0.05f && axes[4] < 0.05f))
			{
				player->yAngularVelocity -= app->deltaTime * player->angularAcceleration * axes[4];
			}
			if (!(axes[3] > -0.05f && axes[3] < 0.05f))
			{
				player->yAngularVelocity -= app->deltaTime * player->angularAcceleration * axes[3];
			}
		}

		player->velocityVector += player->transform.Forward() * player->acceleration;

		//if (app->keys[GLFW_KEY_W].held)
		//	player->velocityVector += player->transform.Forward() * player->acceleration;
		/*if (app->keys[GLFW_KEY_S].held)
			player->velocityVector -= player->transform.Forward() * player->acceleration;*/
		if (app->keys[GLFW_KEY_A].held)
			player->velocityVector -= player->transform.Right() * player->acceleration;
		if (app->keys[GLFW_KEY_D].held)
			player->velocityVector += player->transform.Right() * player->acceleration;

		player->velocityVector *= 1 - player->friction;
		if (glm::length(player->velocityVector) > player->maxSpeed)
			player->velocityVector = glm::normalize(player->velocityVector) * player->maxSpeed;

		if (glm::length(player->velocityVector) > 0.f)
			player->transform.position += player->velocityVector * app->deltaTime;

		if (app->keys[GLFW_KEY_LEFT].held)
			player->yAngularVelocity -= app->deltaTime * player->angularAcceleration;
		if (app->keys[GLFW_KEY_RIGHT].held)
			player->yAngularVelocity += app->deltaTime * player->angularAcceleration;

		player->yAngularVelocity *= 1 - player->angularFriction;
		player->yRotation += player->yAngularVelocity * app->deltaTime;

		player->transform.rotation = glm::quat(glm::vec3(0.f, player->yRotation, 0.f));

		player->Update();

		app->pointSystem.PassCheckpoint(app->mapData, player->transform.position);

		if (player->IsColliding(app->mapData, app->obstacles, 5.f, 1.7f))
		{
			app->ChangeState(new GameOver());
			return;
		}
	}

	void App::PlayGame::DrawUI(App* app)
	{
		app->textRenderer.DrawText("fps:" + std::to_string(int(100.f / app->deltaTime) / 100), app->fpsTextTransform.CalcMatrix(), glm::vec3(1.f));
		app->textRenderer.DrawText("POINTS:" + std::to_string(app->pointSystem.currentPoints) + "\n" + "HIGHSCORE:" + std::to_string(app->pointSystem.highScore), app->pointsTransform.CalcMatrix(), glm::vec3(1.f));
	}

	void App::GameOver::Enter(App* app)
	{
		app->player->yAngularVelocity = 0.f;
		app->player->velocityVector *= 0.f;
		app->pointSystem.ResetScore();
		app->pointSystem.SaveHighScore();
	}

	void App::GameOver::Update(App* app)
	{
		waitTimer += app->deltaTime;
		if (waitTimer >= waitDuration)
		{
			app->ChangeState(new PlayGame());
			return;
		}
	}

	void App::GameOver::DrawUI(App* app)
	{
		app->textRenderer.DrawText("Game Over!", app->gameOverTextTransform.CalcMatrix(), glm::vec3(1.f, 0.f, 0.f));
	}
}