#include <iostream>

#include "MeshStream.hpp"
#include "RawMesh.h"
#include "MeshLoader.h"

#include <gl\glew.h>
#include <GLFW/glfw3.h>

void MeshLoader::MeshLoaderTask::Execute(BlockingQueue<MeshTask>* tasks, BlockingQueue<MeshTask>* dones)
{
	while (Alive)
	{
		MeshTask	task;
		if (tasks->tryPop(task, std::chrono::seconds(1)))
		{
			std::wcout << "Thread have job (" << task.MeshFilename << ")" << std::endl;

			std::ifstream	file;
			file.open(task.MeshFilename, std::ios::binary);
			if (!file.is_open())
				return;

			file.seekg(0, std::ios::end);
			std::streamsize	size = file.tellg();
			file.seekg(0, std::ios::beg);

			std::wcout << "Reading " << task.MeshFilename << std::endl;
			MeshStream	stream(file);

			uint32_t	remainingMeshesToRead = stream.ReadUInt32();

			for (uint32_t i = 0u; i < remainingMeshesToRead; ++i)
			{
				RawMesh*	rawMesh = RawMesh::Read(stream, task.ShaderPathLocation, task.TexturePathLocation);
				if (rawMesh)
					task.RawMeshes.push_back(rawMesh);
			}
			file.close();

			dones->push(task);
		}
		else
		{
			std::cout << "Thread timeout" << std::endl;
		}
	}
}

bool MeshLoader::Initialize(MeshSlotMap* meshPool, size_t threadCount /*= 1u*/)
{
	m_threads.resize(threadCount);

	for (auto& it : m_threads)
	{
		it.Alive = true;
		std::thread([&it, this] { it.Execute(&m_tasks, &m_dones); }).detach();
	}

	m_meshPool = meshPool;

	return true;
}

void MeshLoader::Deinitialize()
{
	for (auto& t : m_threads)
	{
		t.Alive = false;
		// TODO: avoid zombies, currently playing Russian Roulette on exit
		//t.Thread.join();
	}
}

void MeshLoader::LoadMesh(std::wstring const& meshFilename, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation)
{
	MeshTask	task;
	task.MeshFilename = meshFilename;
	task.ShaderPathLocation = shaderPathLocation;
	task.TexturePathLocation = texturePathLocation;
	task.SlotMap = m_meshPool;

	m_tasks.push(task);
}

void MeshLoader::CollectMeshes()
{
	MeshTask	task;
	
	if (m_dones.size() > 0)
	{
		while (m_dones.tryPop(task, std::chrono::seconds::zero()))
		{
			std::cout << "One mesh to collect" << std::endl;

			for (auto& it : task.RawMeshes)
			{
				Mesh*	mesh = m_meshPool->CreateObject();
				assert(mesh != nullptr);

				mesh->Initialize(it);
			}
		}
	}
}
