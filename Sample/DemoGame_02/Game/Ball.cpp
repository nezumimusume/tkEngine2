#include "stdafx.h"
#include "Ball.h"


Ball::Ball()
{
	m_modelRender = NewGO<prefab::CSkinModelRender>(0);
	m_modelRender->Init(L"modelData/ball/ball.cmo");
	m_position.y = 50.0f;
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetShadowReceiverFlag(true);
	m_phyDynamicObject.CreateSphere(
		m_position,
		CQuaternion::Identity, 
		10.0f, 
		2.0f,
		{ 0.2f, 0.2f, 0.2f}
	);
}


Ball::~Ball()
{
	DeleteGO(m_modelRender);
}
void Ball::Update()
{
	//物理オブジェクトから座標と回転を受け取る、
	CVector3 pos;
	CQuaternion rot;
	m_phyDynamicObject.GetPositionAndRotation(pos, rot);
	m_modelRender->SetPosition(pos);
	m_modelRender->SetRotation(rot);
	if (Pad(0).IsTrigger(enButtonA)) {
		m_phyDynamicObject.AddForce({ 0.0f, 50000.0f, 0.0f });
	}
}
