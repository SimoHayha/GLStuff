#pragma once

#include <memory>

#include "Graphics.h"
#include "Renderer.h"

#include <gl\glew.h>
#include <GLFW/glfw3.h>

class RendererApp
{
public:
	RendererApp();
	virtual ~RendererApp();

	bool	Initialize();
	void	Deinitialize();
	void	Render();

	void	SetWindow(GLFWwindow* window);

protected:
	GLFWwindow*					m_window;
	std::unique_ptr<Graphics>	m_graphics;
	std::unique_ptr<Renderer>	m_renderer;

	virtual bool	OnInitialize() = 0;
	virtual void	OnDeinitialize() = 0;
	virtual void	OnRender() = 0;
};