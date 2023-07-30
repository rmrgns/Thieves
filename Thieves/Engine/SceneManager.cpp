#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LobbyScene.h"
#include "RoomScene.h"
#include "InGameScene.h"
#include "Sound.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Physics.h"

// Script
#include "LoginScript.h"
#include "LobbyScript.h"
#include "RoomScript.h"
#include "PlayerInput.h"
#include "PlayerCamera.h"
#include "PlayerParticle.h"
#include "LightEffect.h"
#include "UsingGun.h"
#include "PlayerShadow.h"
#include "ItemScript.h"
#include "DiamondScript.h"
//#include "ItemUI.h"

#include "Resources.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "SphereCollider.h"
#include "MeshData.h"
#include "Animator.h"
#include "NetworkSystem.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"

#include <thread>




void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;
	
	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}







// TEMP
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}







void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : ���� Scene ����
	// TODO : ���Ͽ��� Scene ���� �ε�
	if (_activeScene != nullptr)
	{
		_activeScene.reset();
	}

	LoadSceneLock.lock();
	_activeScene = LoadLoadingScene();
	LoadSceneLock.unlock();
	_currentScene = CURRENT_SCENE::LOADING;

	

	if (sceneName == L"LoginScene")
	{
		std::thread th{ &SceneManager::LoadLoginScene, this};
		th.detach();
	}
	else if (sceneName == L"GameScene")
	{
		std::thread th{ &SceneManager::LoadGameScene, this };
		th.detach();
	}
	else if (sceneName == L"LobbyScene")
	{
		std::thread th{ &SceneManager::LoadLobbyScene, this };
		th.detach();
	}
	else if (sceneName == L"RoomScene")
	{
		std::thread th{ &SceneManager::LoadRoomScene, this };
		th.detach();
	}
	else
	{
		return;
	}

	_activeScene->Awake();
	_activeScene->Start();
}



void SceneManager::ChangeToLoadedScene()
{

	if (_activeScene != nullptr)
	{
		_activeScene.reset();
	}
	
	_activeScene = _loadProgressScene;
	_currentScene = _currentLoadProgressScene;

	_activeScene->Awake();
	_activeScene->Start();
}




void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// ���� ������ ����
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}




uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}




shared_ptr<GameObject> SceneManager::Pick(int32 screenX, int32 screenY)
{
	shared_ptr<Camera> camera = GetActiveScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix projectionMatrix = camera->GetProjectionMatrix();

	// ViewSpace���� Picking ����
	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMatrix();
	Matrix viewMatrixInv = viewMatrix.Invert();

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	float minDistance = FLT_MAX;
	shared_ptr<GameObject> picked;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace������ Ray ����
		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// WorldSpace������ Ray ����
		rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
		rayDir.Normalize();

		// WorldSpace���� ����
		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false)
			continue;

		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
			GET_SINGLE(SceneManager)->LoadScene(L"GameScene");
		}
	}

	return picked;
}





void SceneManager::BuildPlayer()
{
	// �ٸ� �÷��̾� ��ǥ
	Vec3 otherplayer = { 100.f,100.f,100.f };

	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Thief.fbx");

	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
	
	for (auto& gameObject : gameObjects)
	{
		gameObject->SetName(L"Thief");
		gameObject->SetCheckFrustum(false);
		gameObject->GetTransform()->SetLocalPosition(otherplayer);
		gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
		_activeScene->AddGameObject(gameObject);
	}
}





