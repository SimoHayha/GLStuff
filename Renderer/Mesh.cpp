#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>

#include "RawMesh.h"
#include "MeshSlotMap.h"
#include "Mesh.h"

#include <gl\glew.h>
#include <GLFW/glfw3.h>

void Mesh::Render(Graphics& graphics, glm::mat4x4 const& model)
{
	for (SubMesh& submesh : m_rawMesh->Data().SubMeshes)
	{
		GLuint	vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffers[submesh.VertexBufferIndex]);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(Vertex),     // stride
			(void*)0            // array buffer offset
			);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[submesh.IndexBufferIndex]);

		glDrawElements(GL_TRIANGLES, submesh.PrimCount * 3, GL_UNSIGNED_SHORT, (void*)0); // Starting from vertex 0; 3 vertices total -> 1 triangle

		glDisableVertexAttribArray(0);
		glDeleteVertexArrays(1, &vertexArrayID);

	}
}

int Mesh::GetIndex() const
{
	return m_index;
}

int Mesh::GetVersion() const
{
	return m_version;
}

void Mesh::SetIndex(int index)
{
	m_index = index;
}

void Mesh::SetVersion(int version)
{
	m_version = version;
}

Mesh::Mesh() :
m_index(-1),
m_version(-1),
m_ready(false)
{
}

bool Mesh::IsReady() const
{
	return m_ready;
}

bool Mesh::Initialize(RawMesh* rawMesh)
{
	m_rawMesh = rawMesh;

	int	vertexBufferCount = m_rawMesh->Data().Vertices.size();
	int	indexBufferCount = m_rawMesh->Data().Indices.size();

	m_vertexBuffers.resize(vertexBufferCount);
	m_indexBuffers.resize(indexBufferCount);

	for (uint32_t i = 0u; i < vertexBufferCount; ++i)
	{
		glGenBuffers(1, &m_vertexBuffers[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffers[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_rawMesh->Data().Vertices[i].size() * sizeof(Vertex), &m_rawMesh->Data().Vertices[i][0], GL_STATIC_DRAW);
	}

	for (uint32_t i = 0u; i < indexBufferCount; ++i)
	{
		glGenBuffers(1, &m_indexBuffers[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffers[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_rawMesh->Data().Indices[i].size() * sizeof(USHORT), &m_rawMesh->Data().Indices[i][0], GL_STATIC_DRAW);
	}

	m_ready = true;


	//			mesh.m_indexBuffers[i] = task->BuffersIds[0];
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indexBuffers[i]);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibCount * sizeof(USHORT), &indexBuffers[i][0], GL_STATIC_DRAW);

	//			mesh.m_vertexBuffers[i] = task->BuffersIds[0];
	//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_vertexBuffers[i]);
	//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vbCount * sizeof(Vertex), &vertexBuffers[i][0], GL_STATIC_DRAW);

	return true;
}
