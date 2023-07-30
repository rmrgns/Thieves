#pragma once
#include "Component.h"

class Material;
class Mesh;
class StructuredBuffer;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	int32	alive;
	int32	padding[3];
};

struct ComputeSharedInfo
{
	int32 addCount;
	int32 padding[3];
};

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

public:
	virtual void FinalUpdate();
	void Render();
	
	void UseParticle(bool flag) { _useParticle = flag; };
	void MakeParticle(wstring name, wstring path, int effect);
	void SetParticleOption(float minLifeTime, float maxLifeTime, float minSpeed, float maxSpeed);

	void ParticleLogic();

	int GetEffectNumber() { return _effectNumber; }

public:
	virtual void Load(const wstring& path) override { }
	virtual void Save(const wstring& path) override { }

private:
	shared_ptr<StructuredBuffer>	_particleBuffer;
	shared_ptr<StructuredBuffer>	_computeSharedBuffer;
	uint32							_maxParticle = 2000;

	shared_ptr<Material>		_computeMaterial;
	shared_ptr<Material>		_material;
	shared_ptr<Mesh>			_mesh;

	float				_createInterval = 0.0000005f;
	float				_accTime = 0.f;

	float				_minLifeTime = 2.0f;
	float				_maxLifeTime = 2.0f;
	float				_minSpeed = 50.f;
	float				_maxSpeed = 50.f;
	float				_startScale = 5.f;
	float				_endScale = 5.f;

	bool				_useParticle = false;
	int					_effectNumber = 0;
};
