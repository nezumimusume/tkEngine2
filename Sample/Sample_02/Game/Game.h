#pragma once
class Game : public IGameObject
{
public:
	//メンバ関数。
	
	void OnDestroy() override final;
	bool Start() override final;
	void Update() override final;
	
	//メンバ変数。
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。

};

