#include "game_app.h"
#include "world_settings.h"
#include <chrono>
#include "file_reader.h"
#include "player.h"

namespace Game
{
	App::App() :
		shouldClose(false)
	{}

	App::~App(){}

	bool App::Open()
	{
		if (!window.Init(1400, 1000, "Pod Blazer"))
			return false;

		if (!Prefab::Init("assets/shaders/phong"))
			return false;

		if (!skyboxShader.Init("assets/shaders/skybox"))
			return false;

		std::vector<glm::vec2> mapData;
		if (!Engine::ReadMapFile("assets/map_data/map.txt", mapData))
			return false;

		renderer.Init({
			&skyboxShader,
			[](const Engine::MaterialBindInput& inp)
			{
				glm::mat3 invMVP = glm::inverse(glm::mat3(inp.MVP));
				inp.shader->SetMat3("u_invMVP", &invMVP[0][0]);
				inp.shader->SetVec3("u_lightDir", &WorldSettings::Instance().directionalLight[0]);
			},
			nullptr
		});

		prefabs["astronaut"] = new Prefab("assets/kenney_space-kit/Models/OBJ format/craft_speederA.obj");

		for (size_t i = 0; i < 50; i++)
		{
			GameObject* obj = new GameObject(prefabs["astronaut"]);
			obj->transform.position = glm::vec3(-50.f + i, 0.f, 10.f);
			gameObjects.push_back(obj);
		}

		camera.Init(70.f / 180.f * 3.1415f, (float)window.Width() / window.Height(), 0.1f, 200.f);

		return true;
	}

	void App::UpdateLoop()
	{
		Engine::Transform cameraTransform;

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

		Player* player = new Player(prefabs["astronaut"]);
		player->transform.position = glm::vec3(-50.f + 70, 0.f, 10.f);

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
				}
				if (!(axes[0] > -0.05f && axes[0] < 0.05f))
				{
					//move += player->transform.Right() * axes[0];
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
			
			/*
			if (keys[GLFW_KEY_UP].held)
			{
				rotX += dt * rotSpeed;
			}
			if (keys[GLFW_KEY_DOWN].held)
			{
				rotX -= dt * rotSpeed;
			}
			*/
			if (keys[GLFW_KEY_LEFT].held)
			{
				rotY -= dt * rotSpeed;
			}
			if (keys[GLFW_KEY_RIGHT].held)
			{
				rotY += dt * rotSpeed;
			}

			player->transform.rotation = glm::quat(glm::vec3(0.f, rotY, 0.f)) * glm::quat(glm::vec3(rotX, 0.f, 0.f));

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

			// render
			player->Draw(renderer);
			glm::vec3 forwardOffset = -cameraTransform.Forward() * -player->cameraOffset.z;
			forwardOffset.y += player->cameraOffset.y;
			glm::vec3 cameraPosition = player->transform.position +forwardOffset;
			glm::quat cameraRotation = player->transform.rotation * glm::quat(glm::vec3(0.f, glm::radians(0.0f), 0.f));

			cameraTransform.position = cameraPosition;
			cameraTransform.rotation = cameraRotation;
			renderer.SetCamera(camera, cameraTransform);
			float offset = 0.f;
			for (auto& obj : gameObjects)
			{
				obj->transform.rotation = glm::quat(glm::vec3(0.f, totalTime + offset, 0.f));
				obj->Draw(renderer);
				offset += 0.3f;
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

		skyboxShader.Deinit();
		renderer.Deinit();
		Prefab::Deinit();
		window.Deinit();
	}
}