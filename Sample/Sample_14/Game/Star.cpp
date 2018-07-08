#include "stdafx.h"
#include "Star.h"


Star::Star()
{
}


Star::~Star()
{
	DeleteGO(m_modelRender);
}

bool Star::Start()
{
	m_modelRender = NewGO<prefab::CSkinModelRender>(0);
	m_modelRender->Init(L"modelData/star.cmo");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetScale(m_scale);
	m_modelRender->SetRotation(m_rotation);
	return true;
}
void Star::Update()
{
	m_position.y += m_moveSpeed;
	if (m_position.y > 200.0f) {
		m_position.y = 200.0f;
		m_moveSpeed = -1.0f;
	}
	if (m_position.y < 100.0f) {
		m_position.y = 100.0f;
		m_moveSpeed = 1.0f;
	}
	m_modelRender->SetPosition(m_position);
}
