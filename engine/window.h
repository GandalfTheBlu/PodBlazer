#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace Engine
{
	struct KeyEvent
	{
		int key, scancode, action, mods;
	};

	class Window
	{
	private:
		int width;
		int height;
		GLFWwindow* window;

		static void StaticKeyCallback(GLFWwindow* _window, int key, int scancode, int action, int mods);

	public:
		Window();
		~Window();

		std::function<void(const KeyEvent&)> KeyCallback;

		bool Init(int _width, int _height, const char* title);
		void Deinit();

		bool ShouldClose();
		void BeginUpdate();
		void EndUpdate();

		int Width() const;
		int Height() const;
	};
}