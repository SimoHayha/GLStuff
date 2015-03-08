#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>

//#include <codecvt>
//#include <fcntl.h>
//#include <io.h>
//#include <intrin.h>

#include "Mesh.h"

#include <gl\glew.h>
#include <GLFW/glfw3.h>

MeshStream::MeshStream(std::ifstream& stream) :
m_stream(stream)
{
}

uint32_t MeshStream::ReadUInt32()
{
	uint32_t	u32;
	m_stream.read(reinterpret_cast<char*>(&u32), sizeof(uint32_t));
	return u32;
}

void MeshStream::ReadString(std::wstring* output)
{
	uint32_t	count = ReadUInt32();
	ReadString(output, count);
}

void MeshStream::ReadString(std::wstring* output, uint32_t count)
{
	if (count == 0u)
		return;

	std::vector<wchar_t>	characters(count);
	ReadStruct(&characters[0], count * sizeof(wchar_t));
	*output = &characters[0];
}

std::ifstream& MeshStream::file()
{
	return m_stream;
}

GLbyte MeshStream::ReadByte()
{
	BYTE	b;
	m_stream.read(reinterpret_cast<char*>(&b), sizeof(BYTE));
	return b;
}

uint16_t MeshStream::ReadUInt16()
{
	uint16_t	u16;
	m_stream.read(reinterpret_cast<char*>(&u16), sizeof(uint16_t));
	return u16;
}

std::future<void> Mesh::LoadFromFileAsync(Graphics& graphics, std::wstring const& meshFilename, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation, std::vector<Mesh*>& loadedMesh, bool clearLoadedMeshesVector /*= true*/)
{
	if (clearLoadedMeshesVector)
		loadedMesh.clear();

	glewExperimental = true; // Needed in core profile 
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* threadWin = glfwCreateWindow(1, 1, "Thread Window", NULL, graphics.GetWindow());

	auto	fct = std::async([&graphics, threadWin, meshFilename, shaderPathLocation, texturePathLocation, &loadedMesh](/*GLFWwindow* threadWin, Graphics& graphics, std::wstring const& meshFilename, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation, std::vector<Mesh*>* loadedMesh*/)
	{
		glfwMakeContextCurrent(threadWin);
		GLenum	code = glewInit();
		if (code != GLEW_OK)
			std::cerr << "Cannot init glew for thread" << std::endl;

		std::ifstream	file;
		file.open(meshFilename, std::ios::binary);
		if (!file.is_open())
			return;

		file.seekg(0, std::ios::end);
		std::streamsize	size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::wcout << "Reading " << meshFilename << std::endl;
		MeshStream	stream(file);

		uint32_t	remainingMeshesToRead = stream.ReadUInt32();

		for (uint32_t i = 0u; i < remainingMeshesToRead; ++i)
		{
			Mesh*	mesh = Read(graphics, stream, shaderPathLocation, texturePathLocation);
			if (mesh)
				loadedMesh.push_back(mesh);
		}

		glfwDestroyWindow(threadWin);

	});

	return fct;
}

