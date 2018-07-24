/*!
 *@brief	マップチップ
 */

#pragma once

#include "tkEngine/physics/tkPhysicsStaticObject.h"

namespace tkEngine{
	struct LevelObjectData;
	/*!
	 *@brief	マップチップ。
	 */
	class CMapChip : Noncopyable{
	public:
		CMapChip(LevelObjectData& objData);
		~CMapChip()
		{
			DeleteGO(m_modelRender);
		}
		/*!
		*@brief	スキンモデルレンダラーを取得。
		*/
		prefab::CSkinModelRender* GetSkinModelRender()
		{
			return m_modelRender;
		}
	private:
		prefab::CSkinModelRender* m_modelRender = nullptr;	//!<モデルレンダラー。
		CPhysicsStaticObject m_physicsStaticObject;			//!<静的物理オブジェクト。
	};
}
