#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "tkEngine/light/tkPointLight.h"
#include "Enemy.h"
#include "Senshi.h"
#include "Ryukishi.h"
#include "Shiromadoushi.h"


AIAvator* shiro = nullptr;
AIAvator* ryu = nullptr;
AIAvator* senshi = nullptr;
AIAvator* enemy_00 = nullptr;
AIAvator* enemy_01 = nullptr;
AIAvator* enemy_02 = nullptr;

Game* g_game = nullptr;
Game::Game()
{
	g_game = this;
}


Game::~Game()
{
	for (auto& aiAvator : m_aiAvatorList) {
		DeleteGO(aiAvator);
	}
	for (auto& l : m_dirLight) {
		DeleteGO(l);
	}
	for (auto& l : m_pointLight) {
		DeleteGO(l);
	}
}
bool Game::Start()
{
	//レベルを構築。
	level.Init(L"level/level00.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"enemy_00") == true) {
			auto enemy = NewGO<Enemy>(0, "enemy_00");
			enemy->Init(objData);
			wchar_t name[256];
			wchar_t* hoge[] = { L"A", L"B", L"C" };
			swprintf_s(name, L"モンスター%s", hoge[m_aiAvatorList.size()]);
			enemy->SetParameter(name, 1000, 0, 1200, 3500);

			m_aiAvatorList.push_back(enemy);
			return true;
		}else if (objData.EqualObjectName(L"pointLig") == true){
			//ポイントライト。
			auto ptLig = NewGO<prefab::CPointLight>(0);
			ptLig->SetPosition(objData.position);
			ptLig->SetAttn({ 200.0f, 3.0f, 0.0f });
			ptLig->SetColor({ 10.0f, 5.0f, 5.0f });
			m_pointLight.push_back(ptLig);
			return true;
		}else if (objData.EqualObjectName(L"Camera001") == true) {
			//カメラ。
			MainCamera().SetPosition(objData.position);
			CMatrix mCameraRot;
			mCameraRot.MakeRotationFromQuaternion(objData.rotation);
			auto target = objData.position;
			target.x += -mCameraRot.m[1][0];
			target.y += -mCameraRot.m[1][1];
			target.z += -mCameraRot.m[1][2];
			MainCamera().SetFar(10000.0f);
			MainCamera().SetTarget(target);
			MainCamera().Update();
			return true;
		}
		return false;
	});
	enemy_00 = m_aiAvatorList[0];
	enemy_01 = m_aiAvatorList[1];
	enemy_02 = m_aiAvatorList[2];

	//パーティを編成。
	senshi = NewGO<Senshi>(0);
	senshi->SetParameter(L"せんし", 2000, 0, 100, 1500);
	m_aiAvatorList.push_back(senshi);

	ryu = NewGO<Ryukishi>(0);
	ryu->SetParameter(L"りゅうきし", 1000, 100, 2000, 500);
	m_aiAvatorList.push_back(ryu);

	shiro = NewGO < Shiromadoushi>(0);
	shiro->SetParameter(L"しろまどうし", 1500, 1000, 300, 1000);
	m_aiAvatorList.push_back(shiro);

	m_hud.SetAIAvator(senshi, ryu, shiro);

	//ディレクションライトを作成する。
	auto lig = NewGO<prefab::CDirectionLight>(0);
	auto ligDir = CVector3(1.0f, -1.0f, -1.0f);
	ligDir.Normalize();
	lig->SetDirection(ligDir);
	lig->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	m_dirLight.push_back(lig);

	shadow::DirectionShadowMap().SetLightDirection(ligDir);
	lig = NewGO<prefab::CDirectionLight>(0);
	ligDir.Set(1.0f, 1.0f, -1.0f);
	lig->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	lig->SetDirection(ligDir);
	m_dirLight.push_back(lig);

	//敵用のライトを作成する。
	{
		lig = NewGO<prefab::CDirectionLight>(0);
		ligDir.Set(0.0f, 0.0f, -1.0f);
		lig->SetColor({ 20.0f, 20.0f, 20.0f, 1.0f });
		lig->SetDirection(ligDir);
		lig->SetLightingMaterialIDGroup(1 << enMaterialID_Enemy);
		m_dirLight.push_back(lig);

		lig = NewGO<prefab::CDirectionLight>(0);
		ligDir.Set(1.0f, 0.0f, 0.0f);
		ligDir.Normalize();
		lig->SetColor({ 10.0f, 1.0f, 1.0f, 1.0f });
		lig->SetDirection(ligDir);
		lig->SetLightingMaterialIDGroup(1 << enMaterialID_Enemy);
		m_dirLight.push_back(lig);

		lig = NewGO<prefab::CDirectionLight>(0);
		ligDir.Set(-1.0f, 0.0f, 0.0f);
		ligDir.Normalize();
		lig->SetColor({ 10.0f, 1.0f, 1.0f, 1.0f });
		lig->SetDirection(ligDir);
		lig->SetLightingMaterialIDGroup(1 << enMaterialID_Enemy);
		m_dirLight.push_back(lig);
	}
	LightManager().SetAmbientLight({ 0.1f, 0.1f, 0.1f });

	m_bgm = NewGO<prefab::CSoundSource>(0);
	m_bgm->Init(L"sound/bgm.wav");
	m_bgm->Play(true);
	m_hud.SetBattleLog00(L"モンスターがあらわれた");
	return true;

}

