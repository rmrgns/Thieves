#include "pch.h"
#include "ParticleSystem.h"
#include "StructuredBuffer.h"
#include "Mesh.h"
#include "Resources.h"
#include "Transform.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
ParticleSystem::ParticleSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{
	_particleBuffer = make_shared<StructuredBuffer>();
	_particleBuffer->Init(sizeof(ParticleInfo), _maxParticle);

	_computeSharedBuffer = make_shared<StructuredBuffer>();
	_computeSharedBuffer->Init(sizeof(ComputeSharedInfo), 1);

	_mesh = GET_SINGLE(Resources)->LoadPointMesh();
	
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::FinalUpdate()
{
	if (_useParticle == true)
	{
		_computeMaterial->SetInt(3, 1);
	}
	else
	{
		_computeMaterial->SetInt(3, 0);
	}
	ParticleLogic();
	
}

void ParticleSystem::Render()
{
	if (_useParticle == true)
	{
		GetTransform()->PushData();

		_particleBuffer->PushGraphicsData(SRV_REGISTER::t9);
		_material->SetFloat(0, _startScale);
		_material->SetFloat(1, _endScale);
		_material->PushGraphicsData();

		_mesh->Render(_maxParticle);
	}
}

void ParticleSystem::MakeParticle(wstring name, wstring path, int effect)
{
	wstring pt_name = name;
	wstring pt_path = path;
	_effectNumber = effect;

	_material = GET_SINGLE(Resources)->Get<Material>(name);

	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeParticle");
}

void ParticleSystem::SetParticleOption(float minLifeTime, float maxLifeTime, float minSpeed, float maxSpeed)
{
	_minLifeTime = minLifeTime;
	_maxLifeTime = maxLifeTime;
	_minSpeed = minSpeed;
	_maxSpeed = maxSpeed;
}

void ParticleSystem::ParticleLogic()
{
	_accTime += DELTA_TIME;

	int32 add = 0;
	if (_createInterval < _accTime)
	{
		_accTime = _accTime - _createInterval;
		add = 1;
		Vec3 look = GetTransform()->GetLook();

		_computeMaterial->SetVec4(1, Vec4(look.x, look.y, look.z, 0));
	}

	_particleBuffer->PushComputeUAVData(UAV_REGISTER::u0);
	_computeSharedBuffer->PushComputeUAVData(UAV_REGISTER::u1);

	_computeMaterial->SetInt(0, _maxParticle);
	_computeMaterial->SetInt(1, add);
	_computeMaterial->SetInt(2, _effectNumber);
	//_computeMaterial->SetInt(2, 100);

	_computeMaterial->SetVec2(1, Vec2(DELTA_TIME, _accTime));
	_computeMaterial->SetVec4(0, Vec4(_minLifeTime, _maxLifeTime, _minSpeed, _maxSpeed));
	
	_computeMaterial->Dispatch(1, 1, 1);
}
