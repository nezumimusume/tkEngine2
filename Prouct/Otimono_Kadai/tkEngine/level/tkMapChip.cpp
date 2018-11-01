/*!
 *@brief	マップチップ
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/level/tkMapChip.h"
#include "tkEngine/level/tkLevel.h"

namespace tkEngine{
	CMapChip::CMapChip(LevelObjectData& objData)
	{
		char objName[256];
		wcstombs(objName, objData.name, 256);
		//レンダラーを作成。
		m_modelRender = NewGO<prefab::CSkinModelRender>(0, objName);
		//ファイルパスを作成。
		wchar_t filePath[256];
		swprintf_s(filePath, L"modelData/%s.cmo", objData.name);
		m_modelRender->Init(filePath);
		m_modelRender->SetPosition(objData.position);
		m_modelRender->SetRotation(objData.rotation);
		m_modelRender->SetScale(objData.scale);
		//MapChip用のモディファイアを作らなあかんなぁ・・・
		m_modelRender->SetShadowReceiverFlag(true);
		
		m_physicsStaticObject.CreateMesh( 
			objData.position, 
			objData.rotation,
			objData.scale,
			m_modelRender
		);
	}
}
