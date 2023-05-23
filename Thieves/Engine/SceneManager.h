#pragma once

class Scene;

enum class CURRENT_SCENE
{
	NONE,
	LOGIN,
	LOBBY,
	ROOM,
	GAME,
	LOADING,
	// ...
	END
};

enum
{
	MAX_LAYER = 32
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);

	void ChangeToLoadedScene();

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
	bool GetCheckChangeScene() { return _checkChangeScene; }
	void SetCheckChangeScene(bool check) { _checkChangeScene = check; }
	CURRENT_SCENE GetCurrentScene() { return _currentScene; }

	const wstring& GetLoadText() { return _LoadText; }
	void SetLoadText(const wstring& wstr) { _LoadText = wstr; }

	//Vec3 GetPlayerPosition() { return _playerPosition; }
	//float GetPlayerPositionX() { return _playerPosition.x; }
	//float GetPlayerPositionY() { return _playerPosition.y; }
	//float GetPlayerPositionZ() { return _playerPosition.z; }
	//
	//void SetPlayerPosition(Vec3 playerPosition) { _playerPosition = playerPosition; }
	//void SetPlayerPositionX(float playerPosition) { _playerPosition.x = playerPosition; }
	//void SetPlayerPositionY(float playerPosition) { _playerPosition.y = playerPosition; }
	//void SetPlayerPositionZ(float playerPosition) { _playerPosition.z = playerPosition; }

	//Vec3 GetPlayerRotation() { return _playerRotation; }
	//void SetPlayerRotation(Vec3 playerRotation) { _playerRotation = playerRotation; }

	//void SetBuildPlayer(bool BuildPlayer) { _BuildPlayer = BuildPlayer; }
	void BuildPlayer();

private:
	void LoadGameScene();
	void LoadLoginScene();
	void LoadLobbyScene();
	void LoadRoomScene();
	shared_ptr<Scene> LoadLoadingScene();

private:
	shared_ptr<Scene> _activeScene;
	shared_ptr<Scene> _loadProgressScene;
	bool _checkChangeScene = false;
	wstring _changeSceneName = L"";
	CURRENT_SCENE _currentScene = CURRENT_SCENE::NONE;
	CURRENT_SCENE _currentLoadProgressScene = CURRENT_SCENE::NONE;

	wstring _LoadText = L"";

	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;


	//Vec3 _playerPosition;	// �÷��̾� ��ǥ
	//Vec3 _playerRotation;	// �÷��̾� ȸ����
	//bool _BuildPlayer = false;		// �÷��̾� ����
};

