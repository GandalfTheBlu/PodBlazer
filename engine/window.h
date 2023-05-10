#pragma once
#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <nanovg.h>


namespace Engine
{
	class Window
	{
	private:
		int width;
		int height;
		GLFWwindow* window;

	public:
		Window();
		~Window();

		bool Init(int _width, int _height, const char* title);
		void Deinit();

		bool ShouldClose();
		void BeginUpdate();
		void EndUpdate();

		int Width() const;
		int Height() const;
	};
}