#include "pch.h"
#include "Resources.h"
#include "Engine.h"
#include "MeshData.h"

void Resources::Init()
{
	CreateDefaultShader();
	CreateDefaultMaterial();
}

shared_ptr<Mesh> Resources::LoadPointMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Point");
	if (findMesh)
		return findMesh;

	vector<Vertex> vec(1);
	vec[0] = Vertex(Vec3(0, 0, 0), Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(1);
	idx[0] = 0;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Point", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadRectangleMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Rectangle");
	if (findMesh)
		return findMesh;

	float w2 = 0.5f;
	float h2 = 0.5f;

	vector<Vertex> vec(4);

	// ¾Õ¸é
	vec[0] = Vertex(Vec3(-w2, -h2, 0), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, 0), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, 0), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, 0), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(6);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Rectangle", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Cube");
	if (findMesh)
		return findMesh;

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<Vertex> vec(24);

	// ¾Õ¸é
	vec[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// µÞ¸é
	vec[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// À­¸é
	vec[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// ¾Æ·§¸é
	vec[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// ¿ÞÂÊ¸é
	vec[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// ¿À¸¥ÂÊ¸é
	vec[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	vector<uint32> idx(36);

	// ¾Õ¸é
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// µÞ¸é
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// À­¸é
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// ¾Æ·§¸é
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// ¿ÞÂÊ¸é
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// ¿À¸¥ÂÊ¸é
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Cube", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadSphereMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Sphere");
	if (findMesh)
		return findMesh;

	float radius = 0.5f; // ±¸ÀÇ ¹ÝÁö¸§
	uint32 stackCount = 20; // °¡·Î ºÐÇÒ
	uint32 sliceCount = 20; // ¼¼·Î ºÐÇÒ

	vector<Vertex> vec;

	Vertex v;

	// ºÏ±Ø
	v.pos = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 1.0f);
	vec.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// °í¸®¸¶´Ù µ¹¸é¼­ Á¤Á¡À» °è»êÇÑ´Ù (ºÏ±Ø/³²±Ø ´ÜÀÏÁ¡Àº °í¸®°¡ X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// °í¸®¿¡ À§Ä¡ÇÑ Á¤Á¡
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.pos;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vec.push_back(v);
		}
	}

	// ³²±Ø
	v.pos = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 0.0f);
	vec.push_back(v);

	vector<uint32> idx(36);

	// ºÏ±Ø ÀÎµ¦½º
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// ¸öÅë ÀÎµ¦½º
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y) * ringVertexCount + (x));
			idx.push_back(1 + (y) * ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y) * ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// ³²±Ø ÀÎµ¦½º
	uint32 bottomIndex = static_cast<uint32>(vec.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Sphere", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadTerrainMesh(int32 sizeX, int32 sizeZ)
{
	vector<Vertex> vec;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			Vertex vtx;
			vtx.pos = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			vtx.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			vtx.normal = Vec3(0.f, 1.f, 0.f);
			vtx.tangent = Vec3(1.f, 0.f, 0.f);

			vec.push_back(vtx);
		}
	}

	vector<uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Terrain");
	if (findMesh)
	{
		findMesh->Create(vec, idx);
		return findMesh;
	}

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Create(vec, idx);
	Add(L"Terrain", mesh);
	return mesh;
}

shared_ptr<Texture> Resources::CreateTexture(const wstring& name, DXGI_FORMAT format, uint32 width, uint32 height,
	const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
	D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->Create(format, width, height, heapProperty, heapFlags, resFlags, clearColor);
	Add(name, texture);

	return texture;
}

shared_ptr<Texture> Resources::CreateTextureFromResource(const wstring& name, ComPtr<ID3D12Resource> tex2D)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->CreateFromResource(tex2D);
	Add(name, texture);

	return texture;
}

shared_ptr<MeshData> Resources::LoadFBX(const wstring& path)
{
	wstring key = path;

	shared_ptr<MeshData> meshData = Get<MeshData>(key);
	if (meshData)
	{
		//return meshData;
	}
	meshData = MeshData::LoadFromFBX(path);
	meshData->SetName(key);
	Add(key, meshData);

	return meshData;
}

