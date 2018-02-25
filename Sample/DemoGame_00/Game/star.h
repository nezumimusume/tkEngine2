#pragma once
class Player;
class StarRenderer;

class Star : public IGameObject
{
public:
	Star();
	~Star();
	bool Start() override;
	void Update() override;
	/*!
	 *@brief	初期化。
	 */
	void Init(StarRenderer& starRenderer)
	{
		m_renderer = &starRenderer;
	}
	/*!
	 *@brief	座標を設定。
	 */
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	/*!
	 *@brief	座標を取得。
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
private:
	StarRenderer*	m_renderer = nullptr;					//!<レンダラー。
	CVector3		m_position = CVector3::Zero;			//!<座標。
	CQuaternion		m_rotation = CQuaternion::Identity;
	static prefab::CDirectionLight* m_starLight;
	Player*			m_player = nullptr;
	bool			m_isGet = false;
	float			m_jumpSpeed = 0.0f;
	float			m_jumStartPosY = 0.0f;
};

