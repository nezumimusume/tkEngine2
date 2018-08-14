#include "stdafx.h"
#include "star.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "Player.h"
#include "StarRenderer.h"
#include "Game.h"

prefab::CDirectionLight* Star::m_starLight = nullptr;

Star::Star()
{
}
Star::~Star()
{
}
bool Star::Start()
{
	if (m_starLight == nullptr) {
		m_starLight = NewGO<prefab::CDirectionLight>(0);
		m_starLight->SetDirection({ 0.0f, 0.0f, -1.0f });
		m_starLight->SetLightingMaterialIDGroup(1 << enMaterialID_Star);
		m_starLight->SetColor({ 30.0f, 30.0f, 30.0f, 1.0f });
	}
	m_player = FindGO<Player>("Player");
	return true;
}
void Star::Update()
{
	if (!m_isGet) {
		CVector3 dist = m_player->GetPosition() - m_position;
		if (dist.Length() < 130.0f) {
			FindGO<Game>("Game")->AddGetStarCount();
			//コインゲット。
			prefab::CSoundSource* s = NewGO<prefab::CSoundSource>(0);
			s->Init(L"sound/coinGet.wav");
			s->Play(false);
			m_jumpSpeed = 500.0f;
			m_isGet = true;
			m_jumStartPosY = m_position.y;
		}
	}
	else {
		
		m_position.y += m_jumpSpeed * GameTime().GetFrameDeltaTime();
		m_jumpSpeed -= 980.0f * GameTime().GetFrameDeltaTime();
		CQuaternion qAddRot;
		qAddRot.SetRotation(CVector3::AxisY, 0.2f);
		m_rotation.Multiply(qAddRot);
		
		
		if (m_jumStartPosY > m_position.y) {
			DeleteGO(this);
		}
	}
	m_renderer->UpdateWorldMatrix(m_position, m_rotation, { 20.0f, 20.0f, 20.0f });
}

