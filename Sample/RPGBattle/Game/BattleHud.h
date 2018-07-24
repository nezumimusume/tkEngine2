#pragma once
#include "tkEngine/HUD/tkHUD.h"
class AIAvator;

namespace tkEngine {
	class CHUDNode_Font;
}
class BattleHud
{
private:
	struct SParameterNode {
		CHUDNode_Font* m_nameFontNode;
		CHUDNode_Font* m_hpFontNode;
		CHUDNode_Font* m_mpFontNode;
	};

public:
	BattleHud();
	~BattleHud();
	void SetAIAvator(AIAvator* senshi, AIAvator* ryusan, AIAvator* shiro)
	{
		m_senshi = senshi;
		m_shiro = shiro;
		m_ryusan = ryusan;
	}
	void Update();
	void SetBattleLog00(const wchar_t* text);
	void SetBattleLog01(const wchar_t* text);
	void SetBattleLog02(const wchar_t* text);
private:
	void UpdateFont(SParameterNode& font, AIAvator* avator);
private:
	prefab::CHud* m_hud = nullptr;	//HUD
	
	SParameterNode m_senshiParameterNode;
	SParameterNode m_ryusanParameterNode;
	SParameterNode m_siroParameterNode;
	CHUDNode_Font* m_logFont_00 = nullptr;
	CHUDNode_Font* m_logFont_01 = nullptr;
	CHUDNode_Font* m_logFont_02 = nullptr;
	AIAvator* m_senshi = nullptr;
	AIAvator* m_shiro = nullptr;
	AIAvator* m_ryusan = nullptr;
};