void SceneManager::LoadGameScene()
{
	_LoadText = L"Load Start"; // 1
	Network::GetInst()->SendLoadProgressPacket((char)0);
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
	
	_LoadText = L"Load Compute Shader"; // 2

	Network::GetInst()->SendLoadProgressPacket((char)100/21);

#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV �� Texture ����
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		// ������ �׷� (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion

	_LoadText = L"Load Main Camera"; // 3
	Network::GetInst()->SendLoadProgressPacket((char)200 / 21);
	shared_ptr<InGameScene> scene = make_shared<InGameScene>();

	_loadProgressScene = scene;
	_currentLoadProgressScene = CURRENT_SCENE::GAME;

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
		//camera->AddComponent(make_shared<TestCameraScript>());
		camera->AddComponent(make_shared<PlayerCamera>());
		camera->AddComponent(make_shared<LoginScript>());
		camera->GetCamera()->SetFar(30000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI�� �� ����

		scene->AddGameObject(camera);
	}	
#pragma endregion

	_LoadText = L"Load UI Camera"; // 4
	Network::GetInst()->SendLoadProgressPacket((char)300 / 21);

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // �� ����
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion

	_LoadText = L"Load SkyBox"; // 5
	Network::GetInst()->SendLoadProgressPacket((char)400 / 21);
#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion
	

//#pragma region Terrain
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->AddComponent(make_shared<Transform>());
//		obj->AddComponent(make_shared<Terrain>());
//		obj->AddComponent(make_shared<MeshRenderer>());
//
//		obj->GetTransform()->SetLocalScale(Vec3(50.f, 250.f, 50.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(-100.f, -200.f, 300.f));
//		obj->SetStatic(true);
//		obj->GetTerrain()->Init(64, 64);
//		obj->SetCheckFrustum(false);
//
//		scene->AddGameObject(obj);
//	}
//#pragma endregion
	
	_LoadText = L"Load UI"; // 6
	Network::GetInst()->SendLoadProgressPacket((char)500 / 21);
	

#pragma region ItemBox UI
	
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ItemBox");
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(650.f, -350.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ItemBox");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}

	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ItemBox_Gun");
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(650.f, -350.f, 501.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ItemBox_Gun");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		//obj->AddComponent(make_shared<ItemUI>());
		scene->AddGameObject(obj);
	}

	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ItemBox_Trap");
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(650.f, -350.f, 501.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ItemBox_Trap");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		//obj->AddComponent(make_shared<ItemUI>());
		scene->AddGameObject(obj);
	}
#pragma endregion

	while (true) {
		if (scene->IsGetAllObjInfo()) break;
		Sleep(0);
	}

	_LoadText = L"Load Player FBX Data"; // 7
	Network::GetInst()->SendLoadProgressPacket((char)600 / 21);

#pragma region FBX
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Thief.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
		
		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Thief");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->AddComponent(make_shared<PlayerInput>());
			gameObject->AddComponent(make_shared<NetworkSystem>());
			
			// 자신의 NetworkID에 해당하는 데이터를 NetworkSystem에서 사용하도록 함.
			int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();

			if (Network::GetInst()->GetNetworkObjMap().end() != Network::GetInst()->GetNetworkObjMap().find(myID)) {
				gameObject->GetNetworkSystem()->SetNetworkId(myID);
				gameObject->GetNetworkSystem()->SetNetworkingType(NetworkType::PLAYER);
			}
			gameObject->SetStatic(false);
			int32 index = 2;
			gameObject->GetAnimator()->Play(index);
			scene->AddGameObject(gameObject);
		}
	}
#pragma endregion

	_LoadText = L"Load Other Player FBX Data";// 8
	Network::GetInst()->SendLoadProgressPacket((char)700 / 21);

