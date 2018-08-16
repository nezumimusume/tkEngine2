#include "stdafx.h"
#include "Background.h"


Background::Background()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/background/stage_00.cmo");
	//静的物理オブジェクトを初期化する。
	m_phyStaticObject.CreateMeshObject(m_skinModelRender, CVector3::Zero, CQuaternion::Identity, CVector3::One);
}


Background::~Background()
{
	DeleteGO(m_skinModelRender);
}
