#include "game_app.h"
#include "world_settings.h"
#include <chrono>
#include "file_reader.h"
#include "map_generator.h"
#include "resources.h"
#include "materials.h"

namespace Game
{
	App::App() :
		shouldClose(false),
		skybox(nullptr),
		player(nullptr)
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

		// load all shaders
		if (!LoadShader("assets/shaders/phong", "phong") ||
			!LoadShader("assets/shaders/skybox", "skybox"))
			return false;

		std::string objPath = "assets/kenney_space-kit/Models/OBJ format/";

		// load all meshes and materials from obj files
		if (!LoadObjMeshAndMaterials(objPath + "rock_crystals.obj", "rock1", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "rock_crystalsLargeA.obj", "rock2", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "rock_largeA.obj", "rock3", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "satelliteDish.obj", "parabola", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "terrain.obj", "ground", "phong") ||
			!LoadObjMeshAndMaterials(objPath + "craft_speederC.obj", "ship", "phong"))
			return false;

		// store custom materials
		std::shared_ptr<FlatMaterial> roadMaterial = std::make_shared<FlatMaterial>();
		roadMaterial->shader = Engine::Resources::Instance().GetShader("phong");
		roadMaterial->color = glm::vec3(0.5f);
		Engine::Resources::Instance().StoreMaterial("road0", roadMaterial);

		std::shared_ptr<SkyboxMaterial> skyboxMaterial = std::make_shared<SkyboxMaterial>();
		skyboxMaterial->shader = Engine::Resources::Instance().GetShader("skybox");
		Engine::Resources::Instance().StoreMaterial("skybox0", skyboxMaterial);

		// load road path
		if (!LoadMapFile("assets/map_data/map.txt", mapData))
			return false;

		// store custom meshes
		Engine::Resources::Instance().StoreMesh("road", GenerateRoadMesh(mapData));

		std::shared_ptr<Engine::Mesh> screenQuad = std::make_shared<Engine::Mesh>();
		screenQuad->ScreenQuad();
		Engine::Resources::Instance().StoreMesh("skybox", screenQuad);

		// create prefabs
		prefabs["road"] = CreatePrefab("road");
		prefabs["ground"] = CreatePrefab("ground");
		prefabs["skybox"] = CreatePrefab("skybox");
		prefabs["rock1"] = CreatePrefab("rock1");
		prefabs["rock2"] = CreatePrefab("rock2");
		prefabs["rock3"] = CreatePrefab("rock3");
		prefabs["parabola"] = CreatePrefab("parabola");
		prefabs["ship"] = CreatePrefab("ship");

		// create gameobjects
		GameObject* road = new GameObject(prefabs["road"]);
		road->cullable = false;
		gameObjects.push_back(road);

		SpawnSideObjects(mapData, {prefabs["rock1"], prefabs["rock2"], prefabs["rock3"], prefabs["parabola"]}, gameObjects);

		GameObject* ground = new GameObject(prefabs["ground"]);
		ground->transform.scale *= 500.f;
		ground->transform.position.y = -0.1f;
		ground->cullable = false;
		gameObjects.push_back(ground);

		player = new Player(prefabs["ship"]);
		gameObjects.push_back(player);

		skybox = new GameObject(prefabs["skybox"]);

		// setup camera
		camera.Init(70.f / 180.f * 3.1415f, (float)window.Width() / window.Height(), 0.1f, 200.f);