#pragma region OtherPlayers
	{

		std::vector<int> occupied_id;

		for (int i = 0; i < 7; ++i)
		{
			_LoadText = L"Load Other Player FBX Data ";
			_LoadText.append(to_wstring(i));
			_LoadText.append(L" / 7");
			Network::GetInst()->SendLoadProgressPacket((char)(800 + i * 100) / 21);
			 // 9 10 11 12 13 14 15 
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Thief.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				
				gameObject->SetName(L"Thief");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, -300.f, 0.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->AddComponent(make_shared<NetworkSystem>());
				gameObject->SetStatic(false);
				// 이미 존재하는 오브젝트들에 대해 NetworkSystem을 설정한다.
				// 새로 생성되는 오브젝트들의 경우에는 ObjInfo가 왔을 때에 설정해 주면 된다.
				for (auto& p : Network::GetInst()->GetNetworkObjMap()) {
					if (p.second->GetType() == NW_OBJ_TYPE::OT_PLAYER) {
						if ((occupied_id.empty() || std::find(occupied_id.begin(), occupied_id.end(), p.first) == occupied_id.end()))
						{
							occupied_id.emplace_back(p.first);
							gameObject->GetNetworkSystem()->SetNetworkId(p.first);
							gameObject->GetNetworkSystem()->SetNetworkingType(NetworkType::OTHER_PLAYER);
							break;
						}
					}
				}

				int32 index = 2;
				gameObject->GetAnimator()->Play(index);
				scene->AddGameObject(gameObject);
			}
		}
	}
#pragma endregion

#pragma region PoliceFBX
	{
		for (int i = 0; i < 8; i++)
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\PoliceMan.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Police");
				gameObject->SetCheckFrustum(false);
				gameObject->SetStatic(true);
				gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, -4000.f, 0.f));
				//gameObject->GetTransform()->SetLocalRotation(Vec3(2.f * i, 2.f, 0.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				scene->AddGameObject(gameObject);

			}
		}
	}
#pragma endregion

	_LoadText = L"Load Map"; // 16
	Network::GetInst()->SendLoadProgressPacket((char)1500 / 21);

#pragma region Map
	{

		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Map");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.1415f, 0.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(40.f, 100.f, 40.f));
			//gameObject->AddComponent(make_shared<TestObjectMove>());
			//gameObject->AddComponent(make_shared<PlayerInput>());
			gameObject->SetStatic(false);
			scene->AddGameObject(gameObject);
			//gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion

//#pragma region Object
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->SetName(L"OBJ");
//		obj->AddComponent(make_shared<Transform>());
//		//obj->AddComponent(make_shared<SphereCollider>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 1.f, 1.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(0, 50.f, 0.f));
//		obj->SetStatic(true);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
//			meshRenderer->SetMaterial(material->Clone());
//		}
//		//dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetRadius(0.5f);
//		//dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion

//#pragma region Object
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->SetName(L"BulletLine");
//		obj->AddComponent(make_shared<Transform>());
//
//		obj->GetTransform()->SetLocalScale(Vec3(2.f, 2.f, 1000.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(-100.f, -100.f, -100.f));
//		
//		obj->SetStatic(true);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
//			meshRenderer->SetMaterial(material->Clone());
//		}
//		obj->AddComponent(make_shared<UsingGun>());
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion

#pragma region Diamond
	{

		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Diamond.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Diamond");
			//gameObject->SetCheckFrustum(true);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.1415f, 0.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			//gameObject->AddComponent(make_shared<TestObjectMove>());
			//gameObject->AddComponent(make_shared<PlayerInput>());
			gameObject->SetStatic(true);
			gameObject->AddComponent(make_shared<DiamondScript>());
			scene->AddGameObject(gameObject);
			//gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion

#pragma region Safe
	{

		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\ClosedSafe.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Safe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 1.57f));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			//gameObject->AddComponent(make_shared<TestObjectMove>());
			//gameObject->AddComponent(make_shared<PlayerInput>());
			gameObject->SetStatic(true);
			scene->AddGameObject(gameObject);
			//gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion

#pragma region OpenSafe
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\OpenedSafe.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"OpenedSafe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, -4000.f, 0.f));
			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 1.57f));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			//gameObject->AddComponent(make_shared<TestObjectMove>());
			//gameObject->AddComponent(make_shared<PlayerInput>());
			gameObject->SetStatic(true);
			scene->AddGameObject(gameObject);
			//gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion

