#include "stdafx.h"
#include "BattleHud.h"
#include "tkEngine/HUD/node/tkHUDNode_Font.h"
#include "AIAvator.h"

BattleHud::BattleHud()
{
	m_hud = NewGO<prefab::CHud>(0);
	m_hud->Init(L"HUD/battle.tkhud");
	auto rootNode = m_hud->FindNode(L"senshi_parameter");
	rootNode->QueryChildren([&](IHUDNode* node) {
		if(node->EqualNodeName(L"font_name") == true) {
			m_senshiParameterNode.m_nameFontNode = (CHUDNode_Font*)node;
		}
		else if (node->EqualNodeName(L"font_HP") == true) {
			m_senshiParameterNode.m_hpFontNode = (CHUDNode_Font*)node;
		}
		else if (node->EqualNodeName(L"font_MP") == true) {
			m_senshiParameterNode.m_mpFontNode = (CHUDNode_Font*)node;
		}
	});
	
	rootNode = m_hud->FindNode(L"ryusan_parameter");
	rootNode->QueryChildren([&](IHUDNode* node) {
		if (node->EqualNodeName(L"font_name") == true) {
			m_ryusanParameterNode.m_nameFontNode = (CHUDNode_Font*)node;
		}
		else if (node->EqualNodeName(L"font_HP") == true) {
			m_ryusanParameterNode.m_hpFontNode = (CHUDNode_Font*)node;
		}
		else if (node->EqualNodeName(L"font_MP") == true) {
			m_ryusanParameterNode.m_mpFontNode = (CHUDNode_Font*)node;
		}
	});
	rootNode = m_hud->FindNode(L"siro_parameter");
	rootNode->QueryChildren([&](IHUDNode* node) {
		if (node->EqualNodeName(L"font_name") == true) {
			m_siroParameterNode.m_nameFontNode = (CHUDNode_Font*)node;
		}
		else if (node->EqualNodeName(L"font_HP") == true) {
			m_siroParameterNode.m_hpFontNode = (CHUDNode_Font*)node;
		}
		else if (node->EqualNodeName(L"font_MP") == true) {
			m_siroParameterNode.m_mpFontNode = (CHUDNode_Font*)node;
		}
	});
	//ドラクエ風フォントをロード。
	m_dqFont = std::make_unique<DirectX::SpriteFont>(GraphicsEngine().GetD3DDevice(), L"font/dq.spritefont");
	//使用するフォントを差し替える。
	m_hud->QueryFontNode([&](CHUDNode_Font& font) {
		font.SetFont( m_dqFont.get() );
	});
	m_logFont_00 = (CHUDNode_Font*)m_hud->FindNode(L"font_log_00");
	m_logFont_01 = (CHUDNode_Font*)m_hud->FindNode(L"font_log_01");
	m_logFont_02 = (CHUDNode_Font*)m_hud->FindNode(L"font_log_02");
	m_logFont_00->SetText(L"");
	m_logFont_01->SetText(L"");
	m_logFont_02->SetText(L"");
	m_senshiParameterNode.m_nameFontNode->SetText(L"せんし");
	m_ryusanParameterNode.m_nameFontNode->SetText(L"りゅうきし");
	m_siroParameterNode.m_nameFontNode->SetText(L"しろまどうし");

	//戦士の名前ノードを取得する。
}


BattleHud::~BattleHud()
{
	DeleteGO(m_hud);
}
void BattleHud::Update()
{
	UpdateFont(m_senshiParameterNode, m_senshi);
	UpdateFont(m_ryusanParameterNode, m_ryusan);
	UpdateFont(m_siroParameterNode, m_shiro);
}
void BattleHud::UpdateFont(SParameterNode& font, AIAvator* avator)
{
	wchar_t text[256];
	//HPを更新。
	swprintf_s(text, L"HP %d", avator->GetHP());
	font.m_hpFontNode->SetText(text);

	//MPを更新。
	swprintf_s(text, L"MP %d", avator->GetMP());
	font.m_mpFontNode->SetText(text);
}
void BattleHud::SetBattleLog00(const wchar_t* text)
{
	m_logFont_00->SetText(text);
}
void BattleHud::SetBattleLog01(const wchar_t* text)
{
	m_logFont_01->SetText(text);
}
void BattleHud::SetBattleLog02(const wchar_t* text)
{
	m_logFont_02->SetText(text);

}