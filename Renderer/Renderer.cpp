#include <iostream>
#include <random>
#include <thread>
#include <chrono>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Resources.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Renderer.h"

#include <gl\glew.h>
#include <GLFW/glfw3.h>

Renderer::Renderer()
{
	m_ready = false;
}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(Graphics& graphics)
{
	m_shader = Resources::LoadShader("diffuse.vertex", "diffuse.fragment");

	if (!m_meshes.Initialize())
		return false;
	if (!m_meshLoader.Initialize(&m_meshes, 2))
		return false;

	int numMeshes = 1000;
	for (int i = 0; i < numMeshes; ++i)
	{
		/* TODO: Implement mesh loader with maximum async operation
		** Right now a too big number of meshes at the same time, will not work, the system cannot create as much OpenGL context
		*/
		//Mesh::LoadFromFileAsync(m_ready, graphics, L"humanoid.cmo", L"", L"", &m_meshes);
		if (i % 2 == 0)
			m_meshLoader.LoadMesh(L"humanoid.cmo", L"", L"");
		else
			m_meshLoader.LoadMesh(L"teapot.cmo", L"", L"");
	}

	return true;
}

void Renderer::Deinitialize()
{

}

void Renderer::Render(Graphics& graphics)
{
	m_meshLoader.CollectMeshes();

	// Use our shader
	glUseProgram(m_shader);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 50000.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 10000), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 50, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(m_shader, "MVP");

	m_meshes.Render(graphics, MatrixID, Projection, View);
}