#pragma region EscapeZone
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\EscapeZone.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"EscapeZone");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(500.f, 100.f, 500.f));
			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 1.57f));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			//gameObject->AddComponent(make_shared<TestObjectMove>());
			//gameObject->AddComponent(make_shared<PlayerInput>());
			gameObject->SetStatic(true);
			scene->AddGameObject(gameObject);
			//gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion

#pragma region Items
	{
		for (auto& item : Network::GetInst()->GetItemObjMap())
		{
			if (item.second->GetItemType() == ITEM_NUM_GUN)
			{
				shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Gun.fbx");

				vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

				for (auto& gameObject : gameObjects)
				{
					gameObject->SetName(L"Gun");
					gameObject->SetCheckFrustum(false);
					gameObject->GetTransform()->SetLocalPosition(item.second->GetPosition());
					gameObject->GetTransform()->SetLocalRotation(Vec3(1.f, 1.57f, 1.57f));
					gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
					gameObject->AddComponent(make_shared<ItemScript>(item.second->GetID()));
					gameObject->SetStatic(false);
					scene->AddGameObject(gameObject);
				}
			}
			else if (item.second->GetItemType() == ITEM_NUM_TRAP)
			{
				shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Trap.fbx");

				vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

				for (auto& gameObject : gameObjects)
				{
					gameObject->SetName(L"Trap");
					gameObject->SetCheckFrustum(false);
					gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
					//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 1.57f));
					gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
					gameObject->AddComponent(make_shared<ItemScript>(item.second->GetID()));
					gameObject->SetStatic(true);
					scene->AddGameObject(gameObject);
				}
			}
			else
			{
				continue;
			}
		}
	}
#pragma endregion

//#pragma region Trap
//	{
//
//		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Trap.fbx");
//
//		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
//
//		for (auto& gameObject : gameObjects)
//		{
//			gameObject->SetName(L"Trap");
//			gameObject->SetCheckFrustum(false);
//			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
//			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 1.57f));
//			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
//			//gameObject->AddComponent(make_shared<TestObjectMove>());
//			//gameObject->AddComponent(make_shared<PlayerInput>());
//			gameObject->SetStatic(true);
//			scene->AddGameObject(gameObject);
//			//gameObject->AddComponent(make_shared<TestDragon>());
//		}
//	}
//#pragma endregion
//
//#pragma region Gun
//	{
//
//		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Gun.fbx");
//
//		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();
//
//		for (auto& gameObject : gameObjects)
//		{
//			gameObject->SetName(L"Gun");
//			gameObject->SetCheckFrustum(false);
//			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
//			//gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 1.57f));
//			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
//			//gameObject->AddComponent(make_shared<TestObjectMove>());
//			//gameObject->AddComponent(make_shared<PlayerInput>());
//			gameObject->SetStatic(false);
//			scene->AddGameObject(gameObject);
//			//gameObject->AddComponent(make_shared<TestDragon>());
//		}
//	}
//#pragma endregion

	_LoadText = L"Load Directinal Light"; // 17
	Network::GetInst()->SendLoadProgressPacket((char)1500 / 21);

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"Dir_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, -500));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);

		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.2f, 0.2f, 0.2f));
		light->GetLight()->SetSpecular(Vec3(0.6f, 0.6f, 0.6f));
		light->AddComponent(make_shared<LightEffect>());

		scene->AddGameObject(light);
	}
#pragma endregion
	
	_LoadText = L"Load Point Light"; // 18
	Network::GetInst()->SendLoadProgressPacket((char)1700 / 21);