Mesh* Mesh::Read(Graphics& graphics, MeshStream& stream, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation)
{
	std::cout << "Submesh" << std::endl;

	std::vector<std::future<void>>	innerTasks;

	Mesh*	mesh = new Mesh();

	stream.ReadString(&mesh->m_name);
	std::wcout << "Name: " << mesh->m_name << std::endl;

	// Read material count
	uint32_t	numMaterials = stream.ReadUInt32();
	mesh->m_materials.resize(numMaterials);
	std::wcout << "Material count: " << numMaterials << std::endl;

	// Load each material
	for (uint32_t i = 0u; i < numMaterials; ++i)
	{
		std::cout << "\nMaterial" << std::endl;
		Material&	material = mesh->m_materials[i];

		// Read the material name
		stream.ReadString(&material.Name);
		std::wcout << "Name:" << material.Name << std::endl;

		// Read the ambient and diffuse properties of material
		stream.ReadStruct(&material.Ambient, sizeof(material.Ambient));
		stream.ReadStruct(&material.Diffuse, sizeof(material.Diffuse));
		stream.ReadStruct(&material.Specular, sizeof(material.Specular));
		stream.ReadStruct(&material.SpecularPower);
		stream.ReadStruct(&material.Emissive, sizeof(material.Emissive));
		stream.ReadStruct(&material.UVTransform);

		std::cout << "Ambient" << material.Ambient[0] << "," << material.Ambient[1] << "," << material.Ambient[2] << "," << material.Ambient[3] << std::endl;
		std::cout << "Diffuse" << material.Diffuse[0] << "," << material.Diffuse[1] << "," << material.Diffuse[2] << "," << material.Diffuse[3] << std::endl;
		std::cout << "Specular" << material.Specular[0] << "," << material.Specular[1] << "," << material.Specular[2] << "," << material.Specular[3] << std::endl;
		std::cout << "SpecularPower" << material.SpecularPower << std::endl;
		std::cout << "Emissive" << material.Emissive[0] << "," << material.Emissive[1] << "," << material.Emissive[2] << "," << material.Emissive[3] << std::endl;

		// TODO: do something
		// Assign the vertex shader and sampler state
		//material.VertexShader = graphics.GetVertexShader();
		//material.SamplerState = graphics.GetSamplerState();

		// Read the size of the name of the pixel shader
		uint32_t	stringLen = stream.ReadUInt32();
		if (stringLen)
		{
			// Read the pixel shader if name not empty
			std::wstring	sourceFile;
			stream.ReadString(&sourceFile, stringLen);

			// Continue loading pixel shader if name not empty
			if (!sourceFile.empty())
			{
				// Create well-formed file name for the pixel shader
				Mesh::StripPath(sourceFile);

				// Append the base path
				sourceFile = shaderPathLocation + sourceFile;

				// TODO: load the pixel shader
				// Get or create the pixel shader
				//innerTasks.push_back(std::async([&material, graphics, sourceFile](Graphics& graphics, std::wstring const& sourceFile)
				//{
				//	//material.PixelShader = graphics.GetOrCreatePixelShader(sourceFile);
				//}));
			}
		}

		// Load the textures
		for (int t = 0; t < MaxTexture; ++t)
		{
			// Read the size of the name of the texture
			stringLen = stream.ReadUInt32();
			if (stringLen > 0)
			{
				// Read the texture name
				std::wstring	sourceFile;
				stream.ReadString(&sourceFile, stringLen);

				// Append the base path
				sourceFile = texturePathLocation + sourceFile;

				// TODO: load the texture
				// Get or create the texture
				//innerTasks.push_back(std::async([&material, t, graphics, sourceFile](Graphics& graphics, std::wstring const& sourceFile)
				//{
				//	//material.Textures[t] = graphics.GetOrCreateTexture(sourceFile);
				//}));
			}
		}
	}

	// Does this object contain sekeletal animation?
	uint16_t	isSkeletalDataPresent = stream.ReadByte();

	// Read the submesh information
	uint32_t	numSubmeshes = stream.ReadUInt32();
	mesh->m_submeshes.resize(numSubmeshes);
	for (uint32_t i = 0u; i < numSubmeshes; ++i)
	{
		SubMesh&	submesh = mesh->m_submeshes[i];
		submesh.MaterialIndex = stream.ReadUInt32();
		submesh.IndexBufferIndex = stream.ReadUInt32();
		submesh.VertexBufferIndex = stream.ReadUInt32();
		submesh.StartIndex = stream.ReadUInt32();
		submesh.PrimCount = stream.ReadUInt32();
	}

	// Read the index buffers
	uint32_t	numIndexBuffers = stream.ReadUInt32();
	mesh->m_indexBuffers.resize(numIndexBuffers);

	std::vector<std::vector<USHORT>> indexBuffers(numIndexBuffers);

	for (uint32_t i = 0u; i < numIndexBuffers; ++i)
	{
		uint32_t	ibCount = stream.ReadUInt32();
		if (ibCount > 0u)
		{
			indexBuffers[i].resize(ibCount);

			// Read in the index data
			for (USHORT& component : indexBuffers[i])
				component = stream.ReadUInt16();

			GenBuffers*	task = new GenBuffers();
			task->ThreadId = std::this_thread::get_id().hash();
			task->Count = 1;

			graphics.AddGenBuffers(task);

			std::unique_lock<std::mutex>	lock(*task->Mutex);
			task->Condition.wait(lock);
			lock.unlock();

			mesh->m_indexBuffers[i] = task->BuffersIds[0];
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indexBuffers[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibCount * sizeof(USHORT), &indexBuffers[i][0], GL_STATIC_DRAW);

			std::cout << glewGetErrorString(glGetError()) << std::endl;

			delete task;
		}
	}

	// Read the vertex buffer
	uint32_t	numVertexBuffers = stream.ReadUInt32();
	mesh->m_vertexBuffers.resize(numVertexBuffers);

	std::vector<std::vector<Vertex>> vertexBuffers(numVertexBuffers);

	for (uint32_t i = 0u; i < numVertexBuffers; ++i)
	{
		uint32_t	vbCount = stream.ReadUInt32();
		if (vbCount > 0u)
		{
			vertexBuffers[i].resize(vbCount);

			// Read in the vertex data
			stream.ReadStruct(&vertexBuffers[i][0], vbCount * sizeof(Vertex));

			GenBuffers*	task = new GenBuffers();
			task->ThreadId = std::this_thread::get_id().hash();
			task->Count = 1;

			graphics.AddGenBuffers(task);

			std::unique_lock<std::mutex>	lock(*task->Mutex);
			task->Condition.wait(lock);
			lock.unlock();

			mesh->m_vertexBuffers[i] = task->BuffersIds[0];
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_vertexBuffers[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vbCount * sizeof(Vertex), &vertexBuffers[i][0], GL_STATIC_DRAW);

			delete task;
		}
	}

	// Create the triangle array for each submesh
	for (SubMesh& subMesh : mesh->m_submeshes)
	{
		std::vector<USHORT>&	ib = indexBuffers[subMesh.IndexBufferIndex];
		std::vector<Vertex>&	vb = vertexBuffers[subMesh.VertexBufferIndex];

		for (uint32_t j = 0u; j < ib.size(); j += 3u)
		{
			Vertex&	v0 = vb[ib[j]];
			Vertex&	v1 = vb[ib[j + 1]];
			Vertex&	v2 = vb[ib[j + 2]];

			Triangle	tri;
			tri.Points[0].x = v0.x;
			tri.Points[0].y = v0.y;
			tri.Points[0].z = v0.z;

			tri.Points[1].x = v1.x;
			tri.Points[1].y = v1.y;
			tri.Points[1].z = v1.z;

			tri.Points[2].x = v2.x;
			tri.Points[2].y = v2.y;
			tri.Points[2].z = v2.z;

			mesh->m_triangles.push_back(tri);
		}
	}

	indexBuffers.clear();
	vertexBuffers.clear();

	// TODO: Read the skinning vertex buffer

	std::cout << "Waiting for " << innerTasks.size() << " tasks to finish" << std::endl;
	for (auto& task : innerTasks)
		task.wait();

	std::cout << "Mesh loaded successfully" << std::endl;
	return mesh;
}

void Mesh::StripPath(std::wstring& path)
{
	size_t	p = path.rfind(L"\\");
	if (p != std::wstring::npos)
		path = path.substr(p + 1);
}

void Mesh::Render(Graphics& graphics, glm::mat4x4 const& model)
{
	for (SubMesh& submesh : m_submeshes)
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
