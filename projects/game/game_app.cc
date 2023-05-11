#include "game_app.h"
#include "world_settings.h"
#include <chrono>
#include "file_reader.h"

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

		while (!shouldClose)
		{
			auto t0 = std::chrono::steady_clock::now();

			window.BeginUpdate();
			if (window.ShouldClose())
			{
				shouldClose = true;
				break;
			}

			glm::vec3 move = glm::vec3(0.f);
			if (keys[GLFW_KEY_W].held)
			{
				move += cameraTransform.Forward();
			}
			if (keys[GLFW_KEY_S].held)
			{
				move -= cameraTransform.Forward();
			}
			if (keys[GLFW_KEY_A].held)
			{
				move -= cameraTransform.Right();
			}
			if (keys[GLFW_KEY_D].held)
			{
				move += cameraTransform.Right();
			}

			if(glm::dot(move, move) > 0.f)
				cameraTransform.position += glm::normalize(move) * (dt * moveSpeed);
			
			if (keys[GLFW_KEY_UP].held)
			{
				rotX += dt * rotSpeed;
			}
			if (keys[GLFW_KEY_DOWN].held)
			{
				rotX -= dt * rotSpeed;
			}
			if (keys[GLFW_KEY_LEFT].held)
			{
				rotY -= dt * rotSpeed;
			}
			if (keys[GLFW_KEY_RIGHT].held)
			{
				rotY += dt * rotSpeed;
			}

			cameraTransform.rotation = glm::quat(glm::vec3(0.f, rotY, 0.f)) * glm::quat(glm::vec3(rotX, 0.f, 0.f));

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