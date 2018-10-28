#pragma once

/// <summary>
/// 地面クラス。
/// </summary>
class Ground : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Ground();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Ground();
	/// <summary>
	/// 開始処理。
	/// </summary>
	/// <returns></returns>
	bool Start() override;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダー。
	CPhysicsStaticObject m_phyStaticObject;			//物理静的オブジェクト。
	CShaderResourceView m_specTexSRV;				//スペキュラマップのシェーダーリソースビュー。
	CVector3 m_position = CVector3::Zero;			//座標。
	CQuaternion m_rotation = CQuaternion::Identity;	//回転。
	CVector3 m_scale = CVector3::One;				//拡大率。
};

