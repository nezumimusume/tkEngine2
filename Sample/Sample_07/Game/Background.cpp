#include "stdafx.h"
#include "Background.h"


void Background::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
bool Background::Start()
{
	//モデルをロード。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/background.cmo");
	
	//静的物理オブジェクトを作成。
	m_phyStaticObject.CreateMesh(CVector3::Zero, CQuaternion::Identity, CVector3::One, m_skinModelRender);
	return true;
}

