#include "pch.h"
#include "LightEffect.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Light.h"
#include "Input.h"
#include "InGameScene.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"

LightEffect::LightEffect()
{
}

LightEffect::~LightEffect()
{
}

void LightEffect::LateUpdate()
{
	if (_lightState == true)
	{
		auto iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());

		if (iScene->GetPhaseChanged())
		{
			// 2 ÆäÀÌÁî

			if (this->GetGameObject()->GetName() == L"Dir_Light")
			{

				//this->GetGameObject()->GetLight()->SetLightState(false);

				this->GetGameObject()->GetLight()->SetDiffuse(Vec3(0.8f, 0.8f, 0.8f));
				this->GetGameObject()->GetLight()->SetAmbient(Vec3(0.5f, 0.5f, 0.5f));
				this->GetGameObject()->GetLight()->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
				_lightState = false;

			}
			if (this->GetGameObject()->GetName() == L"DiaRoom_Pnt_Light")
			{

				//this->GetGameObject()->GetLight()->SetLightState(false);
				this->GetGameObject()->GetLight()->SetDiffuse(Vec3(0.f, 0.f, 0.f));
				this->GetGameObject()->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.f));
				this->GetGameObject()->GetLight()->SetSpecular(Vec3(0.f, 0.f, 0.f));
				_lightState = false;

			}
		}
	}
	
	//if (this->GetGameObject()->GetName() == L"Dir_Light")
	//{
	//	if (_lightState == true)
	//	{
	//		//this->GetGameObject()->GetLight()->SetLightState(false);

	//		this->GetGameObject()->GetLight()->SetDiffuse(Vec3(0.8f, 0.8f, 0.8f));
	//		this->GetGameObject()->GetLight()->SetAmbient(Vec3(0.5f, 0.5f, 0.5f));
	//		this->GetGameObject()->GetLight()->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
	//		_lightState = false;
	//	}
	//	else
	//	{
	//		//this->GetGameObject()->GetLight()->SetLightState(true);
	//		this->GetGameObject()->GetLight()->SetDiffuse(Vec3(0.f, 0.f, 0.f));
	//		this->GetGameObject()->GetLight()->SetAmbient(Vec3(0.2f, 0.2f, 0.2f));
	//		this->GetGameObject()->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));
	//		_lightState = true;
	//	}
	//}



	//if (this->GetGameObject()->GetName() == L"DiaRoom_Pnt_Light")
	//{
	//	if (_lightState == true)
	//	{
	//		//this->GetGameObject()->GetLight()->SetLightState(false);
	//		this->GetGameObject()->GetLight()->SetDiffuse(Vec3(0.f, 0.f, 0.f));
	//		this->GetGameObject()->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.f));
	//		this->GetGameObject()->GetLight()->SetSpecular(Vec3(0.f, 0.f, 0.f));
	//		_lightState = false;
	//	}
	//	else
	//	{
	//		//this->GetGameObject()->GetLight()->SetLightState(true);
	//		_lightState = true;
	//	}
	//}


}