#pragma region Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"Pnt_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 470.f, 0.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
		light->GetLight()->SetSpecular(Vec3(0.15f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(500.f);
		light->AddComponent(make_shared<LightEffect>());

		//light->GetLight()->SetLightState(false);
		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region DiaRoom Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"DiaRoom_Pnt_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(2000.f, 350.f, 0.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
		light->GetLight()->SetSpecular(Vec3(0.15f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(500.f);
		light->AddComponent(make_shared<LightEffect>());

		//light->GetLight()->SetLightState(false);
		scene->AddGameObject(light);
	}

	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"DiaRoom_Pnt_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(-2000.f, 350.f, 0.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
		light->GetLight()->SetSpecular(Vec3(0.15f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(500.f);
		light->AddComponent(make_shared<LightEffect>());

		//light->GetLight()->SetLightState(false);
		scene->AddGameObject(light);
	}

	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"DiaRoom_Pnt_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 350.f, 2000.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
		light->GetLight()->SetSpecular(Vec3(0.15f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(500.f);
		light->AddComponent(make_shared<LightEffect>());

		//light->GetLight()->SetLightState(false);
		scene->AddGameObject(light);
	}

	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"DiaRoom_Pnt_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 350.f, -2000.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.0f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.0f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.0f));
		light->GetLight()->SetLightRange(500.f);
		light->AddComponent(make_shared<LightEffect>());

		//light->GetLight()->SetLightState(false);
		scene->AddGameObject(light);
	}
#pragma endregion
	_LoadText = L"Load Spot Light"; // 19
	Network::GetInst()->SendLoadProgressPacket((char)1800 / 21);

#pragma region DiaRoom Spot Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"DiaRoom_Spt_Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 500.f, -600.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.5f));
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(1000.f);
		light->GetLight()->SetLightAngle(3.14f / 2);
		light->AddComponent(make_shared<LightEffect>());

		//light->GetLight()->SetLightState(false);
		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Shadow Making Directional Light
	//for(int i=0;i<9;i++)
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetName(L"Dir_Light2");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 250));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->AddComponent(make_shared<PlayerShadow>());
		light->GetLight()->SetDiffuse(Vec3(0.01f, 0.01f, 0.01f));
		light->GetLight()->SetAmbient(Vec3(0.01f, 0.01f, 0.01f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.0f, 0.0f));


		scene->AddGameObject(light);
	}
#pragma endregion
//#pragma region Directional Light
//	for (int i = 0; i < 8; i++)
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->SetName(L"Dir_Light2");
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(2000, 1000, 2000));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(0.f, -1, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
//		light->AddComponent(make_shared<PlayerShadow>());
//		light->GetLight()->SetDiffuse(Vec3(0.01f, 0.01f, 0.01f));
//		light->GetLight()->SetAmbient(Vec3(0.01f, 0.01f, 0.01f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
	_LoadText = L"Load Particle"; // 20
	Network::GetInst()->SendLoadProgressPacket((char)1900 / 21);

#pragma region ParticleSystem
	{
		shared_ptr<GameObject> particle = make_shared<GameObject>();
		particle->SetName(L"Particle");
		particle->AddComponent(make_shared<Transform>());
		particle->AddComponent(make_shared<ParticleSystem>());
		particle->SetCheckFrustum(false);
		particle->AddComponent(make_shared<PlayerParticle>());
		//particle->GetParticleSystem()->UseParticle(true);
		particle->GetParticleSystem()->MakeParticle(L"ptc", L"..\\Resources\\Texture\\Particle\\bubble.png", 3);
		particle->GetTransform()->SetLocalPosition(Vec3(-100.f, -100.f, -100.f));
		scene->AddGameObject(particle);
	}
#pragma endregion

#pragma region ParticleSystem_GetItem
	{
		shared_ptr<GameObject> particle = make_shared<GameObject>();
		particle->SetName(L"ParticleGetItem");
		particle->AddComponent(make_shared<Transform>());
		particle->AddComponent(make_shared<ParticleSystem>());
		particle->SetCheckFrustum(false);
		particle->AddComponent(make_shared<PlayerParticle>());
		//particle->GetParticleSystem()->UseParticle(true);
		particle->GetParticleSystem()->MakeParticle(L"ptcGetItem", L"..\\Resources\\Texture\\Particle\\bubble.png", 2);
		particle->GetParticleSystem()->SetParticleOption(3.0f, 3.0f, 500.f, 500.f);
		particle->GetTransform()->SetLocalPosition(Vec3(-100.f, -100.f, -100.f));
		scene->AddGameObject(particle);
	}
#pragma endregion

//#pragma region ParticleSystem
//	{
//		shared_ptr<GameObject> particle = make_shared<GameObject>();
//		particle->SetName(L"Particle");
//		particle->AddComponent(make_shared<Transform>());
//		particle->AddComponent(make_shared<ParticleSystem>());
//		particle->SetCheckFrustum(false);
//		particle->AddComponent(make_shared<PlayerParticle>());
//		particle->GetParticleSystem()->UseParticle(true);
//		particle->GetParticleSystem()->MakeParticle(L"blood", L"..\\Resources\\Texture\\Particle\\bubble.png", 1);
//		particle->GetTransform()->SetLocalPosition(Vec3(100.f, 100.f, 100.f));
//		scene->AddGameObject(particle);
//	}
//#pragma endregion
	

	scene->SetSceneLoaded(true);



	_LoadText = L"Wait Other Players..."; // 21
	Network::GetInst()->SendLoadEndPacket();

	// 만약 나중에 로딩이 모든 사람들의 로딩이 끝난 뒤 보여야 하는 경우라면 
	// 이 부분에서 설정해 주어야 한다
	
	while (true)
	{
		if (scene->IsAllPlayerLoaded()) break;
		Sleep(0);
	}
	
	LoadSceneLock.lock();
	ChangeToLoadedScene();
	LoadSceneLock.unlock();
}





