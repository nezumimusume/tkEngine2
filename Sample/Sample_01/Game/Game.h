#pragma once
class Game : public IGameObject
{
public:
	//メンバ関数。
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(CRenderContext& rc);
	//メンバ変数。
	CSkinModel m_skinModel;			//スキンモデル。
	CSkinModelData m_skinModelData;	//スキンモデルデータ。
	CVector3 m_position = CVector3::Zero;			//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。

};

