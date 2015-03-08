#include "RendererApp.h"


RendererApp::RendererApp() :
m_window(nullptr)
{

}

RendererApp::~RendererApp()
{

}

bool RendererApp::Initialize()
{
	assert(m_window != nullptr);

	return OnInitialize();
}

void RendererApp::Deinitialize()
{
	return OnDeinitialize();
}

void RendererApp::Render()
{
	return OnRender();
}

void RendererApp::SetWindow(GLFWwindow* window)
{
	m_window = window;
}