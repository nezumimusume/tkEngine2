/*!
 *@brief	スプライトノード。
 */

#pragma once

#include "tkEngine/HUD/tkIHUDNode.h"

namespace tkEngine{
	/*!
	 *@brief	スプライトノード。
	 */
	class CHUDNode_Sprite : public IHUDNode{
	public:
		CHUDNode_Sprite(CBone* bone);
		void Update() override;
		void PostRender( CRenderContext& rc ) override;
	private:
		prefab::CSpriteRender m_spriteRender;		//スプライトレンダラー。
	};
}
