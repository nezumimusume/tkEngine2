#include "stdafx.h"
#include "Sky.h"


Sky::Sky()
{
}


Sky::~Sky()
{
}

bool Sky::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/sky.cmo");
	m_skinModelRender->SetScale({ 8000.0f, 8000.0f, 8000.0f });
	m_skinModelRender->SetForwardRenderFlag(true);
	m_skinModelRender->SetEmissionColor({ 0.3f, 0.3f, 0.3f });
	m_skyCube.CreateFromDDSTextureFromFile(L"modelData/skyCubeMap.dds");
	m_psSkyShader.Load("shader/model.fx", "PSMain_SkyCube", CShader::EnType::PS);
	m_skinModelRender->FindMaterial([&](CModelEffect* mat) {
		mat->SetRender3DModelPSShader(m_psSkyShader);
		mat->SetDiffuseTexture(m_skyCube.GetBody());
	});
	return true;
}
