#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "BlockingQueue.hpp"
#include "Graphics.h"
#include "MeshSlotMap.h"

class MeshLoader
{
public:
	MeshLoader() = default;
	~MeshLoader() = default;

	MeshLoader(MeshLoader const&) = delete;
	MeshLoader(MeshLoader&&) = delete;
	MeshLoader&	operator=(MeshLoader const&) = delete;
	MeshLoader&	operator=(MeshLoader&&) = delete;

	bool	Initialize(MeshSlotMap* meshPool, size_t threadCount = 1u);
	void	Deinitialize();
	void	LoadMesh(std::wstring const& meshFilename, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation);
	void	CollectMeshes();

private:
	struct MeshTask
	{
		MeshTask() = default;
		~MeshTask() = default;

		std::wstring			MeshFilename;
		std::wstring			ShaderPathLocation;
		std::wstring			TexturePathLocation;
		MeshSlotMap*			SlotMap;
		std::vector<RawMesh*>	RawMeshes;
	};

	class MeshLoaderTask
	{
	public:
		MeshLoaderTask() = default;
		~MeshLoaderTask() = default;

		void	Execute(BlockingQueue<MeshTask>* tasks, BlockingQueue<MeshTask>* done);

	public:
		bool	Alive;
	};

	std::vector<MeshLoaderTask>	m_threads;
	BlockingQueue<MeshTask>		m_tasks;
	BlockingQueue<MeshTask>		m_dones;
	MeshSlotMap*				m_meshPool;
};