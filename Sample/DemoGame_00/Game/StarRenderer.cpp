#include "stdafx.h"
#include "StarRenderer.h"


StarRenderer::StarRenderer()
{
}


StarRenderer::~StarRenderer()
{
	DeleteGO(m_modelRender);
}
/*!
*@brief	開始時に呼ばれる処理。
*/
bool StarRenderer::Start()
{
	m_modelRender = NewGO<prefab::CSkinModelRender>(0);
	m_modelRender->Init(L"modelData/star.cmo", nullptr, 0, enFbxUpAxisZ, m_numStar);
	m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->FindMaterial([&](CModelEffect* material) {
		//マテリアルＩＤを設定する。
		material->SetMaterialID(enMaterialID_Star);
	});

	return true;
}


void StarRenderer::UpdateWorldMatrix(const CVector3& trans, const CQuaternion& rot, const CVector3& scale)
{
	m_modelRender->UpdateInstancingData(trans, rot, scale);
}