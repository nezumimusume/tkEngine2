/*!
 *@brief	マップチップのレンダラー。
 */

#pragma once

#include "tkEngine/physics/tkPhysicsStaticObject.h"

namespace tkEngine{
	struct LevelObjectData;
	/*!
	 *@brief	マップチップレンダラー。
	 */
	class CMapChipRender : Noncopyable{
	public:
		CMapChipRender();
		~CMapChipRender();
	private:
		prefab::CSkinModelRender* m_modelRender = nullptr;	//!<モデルレンダラー。
	};
}
