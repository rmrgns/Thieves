#include "pch.h"
#include "PlayerParticle.h"
#include "Input.h"
#include "Timer.h"
#include "ParticleSystem.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Scene.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"

PlayerParticle::PlayerParticle()
{
}

PlayerParticle::~PlayerParticle()
{
}

void PlayerParticle::LateUpdate()
{
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	Vec3 pos(0.0f, 0.0f, 0.0f);
	Vec3 rotation(0.0f, 0.0f, 0.0f);
	if (myID != -1)
	{
		pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
		rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
	}

	if (this->GetGameObject()->GetParticleSystem()->GetEffectNumber() == 1)	// player attack
	{
		
		pos.x += GetTransform()->GetLook().x * 100.f;
		pos.z += GetTransform()->GetLook().z * 100.f;
		pos.y += 100.f;

		// Attack
		if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
		{
			if (_attackState == 0)
			{
				_attackState = 1;
				_currentPos = pos;
			}
		}

		PlayerAttack();
		GetTransform()->SetLocalPosition(_currentPos);
		GetTransform()->SetLocalRotation(rotation);
	}
	else if (this->GetGameObject()->GetParticleSystem()->GetEffectNumber() == 2)	// get item
	{
		pos.y += 75.f;
		GetTransform()->SetLocalPosition(pos);

	}
	else if (this->GetGameObject()->GetParticleSystem()->GetEffectNumber() == 3)	// gun
	{
		pos.x += GetTransform()->GetLook().x * 100.f;
		pos.z += GetTransform()->GetLook().z * 100.f;
		pos.y += 100.f;

		// Attack
		if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
		{
			if (_attackState == 0)
			{
				_attackState = 1;
				_currentPos = pos;
			}
		}

		PlayerAttack();
		GetTransform()->SetLocalPosition(_currentPos);
		GetTransform()->SetLocalRotation(rotation);
	}
	else if (this->GetGameObject()->GetParticleSystem()->GetEffectNumber() == 4)	// trap
	{
		pos.y += 75.f;
		GetTransform()->SetLocalPosition(pos);
	}
}

void PlayerParticle::PlayerAttack()
{
	// Attack
	if (_attackState == 1)
	{
		// ontrigger
		for (auto& gameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
		{
			if (gameObject->GetName() == L"Particle")
			{
				gameObject->GetParticleSystem()->UseParticle(true);
			}
			if (gameObject->GetName() == L"ParticleGun")
			{
				gameObject->GetParticleSystem()->UseParticle(true);
			}
			if (gameObject->GetName() == L"ParticleTrap")
			{
				gameObject->GetParticleSystem()->UseParticle(true);
			}
		}

		_attackState = 2;
	}
	else if (_attackState == 2)
	{
		_attackCount += DELTA_TIME;
		if (_attackCount > 2.5f)
		{
			// offtrigger
			for (auto& gameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
			{
				if (gameObject->GetName() == L"Particle")
				{
					gameObject->GetParticleSystem()->UseParticle(false);
				}
				if (gameObject->GetName() == L"ParticleGun")
				{
					gameObject->GetParticleSystem()->UseParticle(false);
				}
				if (gameObject->GetName() == L"ParticleTrap")
				{
					gameObject->GetParticleSystem()->UseParticle(false);
				}
			}

			_attackCount = 0.f;
			_attackState = 0;
		}
	}
	else
		return;
}