void Resources::CreateDefaultShader()
{
	// Skybox
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_EQUAL
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\skybox.fx", info);
		Add<Shader>(L"Skybox", shader);
	}

	// Deferred (Deferred)
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\deferred.fx", info);
		Add<Shader>(L"Deferred", shader);
	}

	// Deferred (AlphaBlending)
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::ALPHA_BLEND,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\deferred.fx", info);
		Add<Shader>(L"DeferredAlpha", shader);
	}

	// Forward (Forward)
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,	
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\forward.fx", info);
		Add<Shader>(L"Forward", shader);
	}

	// UI
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::ALPHA_BLEND,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\forward.fx", info);
		Add<Shader>(L"UI", shader);
	}

	// Texture (Forward)
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE
		};

		ShaderArg arg =
		{
			"VS_Tex",
			"",
			"",
			"",
			"PS_Tex"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\forward.fx", info, arg);
		Add<Shader>(L"Texture", shader);
	}

	// DirLight
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		ShaderArg arg =
		{
			"VS_DirLight",
			"",
			"",
			"",
			"PS_DirLight"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\lighting.fx", info, arg);
		Add<Shader>(L"DirLight", shader);
	}

	// PointLight
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		ShaderArg arg =
		{
			"VS_PointLight",
			"",
			"",
			"",
			"PS_PointLight"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\lighting.fx", info, arg);
		Add<Shader>(L"PointLight", shader);
	}

	// Final
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
		};

		ShaderArg arg =
		{
			"VS_Final",
			"",
			"",
			"",
			"PS_Final"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\lighting.fx", info, arg);
		Add<Shader>(L"Final", shader);
	}

	// Compute Shader
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"..\\Resources\\Shader\\compute.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeShader", shader);
	}

	// Particle
	{
		ShaderInfo info =
		{
			SHADER_TYPE::PARTICLE,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS_NO_WRITE,
			BLEND_TYPE::ALPHA_BLEND,
			D3D_PRIMITIVE_TOPOLOGY_POINTLIST
		};

		ShaderArg arg =
		{
			"VS_Main",
			"",
			"",
			"GS_Main",
			"PS_Main"
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\particle.fx", info, arg);
		Add<Shader>(L"Particle", shader);
	}

	// UI
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::ALPHA_BLEND,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\forward.fx", info);
		Add<Shader>(L"UI", shader);
	}

	// ComputeParticle
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"..\\Resources\\Shader\\particle.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeParticle", shader);
	}

	// Shadow
	{
		ShaderInfo info =
		{
			SHADER_TYPE::SHADOW,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\shadow.fx", info);
		Add<Shader>(L"Shadow", shader);
	}

	// Tessellation
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::WIREFRAME,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
		};

		ShaderArg arg =
		{
			"VS_Main",
			"HS_Main",
			"DS_Main",
			"",
			"PS_Main",
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\tessellation.fx", info, arg);
		Add<Shader>(L"Tessellation", shader);
	}

	// Terrain
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST
		};

		ShaderArg arg =
		{
			"VS_Main",
			"HS_Main",
			"DS_Main",
			"",
			"PS_Main",
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"..\\Resources\\Shader\\terrain.fx", info, arg);
		Add<Shader>(L"Terrain", shader);
	}

	// ComputeAnimation
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"..\\Resources\\Shader\\animation.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeAnimation", shader);
	}
}

