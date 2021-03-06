#include <iostream>

#include "RendererApp.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App : public RendererApp
{
public:
	App() {}
	~App() {}

	bool	OnInitialize() override
	{
		m_graphics.reset(new Graphics);
		m_graphics->SetWindow(m_window);
		m_renderer.reset(new Renderer);

		if (!m_renderer->Initialize(*m_graphics.get()))
			return false;

		return true;
	}

	void	OnDeinitialize() override
	{
		m_graphics.reset(nullptr);
		m_renderer.reset(nullptr);
	}

	void	OnRender() override
	{
		m_graphics->UpdateGenBuffers();
		m_renderer->Render(*m_graphics.get());
	}
};

int create_window()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	App	app;
	app.SetWindow(window);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = true; // Needed in core profile 

	if (glewInit() != GLEW_OK) {
		return -1;
	}

	if (!app.Initialize())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render here */
		app.Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	app.Deinitialize();
	return 0;
}

int main(int ac, char* av[])
{
	int ret = create_window();

	return ret;
}