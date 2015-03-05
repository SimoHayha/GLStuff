#pragma once

#include <memory>

#include "Graphics.h"
#include "Renderer.h"

class RendererApp
{
public:
	RendererApp();
	virtual ~RendererApp();

	virtual bool	Initialize();
	virtual void	Deinitialize();
	virtual void	Render();

protected:
	std::unique_ptr<Graphics>	m_graphics;
	std::unique_ptr<Renderer>	m_renderer;
};