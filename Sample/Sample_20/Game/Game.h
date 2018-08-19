#pragma once
class Game : public IGameObject
{
public:
	//////////////////////////////////////
	// メンバ関数。
	//////////////////////////////////////
	Game();
	~Game();
	bool Start();
	void Update();
	//////////////////////////////////////
	// メンバ変数。
	//////////////////////////////////////
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CQuaternion m_rotation = CQuaternion::Identity;
	int m_rotateMode = 0;	//0は自動回転、1はスティックで回転。
};

