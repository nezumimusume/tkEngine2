#pragma once


class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update() override;
	void Move();
	CVector3 m_moveSpeed = CVector3::Zero;
	CVector3 m_position = CVector3::Zero;
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CCharacterController m_charaCon;

};

