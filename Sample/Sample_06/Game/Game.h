#pragma once
class Game : public IGameObject
{
public:
	/////////////////////////////////////////////
	//メンバ関数。
	/////////////////////////////////////////////
	Game();
	~Game();
	bool Start();
	void Update();
	/*!
	*@brief	Render関数が実行された後で呼ばれる描画処理
	*@details
	* ポストエフェクトの後で実行されます。HUDなどポストエフェクトの影響を受けたくない描画物はここでレンダリングしてください。
	* 2D空間で描画したい場合はRender関数ではなくPostRenderで描画を行うように。
	*/
	void PostRender(CRenderContext& rc);
	/////////////////////////////////////////////
	//メンバ変数とか。
	/////////////////////////////////////////////
	CSprite m_sprite;				//スプライト。
	CShaderResourceView m_texture;	//テクスチャ。
	CVector3	m_position = CVector3::Zero;		//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
};