void SceneManager::LoadLoginScene()
{
	_LoadText = L"Load Start";
#pragma region LayerMask
	SetLayerName(0, L"Default");

#pragma endregion

#pragma region Sound
	GEngine->GetSound()->PlaySound(0, 0);
	GEngine->GetSound()->SetVolume(0, 0.1f);

#pragma endregion

	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
	shared_ptr<Scene> scene = make_shared<Scene>();

	_loadProgressScene = scene;
	_currentLoadProgressScene = CURRENT_SCENE::LOGIN;

	_LoadText = L"Load Main Camera";
#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -1086.f));
		//camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		//uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		//camera->GetCamera()->SetCullingMaskAll(); // �� ����
		//camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion

	_LoadText = L"Load Login Image";
#pragma region LoginScreen
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);
		
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"LOGINSCREEN");
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<LoginScript>());
		obj->GetTransform()->SetLocalScale(Vec3(width, height, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 1.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"LoginScreen");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
		
	}
#pragma endregion
	
	_LoadText = L"Load Login Image";
#pragma region LoginScreenImage
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"LOGINSCREENBUTTON");
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 4.f, height / 10.f, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * 0.475, (height / 2) * -1 * 0.45, 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"LoginScreenButton");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	/*_LoadText = L"Load Icon";
#pragma region ThiefIcon
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"THIEFICON");
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
		obj->GetTransform()->SetLocalPosition(Vec3(23.f, 15.f, 50.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ThiefIcon");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion*/

	_LoadText = L"Load Directional Light";
#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 0, 1000));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, 0.f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion


//#pragma region ParticleSystem
//	{
//		shared_ptr<GameObject> particle = make_shared<GameObject>();
//		particle->AddComponent(make_shared<Transform>());
//		particle->AddComponent(make_shared<ParticleSystem>());
//		particle->SetCheckFrustum(false);
//		particle->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 100.f));
//		scene->AddGameObject(particle);
//	}
//#pragma endregion

	_LoadText = L"Load End.";
	scene->SetSceneLoaded(true);



	LoadSceneLock.lock();
	ChangeToLoadedScene();
	LoadSceneLock.unlock();
}




