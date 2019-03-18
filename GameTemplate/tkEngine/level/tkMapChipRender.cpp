/*!
 *@brief	マップチップレンダラー
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/level/tkMapChipRender.h"
#include "tkEngine/level/tkLevel.h"

namespace tkEngine {
	CMapChipRender::CMapChipRender()
	{
	}
	CMapChipRender::~CMapChipRender()
	{
	}
	
	void CMapChipRender::OnDestroy()
	{
		DeleteGO(m_modelRender);
	}
	
	void CMapChipRender::Update()
	{
		if (m_renderObjDatas.size() > 1) {
			for (auto& objData : m_renderObjDatas) {
				m_modelRender->UpdateInstancingData(objData.position, objData.rotation, objData.scale);
			}
		}
	}

	void CMapChipRender::InitAfterAddAllRenderObjects()
	{
		//スキンモデルレンダラーを初期化する。
		m_modelRender = NewGO<prefab::CSkinModelRender>(0);
		//ファイルパスを作成。
		wchar_t filePath[256];
		swprintf_s(filePath, L"modelData/%s.cmo", m_renderObjDatas[0].name);
		if (m_renderObjDatas.size() > 1) {
			//インスタンシング
			m_modelRender->Init(filePath, nullptr, 0, enFbxUpAxisZ, m_renderObjDatas.size());
		}
		else {
			//のっとインスタンシング
			m_modelRender->Init(filePath);
			m_modelRender->SetPosition(m_renderObjDatas[0].position);
			m_modelRender->SetRotation(m_renderObjDatas[0].rotation);
			m_modelRender->SetScale(m_renderObjDatas[0].scale);
		}
		//影
		m_modelRender->SetShadowReceiverFlag(true);
		m_modelRender->SetShadowCasterFlag(true);
	}
}
