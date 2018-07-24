/*!
 *@brief	フォントノード。
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/HUD/tkHUD.h"
#include "tkEngine/HUD/node/tkHUDNode_Font.h"

namespace tkEngine{
	CHUDNode_Font::CHUDNode_Font(CBone* bone) :
		IHUDNode(bone)
	{
		m_font = std::make_unique<CFont>();
		m_text = &bone->GetName()[HUD_FONT_NODE_NAME_PREFIX_LENGTH];
		CVector3 pos;
		CVector3 scale;
		CQuaternion rot;
		bone->CalcWorldTRS(pos, rot, scale);
		auto t = pos.y;
		pos.y = pos.z;
		pos.z = -t;
		m_position.x = pos.x;
		m_position.y = pos.y;
	}
	void CHUDNode_Font::Update()
	{
	}
	void CHUDNode_Font::PostRender( CRenderContext& rc )
	{
		m_font->Begin(rc);
		m_font->Draw(
			m_text.c_str(),
			m_position,
			CVector4::White, 
			0.0f, 
			1.0f,
			{0.0f, 0.5f}
		);
		m_font->End(rc);
	}
}
