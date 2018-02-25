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
	DeleteGO(m_skinModelRender);
}

bool CBackGround::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/background.cmo" );
	m_skinModelRender->SetShadowCasterFlag(false);
	m_skinModelRender->SetShadowReceiverFlag(true);

	//静的物理オブジェクトを作成。
	m_phyStaticObject.CreateMeshObject(m_skinModelRender, CVector3::Zero, CQuaternion::Identity);
	return true;
}