		return true;
	}

	void App::UpdateLoop()
	{
		struct KeyState
		{
			bool pressed;
			bool held;
			bool released;
		};

		std::unordered_map<int, KeyState> keys;

		window.KeyCallback = [&keys](const Engine::KeyEvent& e)
		{
			if (e.action == GLFW_PRESS)
				keys[e.key] = {true, true, false};
			else if (e.action == GLFW_RELEASE)
				keys[e.key] = {false, false, true};
		};

		float dt = 1.f / 60.f;
		float totalTime = 0.f;

		float rotX = 0.f;
		float rotY = 0.f;
		float rotSpeed = 2.f;
		float moveSpeed = 10.f;
		float sunAngle = 0.3f;
		float sunSpeed = 1.f;

		Engine::Transform cameraTransform;
		cameraTransform.position = player->cameraOffset;

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


			//glm::vec3 move = glm::vec3(0.f);

			int gamePadConnected = glfwJoystickPresent(GLFW_JOYSTICK_1);
			if (gamePadConnected == 1) {
				int axesCount;
				const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

				if (!(axes[1] > -0.05f && axes[1] < 0.05f))
				{
					//move -= player->transform.Forward() * axes[1];
					player->velocityVector -= player->transform.Forward() * player->acceleration * axes[1];
				}
				if (!(axes[0] > -0.05f && axes[0] < 0.05f))
				{
					//move += player->transform.Right() * axes[0];
					player->velocityVector += player->transform.Right() * player->acceleration * axes[0];
				}


				if (!(axes[4] > -0.05f && axes[4] < 0.05f))
				{
					rotX += dt * rotSpeed * axes[4];
				}
				if (!(axes[3] > -0.05f && axes[3] < 0.05f))
				{
					rotY += dt * rotSpeed * axes[3];
				}

			}


			if (keys[GLFW_KEY_W].held)
			{
				player->velocityVector += player->transform.Forward() * player->acceleration;

			}
			if (keys[GLFW_KEY_S].held)
			{
				player->velocityVector -= player->transform.Forward() * player->acceleration;
			}
			if (keys[GLFW_KEY_A].held)
			{
				player->velocityVector -= player->transform.Right() * player->acceleration;
			}
			if (keys[GLFW_KEY_D].held)
			{
				player->velocityVector += player->transform.Right() * player->acceleration;
			}

			player->velocityVector *= 1-player->friction;
			if (glm::length(player->velocityVector) > player->maxSpeed)
			{
				player->velocityVector = glm::normalize(player->velocityVector) * player->maxSpeed;
			}

			if (glm::length(player->velocityVector) > 0.f)
			{
				player->transform.position += player->velocityVector * dt;
			}
			
			if (keys[GLFW_KEY_LEFT].held)
			{
				player->yAngularVelocity -= dt * player->angularAcceleration;
			}
			if (keys[GLFW_KEY_RIGHT].held)
			{
				player->yAngularVelocity += dt * player->angularAcceleration;
			}
			player->yAngularVelocity *= 1 - player->angularFriction;
			player->yRotation += player->yAngularVelocity * dt;

			player->transform.rotation = glm::quat(glm::vec3(0.f, player->yRotation, 0.f)) * glm::quat(glm::vec3(rotX, 0.f, 0.f));

			if (keys[GLFW_KEY_1].held)
			{
				sunAngle += dt * sunSpeed;
			}
			if (keys[GLFW_KEY_2].held)
			{
				sunAngle -= dt * sunSpeed;
			}

			WorldSettings::Instance().directionalLight = glm::normalize(
				glm::quat(glm::vec3(sunAngle, 0.f, 0.f)) * glm::quat(glm::vec3(0.f, 0.2f, 0.f)) *
				glm::vec3(0.f, 0.f, 1.f));

			if (keys[GLFW_KEY_END].pressed)
			{
				bool success = Engine::Shader::ReloadAll();
				printf("[INFO] reloaded shaders, success: %s\n", (success ? "yes" : "no"));
			}

			glm::vec3 forwardOffset = -cameraTransform.Forward() * -player->cameraOffset.z;
			forwardOffset.y += player->cameraOffset.y;
			glm::vec3 cameraPosition = player->transform.position + forwardOffset;
			glm::quat cameraRotation = player->transform.rotation * glm::quat(glm::vec3(0.f, glm::radians(0.0f), 0.f));
			
			cameraTransform.position = glm::mix(cameraTransform.position, cameraPosition, 0.1f);
			cameraTransform.rotation = glm::mix(cameraTransform.rotation, cameraRotation, 0.1f);

			if (player->IsColliding(mapData, 5.f))
			{
				cameraTransform.position = player->cameraOffset;
				cameraTransform.rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
				player->transform.position *= 0.f;
				player->yRotation = 0.f;
				player->yAngularVelocity = 0.f;
				player->velocityVector *= 0.f;
			}

			// render
			renderer.SetCamera(camera, cameraTransform);

			skybox->Draw(renderer);
			renderer.Render();

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

			renderer.Render();

			window.EndUpdate();

			for (auto& k : keys)
			{
				k.second.pressed = false;
				k.second.released = false;
			}

			auto t1 = std::chrono::steady_clock::now();
			dt = glm::min(1.f / 30.f, std::chrono::duration<float>(t1 - t0).count());
			totalTime += dt;
		}
	}

	void App::Close()
	{
		for (auto& e : prefabs)
			delete e.second;

		for (auto& e : gameObjects)
			delete e;

		delete skybox;

		Engine::Resources::Instance().CleanUp();

		window.Deinit();
	}
}