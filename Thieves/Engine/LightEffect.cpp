#include "pch.h"
#include "LightEffect.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Light.h"
#include "Input.h"

LightEffect::LightEffect()
{
}

LightEffect::~LightEffect()
{
}

void LightEffect::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::Q))
	{
		// this->light on/off
		/*if (_lightState == true)
		{
			this->GetGameObject()->GetLight()->SetLightState(false);
			_lightState = false;
		}
		else
		{
			this->GetGameObject()->GetLight()->SetLightState(true);
			_lightState = true;
		}*/

		for (auto& GameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
		{
			if (GameObject->GetName() == L"Dir_Light")
			{
				if (_lightStateDir == true)
				{
					GameObject->GetLight()->SetLightState(false);
					_lightStateDir = false;
				}
				else
				{
					GameObject->GetLight()->SetLightState(true);
					_lightStateDir = true;
				}
			}
		}
	}
	if (INPUT->GetButtonDown(KEY_TYPE::E))
	{
		for (auto& GameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
		{
			if (GameObject->GetName() == L"Spt_Light")
			{
				if (_lightStateSpt == true)
				{
					GameObject->GetLight()->SetLightState(false);
					_lightStateSpt = false;
				}
				else
				{
					GameObject->GetLight()->SetLightState(true);
					_lightStateSpt = true;
				}
			}
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::R))
	{
		for (auto& GameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
		{
			if (GameObject->GetName() == L"Pnt_Light")
			{
				if (_lightStatePnt == true)
				{
					GameObject->GetLight()->SetLightState(false);
					_lightStatePnt = false;
				}
				else
				{
					GameObject->GetLight()->SetLightState(true);
					_lightStatePnt = true;
				}
			}
		}
	}
}
