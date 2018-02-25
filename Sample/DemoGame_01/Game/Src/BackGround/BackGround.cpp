/*!
 * @brief	背景
 */

#include "stdafx.h"
#include "BackGround/BackGround.h"

/*!
 * @brief	コンストラクタ。
 */
CBackGround::CBackGround()
{
}
/*!
 * @brief	デストラクタ。
 */
CBackGround::~CBackGround()
{
}

bool CBackGround::Start()
{
	m_skinModelData.Load(L"modelData/background.cmo");
	m_skinModel.Init(m_skinModelData);
	m_skinModel.SetShadowCasterFlag(false);
	m_skinModel.SetShadowReceiverFlag(true);

	//静的物理オブジェクトを作成。
	m_phyStaticObject.CreateMeshObject(m_skinModel, CVector3::Zero, CQuaternion::Identity);
	return true;
}

void CBackGround::Update()
{
	m_skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}

void CBackGround::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
