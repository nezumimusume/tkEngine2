/*!
 *@brief	ダミーノード
 */

#pragma once

#include "tkEngine/HUD/tkIHUDNode.h"

namespace tkEngine{
	class CHUDNode_Dummy : public IHUDNode{
	public:
		CHUDNode_Dummy(CBone* bone) : IHUDNode(bone) {}
		void Update() override
		{
		}
		void PostRender( CRenderContext& rc ) override
		{
		}
	};
}