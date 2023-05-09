#include "game_app.h"

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

		return true;
	}

	void App::UpdateLoop()
	{
		Engine::Shader shader;
		shader.Init("assets\\shaders\\full_screen");
		
		Engine::Mesh mesh;
		mesh.ScreenQuad();

		Engine::Texture texture;
		texture.Init("assets\\textures\\test.png");

		Engine::Material material
		{
			&shader,
			[&texture](Engine::Shader* s)
			{
				texture.Bind(GL_TEXTURE0);
			},
			[&texture](Engine::Shader* s)
			{
				texture.Unbind(GL_TEXTURE0);
			}
		};

		Engine::Transform transform1 
		{
			glm::vec3(0.f,0.f,3.f),
			glm::quat(1.f,0.f,0.f,0.f),
			glm::vec3(1.f,1.f,1.f)
		};

		Engine::Transform transform2
		{
			glm::vec3(0.f,0.f,2.f),
			glm::quat(1.f,0.f,0.f,0.f),
			glm::vec3(0.5f,0.5f,1.f)
		};

		Engine::Camera camera;
		camera.Init(70.f/180.f*3.1415f, (float)window.Width() / window.Height(), 0.1f, 100.f);

		Engine::Transform cameraTransform
		{
			glm::vec3(0.f,0.f,0.f),
			glm::quat(1.f,0.f,0.f,0.f),
			glm::vec3(1.f,1.f,1.f)
		};

		while (!shouldClose)
		{
			window.BeginUpdate();
			if (window.ShouldClose())
			{
				shouldClose = true;
				break;
			}

			transform2.rotation *= glm::quat(glm::vec3(0.f, 0.02f, 0.f));
			
			renderer.Draw(&mesh, { material }, transform1);
			renderer.Draw(&mesh, { material }, transform2);
			renderer.Render(camera, cameraTransform);

			window.EndUpdate();
		}
	}

	void App::Close()
	{
		window.Deinit();
	}
}