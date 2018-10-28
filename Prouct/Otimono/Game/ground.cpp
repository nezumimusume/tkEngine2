#include "stdafx.h"
#include "ground.h"


Ground::Ground()
{
}


Ground::~Ground()
{
	DeleteGO(m_skinModelRender);
}

bool Ground::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/bg/bg.cmo");
	//スペキュラマップをロード。
	m_specTexSRV.CreateFromDDSTextureFromFile(L"modelData/bg/bgSpec.dds");
	m_skinModelRender->FindMaterial([this](CModelEffect* mat) {
		mat->SetSpecularMap(m_specTexSRV.GetBody());
	});
	m_skinModelRender->SetPRS(m_position, m_rotation, m_scale);
	m_skinModelRender->SetShadowReceiverFlag(true);
	m_phyStaticObject.CreateMesh(m_position, m_rotation, m_scale, m_skinModelRender);
	return true;
}
