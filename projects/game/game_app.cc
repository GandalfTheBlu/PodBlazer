#include "game_app.h"
#include "world_settings.h"
#include <chrono>

namespace Game
{
	App::App() :
		shouldClose(false)
	{}

	App::~App(){}

	bool App::Open()
	{
		if (!window.Init(800, 600, "Pod Blazer"))
			return false;

		if (!GameObjectInstance::Init("assets\\shaders\\phong"))
			return false;

		if (!skyboxShader.Init("assets\\shaders\\skybox"))
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

		prefabs["astronaut"] = new GameObjectInstance("assets\\kenney_space-kit\\Models\\OBJ format\\astronautA.obj");

		for (size_t i = 0; i < 50; i++)
		{
			GameObject* obj = new GameObject(prefabs["astronaut"]);
			obj->transform.position = glm::vec3(-50.f + i, 0.f, 10.f);
			gameObjects.push_back(obj);
		}

		WorldSettings::Instance().directionalLight = glm::normalize(glm::vec3(-1.f, -1.f, 1.f));

		camera.Init(70.f / 180.f * 3.1415f, (float)window.Width() / window.Height(), 0.1f, 200.f);

		return true;
	}

	void App::UpdateLoop()
	{
		Engine::Transform cameraTransform;

		std::unordered_map<int, bool> keys;

		window.KeyCallback = [&keys](const Engine::KeyEvent& e)
		{
			if (e.action == GLFW_PRESS)
				keys[e.key] = true;
			else if (e.action == GLFW_RELEASE)
				keys[e.key] = false;
		};

		float dt = 1.f / 60.f;
		float totalTime = 0.f;

		float rotX = 0.f;
		float rotY = 0.f;
		float rotSpeed = 2.f;
		float moveSpeed = 10.f;

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
			if (keys[GLFW_KEY_W])
			{
				move += cameraTransform.Forward();
			}
			if (keys[GLFW_KEY_S])
			{
				move -= cameraTransform.Forward();
			}
			if (keys[GLFW_KEY_A])
			{
				move -= cameraTransform.Right();
			}
			if (keys[GLFW_KEY_D])
			{
				move += cameraTransform.Right();
			}

			if(glm::dot(move, move) > 0.f)
				cameraTransform.position += glm::normalize(move) * (dt * moveSpeed);
			
			if (keys[GLFW_KEY_UP])
			{
				rotX += dt * rotSpeed;
			}
			if (keys[GLFW_KEY_DOWN])
			{
				rotX -= dt * rotSpeed;
			}
			if (keys[GLFW_KEY_LEFT])
			{
				rotY -= dt * rotSpeed;
			}
			if (keys[GLFW_KEY_RIGHT])
			{
				rotY += dt * rotSpeed;
			}

			cameraTransform.rotation = glm::quat(glm::vec3(0.f, rotY, 0.f)) * glm::quat(glm::vec3(rotX, 0.f, 0.f));

			// render
			renderer.SetCamera(camera, cameraTransform);
			for (auto& obj : gameObjects)
			{
				obj->Draw(renderer);
			}
			renderer.Render();

			window.EndUpdate();

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

		GameObjectInstance::Deinit();

		window.Deinit();
	}
}