void SceneManager::LoadLobbyScene()
{
	_LoadText = L"Load Start";

#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
	shared_ptr<LobbyScene> scene = make_shared<LobbyScene>();
	_loadProgressScene = scene;
	_currentLoadProgressScene = CURRENT_SCENE::LOBBY;

//#pragma region Camera
//	{
//		shared_ptr<GameObject> camera = make_shared<GameObject>();
//		camera->SetName(L"Main_Camera");
//		camera->AddComponent(make_shared<Transform>());
//		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
//		//camera->AddComponent(make_shared<TestCameraScript>());
//		//camera->AddComponent(make_shared<PlayerCamera>());
//		camera->GetCamera()->SetFar(10000.f);
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -1000.f));
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"Default");
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex,true); // UI�� �� ����
//
//		scene->AddGameObject(camera);
//	}
//#pragma endregion

	_LoadText = L"Load Main Camera";

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -1086.f));
		//camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		//uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		//camera->GetCamera()->SetCullingMaskAll(); // �� ����
		//camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region script
	{
		shared_ptr<GameObject> script = make_shared<GameObject>();
		script->SetName(L"Script");
		script->AddComponent(make_shared<Transform>());
		script->AddComponent(make_shared<LobbyScript>());
		script->AddComponent(make_shared<LoginScript>());
		script->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));

		scene->AddGameObject(script);
	}
#pragma endregion


	

	_LoadText = L"Load Robby Image";
#pragma region LobbyScreen
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"LobbyScreen");
		obj->AddComponent(make_shared<Transform>());
		//obj->AddComponent(make_shared<LoginScript>());
		obj->GetTransform()->SetLocalScale(Vec3(width, height, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 1.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"LobbyScreen");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion


	_LoadText = L"Load Lobby Room Buttion";
#pragma region LobbyRoomButton
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				float width = static_cast<float>(GEngine->GetWindow().width);
				float height = static_cast<float>(GEngine->GetWindow().height);

				shared_ptr<GameObject> obj = make_shared<GameObject>();
				wstring text = L"LOBBYSCREEN";
				text.append(to_wstring(i * 2 + j + 1));
				obj->SetName(text);
				obj->AddComponent(make_shared<Transform>());
				obj->GetTransform()->SetLocalScale(Vec3(width / 3.f, height / 7.5f, 1.f));
				if (j == 0)
					obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * -1.f * 0.25f, (height / 2) * (0.6f - (i * 0.4f)), 0.f));
				else
					obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * 0.55f, (height / 2) * (0.6f - (i * 0.4f)), 0.f));
				//obj->SetStatic(false);

				shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
				{
					shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
					meshRenderer->SetMesh(ScreenMesh);
				}
				{
					shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"RoomButton");
					meshRenderer->SetMaterial(material->Clone());
				}
				obj->AddComponent(meshRenderer);
				scene->AddGameObject(obj);
			}
		}
	}
#pragma endregion

	_LoadText = L"Load Lobby Back Button";
#pragma region LobbyBackButton
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"LOBBYBACKBUTTON");
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 7.f, height / 17.5f, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * -1 * 0.825, (height / 2) * 0.2, 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"BackButton");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	_LoadText = L"Load Make Room Button";
#pragma region MakeRoomButton
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"MKROOMBUTTON");
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 7.f, height / 17.5f, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * -1 * 0.825, (height / 2) * -1.f * 0.05f, 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"EnterBtn");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	_LoadText = L"Load Directional Light";
#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 0, 1000));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, 0.f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion

	_LoadText = L"Load Room Datas.";
	scene->SetSceneLoaded(true);



	scene->GetRoomsDataFromNetwork();

	_LoadText = L"Load End.";
	LoadSceneLock.lock();
	ChangeToLoadedScene();
	LoadSceneLock.unlock();
}



