/*!
 * @brief	プレイヤーレンダラー
 *@details
 * プレイヤーの描画関係の処理を行うクラスです。
 */

#include "stdafx.h"
#include "Player/Player.h"
#include "Player/PlayerRenderer.h"


void CPlayerRenderer::Awake()
{
	m_skinModelData.Load(L"modelData/Thethief_H.cmo");
	m_skinModel.Init(m_skinModelData);

	m_skinModel.SetShadowCasterFlag(true);
	m_skinModel.SetShadowReceiverFlag(true);

	//法線マップとスペキュラマップをロード。
	m_normalMap.CreateFromDDSTextureFromFile(L"modelData/Thethief_N.dds");
	m_specMap.CreateFromDDSTextureFromFile(L"modelData/Thethief_S.dds");
	m_wnormalMap.CreateFromDDSTextureFromFile(L"modelData/Thethief_wuqi_N.dds");
	m_wspecMap.CreateFromDDSTextureFromFile(L"modelData/Thethief_wuqi_S.dds");
	//法線マップとスペキュラマップをモデルに貼る。
	m_skinModel.FindMaterial([&](CModelEffect* material)
	{
		if (material->EqualMaterialName(L"bodyMat")) {
			//体のマテリアル。
			material->SetNormalMap(m_normalMap.GetBody());
			material->SetSpecularMap(m_specMap.GetBody());
		}else if (material->EqualMaterialName(L"weapon")) {
			//武器のマテリアル。
			material->SetNormalMap(m_wnormalMap.GetBody());
			material->SetSpecularMap(m_wspecMap.GetBody());
		}
	});
}

void CPlayerRenderer::Update()
{
	CQuaternion qRot;
	qRot.SetRotation(CVector3::AxisX, CMath::PI * 0.5f);
	qRot.Multiply(m_player->GetRotation(), qRot);
	m_skinModel.Update(m_player->GetPosition(), qRot, CVector3::One, CSkinModel::enFbxUpAxisY);
}

void CPlayerRenderer::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc);
}