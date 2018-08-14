#include "stdafx.h"
#include "IAICommand.h"
#include "AIAvator.h"
#include "Game.h"

namespace {
	int CalcDamage(int atk, int def)
	{
		float hoge = (rand() % 100) / 200.0f;
		int damage = max(0.0f, (atk / 2 + atk * hoge) - def / 4);
		return damage;
	}
}
IAICommand::~IAICommand()
{
	g_game->GetBattleHUD().SetBattleLog00(L"");
	g_game->GetBattleHUD().SetBattleLog01(L"");
	g_game->GetBattleHUD().SetBattleLog02(L"");
}

bool AICommandAttack::Execute()
{
	if (m_attackAvator->GetHP() == 0){
		return false; 
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_PlayAttackAnimation:
		if (m_targetAvator->GetHP() == 0) {
			return false;
		}
		m_attackAvator->PlayAnimation(AIAvator::enAnimation_Attack, 0.2f);
		wchar_t text[256];
		swprintf_s(text, L"%sのこうげき", m_attackAvator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_WaitText;
		m_timer = 0.0f;
		break;
	case enStep_WaitText:
		if (m_timer > 2.0f) {
			m_targetAvator->PlayAnimation(AIAvator::enAnimation_Damage, 0.2f);
			wchar_t text[256];
			int damage = CalcDamage(m_attackAvator->GetAtk(), m_targetAvator->GetDef());
			swprintf_s(text, L"%sに%dのダメージをあたえた", m_targetAvator->GetName(), damage);
			m_targetAvator->AddDamage(damage);
			g_game->GetBattleHUD().SetBattleLog01(text);
			if (m_targetAvator->GetHP() == 0) {
				m_targetAvator->PlayAnimation(AIAvator::enAnimation_Dead, 0.2f);
			}
			m_step = enStep_WaitPlayEndAttackAnimation;
			m_timer = 0.0f;
		}
		break;
	case enStep_WaitPlayEndAttackAnimation: {
		bool isEnd = true;
		if (m_targetAvator->GetCurrentAnimationNo() != AIAvator::enAnimation_Dead) {
			if (m_targetAvator->GetCurrentAnimationNo() != AIAvator::enAnimation_Idle
				&& m_targetAvator->IsPlayAnimation() == false
				) {
				m_targetAvator->PlayAnimation(AIAvator::enAnimation_Idle, 0.2f);
			}
			else {
				isEnd = false;
			}
		}
		if (m_attackAvator->IsPlayAnimation() == false
			&& m_attackAvator->GetCurrentAnimationNo() != AIAvator::enAnimation_Idle
			) {
			m_attackAvator->PlayAnimation(AIAvator::enAnimation_Idle, 0.2f);
		}
		else {
			isEnd = false;
		}
		if (isEnd == true) {
			m_step = enStep_WaitText2;
		}
	}	break;
	case enStep_WaitText2:
		if (m_timer > 2.0f) {
			m_endTimer = 0.2f;
			if (m_targetAvator->GetHP() == 0) {
				//死亡。
				wchar_t text[256];
				swprintf_s(text, L"%sはちからつきた", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog02(text);
				if (m_targetAvator->isTenshiNoInori == true) {
					//てんしの祈りがかかっている。
					m_targetAvator->isTenshiNoInori = false;
					m_step = enStep_WaitText3;
				}
				else {
					m_step = enStep_WaitEnd;
				}
				m_endTimer = 2.0f;	//死亡メッセージを読ませるために少し長めにする。

			}else{
		
				m_step = enStep_WaitEnd;
			}
			m_timer = 0.0f;
		}
		break;
	case enStep_WaitText3:
		if (m_timer > 2.0f) {
			wchar_t text[256];
			swprintf_s(text, L"てんしのいのりのこうかで%sはいきかえった", m_targetAvator->GetName());
			g_game->GetBattleHUD().SetBattleLog02(L"");
			g_game->GetBattleHUD().SetBattleLog01(L"");
			g_game->GetBattleHUD().SetBattleLog00(text);
			auto ss = NewGO<prefab::CSoundSource>(0);
			ss->Init(L"sound/kaifuku.wav");
			ss->Play(false);
			auto effect = NewGO<prefab::CEffect>(0);
			effect->Play(L"effect/sukuruto/TenshiNoInoriHatudou.efk");
			auto pos = MainCamera().GetPosition();
			pos += MainCamera().GetForward() * 250.0f;
			pos.y = 0.0f;
			effect->SetPosition(pos);
			effect->SetScale({ 60.0f, 60.0f, 40.0f });
			m_targetAvator->hp = m_targetAvator->maxHp;
			m_timer = 0.0f;
			m_step = enStep_WaitEnd;
		}
		break;
	case enStep_WaitEnd:
		if (m_timer > m_endTimer) {
			//終わり。
			return false;
		}
		break;
	}
	//まだコマンド継続中。
	return true;
}

bool AICommandChouhatu::Execute()
{
	if (m_avator->GetHP() == 0 ) {
		return false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_Start:{
		wchar_t text[256];
		swprintf_s(text, L"%sはちょうはつをおこなった", m_avator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_WaitMessage_00;
		m_timer = 0.0f;
	}break;
	case enStep_WaitMessage_00:
		if (m_timer > 2.0f) {
			wchar_t text[256];
			swprintf_s(text, L"%sはもんすたーをおこらせた", m_avator->GetName());
			g_game->GetBattleHUD().SetBattleLog01(text);
			m_avator->AddHate();
			m_timer = 0.0f;
			m_step = enStep_WaitMessage_01;
		}
		break;
	case enStep_WaitMessage_01:
		if (m_timer > 2.0f) {
			//終わり。
			return false;
		}
		break;
	}
	return true;
}
bool AICommandHoimi::Execute()
{
	if (m_healAvator->GetHP() == 0) {
		return false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_00: {
		wchar_t text[256];
		swprintf_s(text, L"%sはホイミをとなえた", m_healAvator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_01;
		m_timer = 0.0f;
		auto effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/heal.efk");
		auto pos = MainCamera().GetPosition();
		pos += MainCamera().GetForward() * 400.0f;
		effect->SetPosition(pos);
		effect->SetScale({ 15.0f, 15.0f, 15.0f });
		auto ss = NewGO<prefab::CSoundSource>(0);
		ss->Init(L"sound/kaifuku.wav");
		ss->Play(false);
	}break;
	case enStep_01:
		if (m_timer > 2.0f) {
			if (m_healAvator->GetMP() < 50) {
				//
				g_game->GetBattleHUD().SetBattleLog01(L"しかしMPがたりなかった");
			}
			else if (m_targetAvator->GetHP() == 0) {
				wchar_t text[256];
				swprintf_s(text, L"%sはちからつきており こうかがなかった", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			else {
				m_healAvator->UseMP(50);
				m_targetAvator->AddHP(1000);
				wchar_t text[256];
				swprintf_s(text, L"%sはHPが1000かいふくした。", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			m_step = enStep_02;
			m_timer = 0.0f;
		}
		break;
	case enStep_02:
		if (m_timer > 2.0f) {
			return false;
		}
		break;
	}
	return true;
}
bool AICommandZaoriku::Execute()
{
	if (m_healAvator->GetHP() == 0) {
		return false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_00: {
		wchar_t text[256];
		swprintf_s(text, L"%sはザオリクをとなえた", m_healAvator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_01;
		m_timer = 0.0f;
		auto effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/zaoriku/zaoriku.efk");
		auto pos = MainCamera().GetPosition();
		pos += MainCamera().GetForward() * 250.0f;
		pos.y = 0.0f;
		effect->SetPosition(pos);
		effect->SetScale({ 40.0f, 40.0f, 40.0f });
		auto ss = NewGO<prefab::CSoundSource>(0);
		ss->Init(L"sound/kaifuku.wav");
		ss->Play(false);
	}break;
	case enStep_01:
		if (m_timer > 2.0f) {
			if (m_healAvator->GetMP() < 100) {
				//
				g_game->GetBattleHUD().SetBattleLog01(L"しかしMPがたりなかった");
			}
			else if (m_targetAvator->GetHP() != 0) {
				wchar_t text[256];
				swprintf_s(text, L"%sにはこうかがなかった", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			else {
				m_healAvator->UseMP(100);
				m_targetAvator->hp = m_targetAvator->maxHp;
				wchar_t text[256];
				swprintf_s(text, L"%sはいきかえった", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			m_step = enStep_02;
			m_timer = 0.0f;
		}
		break;
	case enStep_02:
		if (m_timer > 2.0f) {
			return false;
		}
		break;
	}
	return true;
}
bool AICommandSukuruto::Execute()
{
	if (m_healAvator->GetHP() == 0) {
		return false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_00: {
		wchar_t text[256];
		swprintf_s(text, L"%sはスクルトをとなえた", m_healAvator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_01;
		m_timer = 0.0f;
		auto effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/sukuruto/sukuruto.efk");
		auto pos = MainCamera().GetPosition();
		pos += MainCamera().GetForward() * 60.0f;
		pos.y = 0.0f;
		effect->SetPosition(pos);
		effect->SetScale({ 60.0f, 40.0f, 40.0f });
		CQuaternion rot;
		rot.SetRotationDeg(CVector3::AxisY, 90.0f);
		effect->SetRotation(rot);
		auto ss = NewGO<prefab::CSoundSource>(0);
		ss->Init(L"sound/kaifuku.wav");
		ss->Play(false);
	}break;
	case enStep_01:
		if (m_timer > 2.0f) {
			if (m_healAvator->GetMP() < 30) {
				//
				g_game->GetBattleHUD().SetBattleLog01(L"しかしMPがたりなかった");
			}
			else {
				senshi->ApplySukuruto();
				ryu->ApplySukuruto();
				shiro->ApplySukuruto();
				shiro->UseMP(30);
				g_game->GetBattleHUD().SetBattleLog01(L"みかたのしゅびりょくがおおきくあがった");
			}
			m_step = enStep_02;
			m_timer = 0.0f;
		}
		break;
	case enStep_02:
		if (m_timer > 2.0f) {
			return false;
		}
		break;
	}
	return true;
}
bool AICommandBaikiruto::Execute()
{
	if (m_healAvator->GetHP() == 0) {
		return false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_00: {
		wchar_t text[256];
		swprintf_s(text, L"%sはバイキルトをとなえた", m_healAvator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_01;
		m_timer = 0.0f;
		auto effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/sukuruto/baikiruto.efk");
		auto pos = MainCamera().GetPosition();
		pos += MainCamera().GetForward() * 450.0f;
		pos.y = 0.0f;
		effect->SetPosition(pos);
		effect->SetScale({ 40.0f, 40.0f, 40.0f });
		auto ss = NewGO<prefab::CSoundSource>(0);
		ss->Init(L"sound/kaifuku.wav");
		ss->Play(false);
	}break;
	case enStep_01:
		if (m_timer > 2.0f) {
			if (m_healAvator->GetMP() < 50) {
				//
				g_game->GetBattleHUD().SetBattleLog01(L"しかしMPがたりなかった");
			}
			else if (m_targetAvator->GetHP() == 0) {
				wchar_t text[256];
				swprintf_s(text, L"%sはちからつきており こうかがなかった", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			else {
				m_healAvator->UseMP(50);
				m_targetAvator->ApplyBaikiruto();
				wchar_t text[256];
				swprintf_s(text, L"%sのこうげきりょくが　おおはばにあがった", m_targetAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			m_step = enStep_02;
			m_timer = 0.0f;
		}
		break;
	case enStep_02:
		if (m_timer > 2.0f) {
			return false;
		}
		break;
	}
	return true;
}
bool AICommandTenshiNoInori::Execute()
{
	if (m_healAvator->GetHP() == 0) {
		return false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	switch (m_step) {
	case enStep_00: {
		wchar_t text[256];
		swprintf_s(text, L"%sはてんしのいのりをとなえた", m_healAvator->GetName());
		g_game->GetBattleHUD().SetBattleLog00(text);
		m_step = enStep_01;
		m_timer = 0.0f;
		auto effect = NewGO<prefab::CEffect>(0);
		effect->Play(L"effect/sukuruto/TenshiNoInori.efk");
		auto pos = MainCamera().GetPosition();
		pos += MainCamera().GetForward() * 250.0f;
		pos.y = 0.0f;
		effect->SetPosition(pos);
		effect->SetScale({ 40.0f, 40.0f, 40.0f });
		auto ss = NewGO<prefab::CSoundSource>(0);
		ss->Init(L"sound/kaifuku.wav");
		ss->Play(false);
	}break;
	case enStep_01:
		if (m_timer > 2.0f) {
			if (m_healAvator->GetMP() < 200) {
				//
				g_game->GetBattleHUD().SetBattleLog01(L"しかしMPがたりなかった");
			}else {
				m_healAvator->UseMP(200);
				m_healAvator->ApplyTenshiNoInori();
				wchar_t text[256];
				swprintf_s(text, L"%sにてんしのいのりのこうかがかかった", m_healAvator->GetName());
				g_game->GetBattleHUD().SetBattleLog01(text);
			}
			m_step = enStep_02;
			m_timer = 0.0f;
		}
		break;
	case enStep_02:
		if (m_timer > 2.0f) {
			return false;
		}
		break;
	}
	return true;
}
//コマンド関数
void CommandAttackEnemy(std::deque<IAICommandUPtr>& commandList, AIAvator* attackAvator)
{
	AIAvator* enemyArray[] = {
		enemy_00,enemy_01,enemy_02
	};
	int t = Random().GetRandInt() % 3;
	while (enemyArray[t]->GetHP() == 0) {
		//再抽選。
		t = Random().GetRandInt() % 3;
	}
	auto command = std::make_unique<AICommandAttack>(attackAvator, enemyArray[t]);
	commandList.push_back(std::move(command));
}
void CommandChouhatu(std::deque<IAICommandUPtr>& commandList, AIAvator* attackAvator)
{
	auto command = std::make_unique<AICommandChouhatu>(attackAvator);
	commandList.push_back(std::move(command));
}
void CommandHoim(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator, AIAvator* targetAvator)
{
	auto command = std::make_unique<AICommandHoimi>(healAvator, targetAvator);
	commandList.push_back(std::move(command));
}
void CommandZaoriku(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator, AIAvator* targetAvator)
{
	auto command = std::make_unique<AICommandZaoriku>(healAvator, targetAvator);
	commandList.push_back(std::move(command));
}
void CommandSukuruto(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator)
{
	auto command = std::make_unique<AICommandSukuruto>(healAvator);
	commandList.push_back(std::move(command));
}
void CommandBaikiruto(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator, AIAvator* targetAvator)
{
	auto command = std::make_unique<AICommandBaikiruto>(healAvator, targetAvator);
	commandList.push_back(std::move(command));
}
void CommandTenshiNoInori(std::deque<IAICommandUPtr>& commandList, AIAvator* healAvator)
{
	auto command = std::make_unique<AICommandTenshiNoInori>(healAvator);
	commandList.push_back(std::move(command));
}