#include <iostream>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Resources.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Renderer.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

bool Renderer::Initialize(Graphics& graphics)
{
	m_shader = Resources::LoadShader("diffuse.vertex", "diffuse.fragment");

	auto	loadHumanoidTask = Mesh::LoadFromFileAsync(graphics, L"humanoid.cmo", L"", L"", m_meshes);
	//loadHumanoidTask.wait();

	//static const GLfloat g_vertex_buffer_data[] = {
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//};

	//// Generate 1 buffer, put the resulting identifier in vertexbuffer
	//glGenBuffers(1, &m_vertexBuffer);

	//// The following commands will talk about our 'vertexbuffer' buffer
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	//// Give our vertices to OpenGL.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return true;
}

void Renderer::Deinitialize()
{

}

void Renderer::Render(Graphics& graphics)
{
	// Use our shader
	glUseProgram(m_shader);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 800), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 50, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(m_shader, "MVP");

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	for (auto& mesh : m_meshes)
	{
		mesh->Render(graphics, Model);

		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		//glVertexAttribPointer(
		//	0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		//	3,                  // size
		//	GL_FLOAT,           // type
		//	GL_FALSE,           // normalized?
		//	0,                  // stride
		//	(void*)0            // array buffer offset
		//	);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, )

		//	// Draw the triangle !
		//	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

		//glDisableVertexAttribArray(0);
	}
}
