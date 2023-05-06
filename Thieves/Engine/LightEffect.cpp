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
		if (_lightState == true)
		{
			this->GetGameObject()->GetLight()->SetLightState(false);
			_lightState = false;
		}
		else
		{
			this->GetGameObject()->GetLight()->SetLightState(true);
			_lightState = true;
		}
	}


	for (auto& GameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
	{
		if (GameObject->GetName() == L"Spt_Light")
		{

		}
	}
}
