#pragma once
class Game : public IGameObject
{
public:
	//メンバ関数。
	Game();
	~Game();
	bool Start();
	void Update();
	//メンバ変数。
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position = CVector3::Zero;			//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。

};