void Game::Update()
{
	switch (m_step) {
	case enStepWaitBattleStart:
		m_timer += GameTime().GetFrameDeltaTime();
		if (m_timer > 5.0f) {
			m_step = enStepDecideCommand;
		}
		break;
	case enStepDecideCommand:
		//AIのコマンドリストを作成する。
		if (m_aiAvatorList[m_currentAvatorNo]->GetHP() != 0) {
			m_aiAvatorList[m_currentAvatorNo]->GenerateAICommandList(m_aiCommandList);
		}
		m_aiAvatorList[m_currentAvatorNo]->UpdateStatus();
		m_step = enStepExecuteCommand;
		break;
	case enStepExecuteCommand: {
		if (m_aiCommandList.empty() == true) {
			//コマンドを全部処理した。
			m_currentAvatorNo = (m_currentAvatorNo + 1) % m_aiAvatorList.size();
			m_step = enStepDecideCommand;
		}
		else {
			auto& command = m_aiCommandList[0];
			if (command->Execute() == false) {
				//コマンド終了。
				m_aiCommandList.pop_front();
				if (enemy_00->GetHP() == 0 && enemy_01->GetHP() == 0 && enemy_02->GetHP() == 0) {
					//ゲームクリア。
					m_step = enStepGameClear;
					auto ss = NewGO<prefab::CSoundSource>(0);
					ss->Init(L"sound/gameclear.wav");
					ss->Play(false);
				}if (ryu->GetHP() == 0 && senshi->GetHP() == 0 && shiro->GetHP() == 0) {
					//ゲームオーバー。
					m_step = enStepGameOver;
					auto ss = NewGO<prefab::CSoundSource>(0);
					ss->Init(L"sound/gameover.wav");
					ss->Play(false);
				}
			}
		}
		break;
	case enStepGameOver:
		if (m_bgm != nullptr) {
			float v = m_bgm->GetVolume();
			v -= 0.1f;
			if (v < 0.0f) {
				DeleteGO(m_bgm);
				m_bgm = nullptr;
			}
			else {
				m_bgm->SetVolume(v);
			}
		}
		m_hud.SetBattleLog00(L"パーティはぜんめつした");
		break;
	case enStepGameClear:
		if (m_bgm != nullptr) {
			float v = m_bgm->GetVolume();
			v -= 0.1f;
			if (v < 0.0f) {
				DeleteGO(m_bgm);
				m_bgm = nullptr;
			}
			else {
				m_bgm->SetVolume(v);
			}
		}
		m_hud.SetBattleLog00(L"モンスターをたおした");
		break;
	}break;
	}
	//プレイヤーのパラメータを更新する。
	m_hud.Update();

	//m_timer += GameTime().GetFrameDeltaTime();
	//if (m_timer > 2.0f) {
	//	if (Random().GetRandDouble() < 0.9f) {
	//		//エネミーのうめき声をランダムに再生する。
	//		auto ss = NewGO<prefab::CSoundSource>(0);
	//		char filePath[256];
	//		int t = Random().GetRandInt() % 3;
	//		sprintf(filePath, "sound/enemy_voice_0%d.wav", t);
	//		ss->Init(filePath);
	//		ss->Play(false);
	//	}
	//	m_timer = 0.0f;
	//}
}