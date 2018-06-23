#include "stdafx.h"
#include "Background.h"



void Background::OnDestroy()
{
	DeleteGO(m_skinModelRender);
}
bool Background::Start()
{
	//モデルレンダラーを作成。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/background.cmo");
	m_skinModelRender->SetShadowReceiverFlag(true);
	//静的物理オブジェクトを作成。
	m_physicsStaticObject.CreateMeshObject(m_skinModelRender, CVector3::Zero, CQuaternion::Identity);
	return true;
}

