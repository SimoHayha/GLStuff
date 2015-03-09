#include "RawMesh.h"

RawMeshData& RawMesh::Data()
{
	return m_data;
}

RawMesh* RawMesh::Read(MeshStream& stream, std::wstring const& shaderPathLocation, std::wstring const& texturePathLocation)
{
	RawMesh*	mesh = new RawMesh;

	stream.ReadString(&mesh->m_data.Name);

	// Read material count
	uint32_t	numMaterials = stream.ReadUInt32();
	mesh->m_data.Materials.resize(numMaterials);

	// Load each material
	for (uint32_t i = 0u; i < numMaterials; ++i)
	{
		Material&	material = mesh->m_data.Materials[i];

		// Read the material name
		stream.ReadString(&material.Name);

		// Read the ambient and diffuse properties of material
		stream.ReadStruct(&material.Ambient, sizeof(material.Ambient));
		stream.ReadStruct(&material.Diffuse, sizeof(material.Diffuse));
		stream.ReadStruct(&material.Specular, sizeof(material.Specular));
		stream.ReadStruct(&material.SpecularPower);
		stream.ReadStruct(&material.Emissive, sizeof(material.Emissive));
		stream.ReadStruct(&material.UVTransform);

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
				StripPath(sourceFile);

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
	mesh->m_data.SubMeshes.resize(numSubmeshes);
	for (uint32_t i = 0u; i < numSubmeshes; ++i)
	{
		SubMesh&	submesh = mesh->m_data.SubMeshes[i];
		submesh.MaterialIndex = stream.ReadUInt32();
		submesh.IndexBufferIndex = stream.ReadUInt32();
		submesh.VertexBufferIndex = stream.ReadUInt32();
		submesh.StartIndex = stream.ReadUInt32();
		submesh.PrimCount = stream.ReadUInt32();
	}

	// Read the index buffers
	uint32_t	numIndexBuffers = stream.ReadUInt32();
	mesh->m_data.Indices.resize(numIndexBuffers);

	for (uint32_t i = 0u; i < numIndexBuffers; ++i)
	{
		uint32_t	ibCount = stream.ReadUInt32();
		if (ibCount > 0u)
		{
			mesh->m_data.Indices[i].resize(ibCount);

			// Read in the index data
			for (USHORT& component : mesh->m_data.Indices[i])
				component = stream.ReadUInt16();
		}
	}

	// Read the vertex buffer
	uint32_t	numVertexBuffers = stream.ReadUInt32();
	mesh->m_data.Vertices.resize(numVertexBuffers);

	for (uint32_t i = 0u; i < numVertexBuffers; ++i)
	{
		uint32_t	vbCount = stream.ReadUInt32();
		if (vbCount > 0u)
		{
			mesh->m_data.Vertices[i].resize(vbCount);

			// Read in the vertex data
			stream.ReadStruct(&mesh->m_data.Vertices[i][0], vbCount * sizeof(Vertex));
		}
	}

	// Create the triangle array for each submesh
	for (SubMesh& subMesh : mesh->m_data.SubMeshes)
	{
		std::vector<USHORT>&	ib = mesh->m_data.Indices[subMesh.IndexBufferIndex];
		std::vector<Vertex>&	vb = mesh->m_data.Vertices[subMesh.VertexBufferIndex];

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

			mesh->m_data.Triangles.push_back(tri);
		}
	}

	return mesh;
}

void RawMesh::StripPath(std::wstring& path)
{
	size_t	p = path.rfind(L"\\");
	if (p != std::wstring::npos)
		path = path.substr(p + 1);
}