void Resources::CreateDefaultMaterial()
{
	// Skybox
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Skybox", material);
	}

	// DirLight
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"DirLight");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, GET_SINGLE(Resources)->Get<Texture>(L"PositionTarget"));
		material->SetTexture(1, GET_SINGLE(Resources)->Get<Texture>(L"NormalTarget"));
		Add<Material>(L"DirLight", material);
	}

	// PointLight
	{
		const WindowInfo& window = GEngine->GetWindow();
		Vec2 resolution = { static_cast<float>(window.width), static_cast<float>(window.height) };

		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"PointLight");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, GET_SINGLE(Resources)->Get<Texture>(L"PositionTarget"));
		material->SetTexture(1, GET_SINGLE(Resources)->Get<Texture>(L"NormalTarget"));
		material->SetVec2(0, resolution);
		Add<Material>(L"PointLight", material);
	}

	// Final
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Final");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, GET_SINGLE(Resources)->Get<Texture>(L"DiffuseTarget"));
		material->SetTexture(1, GET_SINGLE(Resources)->Get<Texture>(L"DiffuseLightTarget"));
		material->SetTexture(2, GET_SINGLE(Resources)->Get<Texture>(L"SpecularLightTarget"));
		Add<Material>(L"Final", material);
	}

	// Compute Shader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"ComputeShader", material);
	}

	// Particle
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Particle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Particle", material);
	}

	// ComputeParticle
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeParticle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);

		Add<Material>(L"ComputeParticle", material);
	}

	// GameObject
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
		shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		material->SetTexture(1, texture2);
		Add<Material>(L"GameObject", material);
	}

	// LoginScreen
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Login", L"..\\Resources\\Texture\\Thieves_UI\\Title.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"LoginScreen", material);
	}

	// Temp LoginScreen Enter
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"LoginButton", L"..\\Resources\\Texture\\Thieves_UI\\Button_Base.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"LoginScreenButton", material);
	}

	// Back Button Image
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"BackButton", L"..\\Resources\\Texture\\Thieves_UI\\BackButton.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"BackButton", material);
	}

	// LobbyScreen
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Robby", L"..\\Resources\\Texture\\Thieves_UI\\RoomBase.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"LobbyScreen", material);
	}

	// Temp Robby Room Button
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"LobbyRoomButton", L"..\\Resources\\Texture\\Thieves_UI\\Button_Base.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"LobbyRoomButton", material);
	}

	// Room Button
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"RoomButton", L"..\\Resources\\Texture\\Thieves_UI\\RoomButton.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"RoomButton", material);
	}

	// RoomScreen
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"InRoom", L"..\\Resources\\Texture\\Thieves_UI\\InRoom.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"InRoom", material);
	}

	// LoadingScreen
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Loading", L"..\\Resources\\Texture\\Thieves_UI\\Load.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"LoadScreen", material);
	}

	// GameStartButton
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"GameStartBtn", L"..\\Resources\\Texture\\Thieves_UI\\Button_GameStart_Base.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"GameStartBtn", material);
	}

	// GameStartButtonClicked
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"GameStartBtnClk", L"..\\Resources\\Texture\\Thieves_UI\\Button_GameStart_Clicked.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"GameStartBtnClk", material);
	}

	// GameStartButtonUp
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"GameStartBtnUp", L"..\\Resources\\Texture\\Thieves_UI\\Button_GameStart_MouseUp.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"GameStartBtnUp", material);
	}

	// ReadyButton
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ReadyBtn", L"..\\Resources\\Texture\\Thieves_UI\\ReadyButton.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ReadyBtn", material);
	}

	// ReadyButtonClicked
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ReadyBtnClk", L"..\\Resources\\Texture\\Thieves_UI\\ReadyButton_Clicked.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ReadyBtnClk", material);
	}

	// ReadyButtonUp
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ReadyBtnUp", L"..\\Resources\\Texture\\Thieves_UI\\ReadyButton_MouseUp.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ReadyBtnUp", material);
	}


	// ItemBox UI default
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ItemBox", L"..\\Resources\\Texture\\Thieves_UI\\ItemBox.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ItemBox", material);
	}

	// ItemBox UI Gun
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ItemBox_Gun", L"..\\Resources\\Texture\\Thieves_UI\\Item_Gun.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ItemBox_Gun", material);
	}

	// ItemBox UI Trap
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ItemBox_Trap", L"..\\Resources\\Texture\\Thieves_UI\\Item_Trap.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ItemBox_Trap", material);
	}

	// EnterButtonUp
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"EnterBtn", L"..\\Resources\\Texture\\Thieves_UI\\EnterRoomButton.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"EnterBtn", material);

	}

	// ThiefIcon
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Forward");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"ThiefIcon", L"..\\Resources\\Texture\\thief_icon.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"ThiefIcon", material);
	}

	// Shadow
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Shadow");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Shadow", material);
	}

	// Tessellation
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Tessellation");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Tessellation", material);
	}

	// Terrain
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Terrain");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Terrain", L"..\\Resources\\Texture\\Terrain\\terrain.png");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		Add<Material>(L"Terrain", material);
	}

	// ComputeAnimation
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeAnimation");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);

		Add<Material>(L"ComputeAnimation", material);
	}
}
