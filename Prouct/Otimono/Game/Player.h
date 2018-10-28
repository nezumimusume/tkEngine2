#pragma once
/// <summary>
/// プレイヤークラス。
/// </summary>
class Player : public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Player();
	/// <summary>
	/// 更新。
	/// </summary>
	void Update() override;
	/// <summary>
	/// プレイヤーのワールド座標を取得。
	/// </summary>
	/// <returns>プレイヤーのワールド座標</returns>
	CVector3 GetPosition() const
	{
		return m_position;
	}
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダー。
	CVector3 m_position = CVector3::Zero;					//座標。
	CCharacterController m_charaCon;						//キャラコン。
	CVector3 m_moveSpeed = CVector3::Zero;					//移動速度。(単位:cm/sec)
};

