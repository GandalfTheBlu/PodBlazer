#include "window.h"
#include <stdio.h>
#include <string>

#define GLFW_INCLUDE_ES3
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#define LogError(msg) printf("[ERROR] %s\n", msg)
#define LogInfo(msg) printf("[INFO] %s\n", msg)

void GLAPIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string msg = "[OPENGL] ";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		msg.append("[LOW SEVERITY] ");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		msg.append("[MEDIUM SEVERITY] ");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		msg.append("[HIGH SEVERITY] ");
		break;
	}

	msg.append(message);

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		LogError(msg.c_str());
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		LogInfo(msg.c_str());
		break;
	}
}

namespace Engine
{
	Window::Window() :
		width(-1),
		height(-1),
		window(nullptr)
	{}

	Window::~Window() {}

	bool Window::Init(int _width, int _height, const char* title)
	{
		// setup glfw to get a window
		if (!glfwInit())
		{
			LogError("failed to initialize glfw");
			return false;
		}

		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
		//glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glewExperimental = true;

		width = _width;
		height = _height;
		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		glfwMakeContextCurrent(window);

		// setup glew to get opengl stuff
		if (glewInit() != GLEW_OK)
		{
			LogError("falied to initialize glew");
			return false;
		}

				const GLubyte* vendor = glGetString(GL_VENDOR);
		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* version = glGetString(GL_VERSION);
		const GLubyte* shdVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		printf("GPU Vendor: %s\n", vendor);
		printf("GPU Render Device: %s\n", renderer);
		printf("OpenGL Version: %s\n", version);
		printf("GLSL Version: %s\n", shdVersion);

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		if (glDebugMessageCallback)
		{
			glDebugMessageCallback(GLDebugCallback, nullptr);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glViewport(0, 0, width, height);

		return true;
	}

	void Window::Deinit()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void Window::BeginUpdate()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
	}

	void Window::EndUpdate()
	{
		glfwSwapBuffers(window);
	}

	int Window::Width() const
	{
		return width;
	}

	int Window::Height() const
	{
		return height;
	}
}