void SceneManager::LoadRoomScene()
{
	_LoadText = L"Load Start";
#pragma region LayerMask
	SetLayerName(0, L"Default");

#pragma endregion
#pragma region Sound
	GEngine->GetSound()->StopSound(0);

#pragma endregion
	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
	shared_ptr<RoomScene> scene = make_shared<RoomScene>();
	
	_loadProgressScene = scene;
	_currentLoadProgressScene = CURRENT_SCENE::ROOM;

	auto rScene = std::static_pointer_cast<RoomScene>(_loadProgressScene);
	rScene->SetRoomId(GET_SINGLE(SceneManager)->GetRoomNum());
	_LoadText = L"Load Main Camera";
#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -1086.f));
		//camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		//uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		//camera->GetCamera()->SetCullingMaskAll(); // �� ����
		//camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region script
	{
		shared_ptr<GameObject> script = make_shared<GameObject>();
		script->SetName(L"Script");
		script->AddComponent(make_shared<Transform>());
		script->AddComponent(make_shared<RoomScript>());
		script->AddComponent(make_shared<LoginScript>());
		script->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));

		scene->AddGameObject(script);
	}
#pragma endregion

	_LoadText = L"Load Room Image";
#pragma region RoomScreen
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ROOMSCREEN");
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(width, height, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 1.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"InRoom");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	_LoadText = L"Room Back Button Image";
#pragma region RoomBackButtonImage
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ROOMBACKBUTTON");
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<LoginScript>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 7.f, height / 17.5f, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * -1 * 0.825, (height / 2) * 0.2, 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"BackButton");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	_LoadText = L"Load Ready Button";
#pragma region ReadyButtonImage
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ROOMREADYBUTTON");
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 7.f, height / 17.5f, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * -1 * 0.825, (height / 2) * -1.f * 0.05f, 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ReadyBtn");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	_LoadText = L"Load GameStart Button";
#pragma region GameStartButtonImage
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"ROOMGAMESTARTBUTTON");
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<LoginScript>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 7.f, height / 17.5f, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * -1 * 0.825, (height / 2) * -1.f * 0.3f, 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameStartBtn");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	/*for (int i = 0; i < 8; i++)
	{

		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		wstring text = L"LobbyScreen";
		text.append(to_wstring(i + 1));
		obj->SetName(text);
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(width / 3.f, height / 12.f, 1.f));

			obj->GetTransform()->SetLocalPosition(Vec3((width / 2) * 0.475f, (height / 2) * (0.7f - (i * 0.2f)), 0.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"LobbyRoomButton");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}*/
	


	_LoadText = L"Load Directional Light";
#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 0, 1000));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, 0.f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion

	_LoadText = L"Load Room Data.";

	scene->SetSceneLoaded(true);
	scene->GetRoomDataFromNetwork();

	_LoadText = L"Load End.";

	LoadSceneLock.lock();
	ChangeToLoadedScene();
	LoadSceneLock.unlock();
}




shared_ptr<Scene> SceneManager::LoadLoadingScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");

#pragma endregion

	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -1086.f));
		//camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		//uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		//camera->GetCamera()->SetCullingMaskAll(); // �� ����
		//camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion



	_LoadText = L"Load Loading Image";
#pragma region RoomScreen
	{
		float width = static_cast<float>(GEngine->GetWindow().width);
		float height = static_cast<float>(GEngine->GetWindow().height);

		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetName(L"LOADSCREEN");
		obj->AddComponent(make_shared<Transform>());
		
		obj->GetTransform()->SetLocalScale(Vec3(width, height, 1.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 1.f));
		obj->SetStatic(false);

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> ScreenMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(ScreenMesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"LoadScreen");
			meshRenderer->SetMaterial(material->Clone());
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);

	}
#pragma endregion

	_LoadText = L"Load Directional Light";
#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0, 0, 1000));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, 0.f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		scene->AddGameObject(light);
	}
#pragma endregion

	return scene;
}
