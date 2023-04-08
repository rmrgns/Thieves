#include "pch.h"
#include "Physics.h"
#include "Transform.h"
#include "Timer.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

Physics::Physics() : Component(COMPONENT_TYPE::PHYSICS)
{

}

Physics::~Physics()
{
}

void Physics::FinalUpdate()
{
	if(_useGravity == true)
		UseGravity();
	
}

void Physics::UseGravity()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	pos -= GetTransform()->GetUp() * _gravity * DELTA_TIME;
	GetTransform()->SetLocalPosition(pos);
}
