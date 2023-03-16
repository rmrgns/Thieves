#pragma once

class Scene;

enum class CURRENT_SCENE
{
	NONE,
	LOGIN,
	GAME,
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

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
	bool GetCheckChangeScene() { return _checkChangeScene; }
	void SetCheckChangeScene(bool check) { _checkChangeScene = check; }
	CURRENT_SCENE GetCurrentScene() { return _currentScene; }

private:
	shared_ptr<Scene> LoadTestScene();
	shared_ptr<Scene> LoadLoginScene();

private:
	shared_ptr<Scene> _activeScene;
	bool _checkChangeScene = false;
	wstring _changeSceneName = L"";
	CURRENT_SCENE _currentScene = CURRENT_SCENE::NONE;

	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

