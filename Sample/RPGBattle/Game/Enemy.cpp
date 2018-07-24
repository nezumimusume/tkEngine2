#include "stdafx.h"
#include "Enemy.h"
#include "Game.h"
#include "IAICommand.h"

Enemy::Enemy(LevelObjectData& objData)
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	wchar_t filePath[256];
	m_anmClips[enAnimation_Idle].Load(L"animData/enemy/idle.tka");
	m_anmClips[enAnimation_Idle].SetLoopFlag(true);
	m_anmClips[enAnimation_Attack].Load(L"animData/enemy/attack.tka");
	m_anmClips[enAnimation_Attack].SetLoopFlag(false);
	m_anmClips[enAnimation_Damage].Load(L"animData/enemy/damage.tka");
	m_anmClips[enAnimation_Damage].SetLoopFlag(false);

	m_anmClips[enAnimation_Dead].Load(L"animData/enemy/death.tka");
	m_anmClips[enAnimation_Dead].SetLoopFlag(false);

	swprintf_s(filePath, L"modelData/%s.cmo", objData.name);
	m_skinModelRender->Init(filePath, m_anmClips, enAnimation_Num);
	m_skinModelRender->SetPosition(objData.position);
	m_skinModelRender->SetRotation(objData.rotation);
	m_skinModelRender->SetScale(objData.scale);
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->SetShadowReceiverFlag(true);
	m_specMap.CreateFromDDSTextureFromFile(L"modelData/enemy_00_s.dds");
	m_skinModelRender->FindMaterial([&](CModelEffect* mat) {
		mat->SetSpecularMap(m_specMap.GetBody());
		mat->SetMaterialID(enMaterialID_Enemy);
	});
	m_skinModelRender->AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}
void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	auto ss = NewGO<prefab::CSoundSource>(0);
	char soundFileName[256];
	size_t size;
	wcstombs_s(&size, soundFileName, eventName, 256);
	ss->Init(soundFileName);
	
	ss->SetVolume(4.0f);
	ss->Play(false);
}
void Enemy::GenerateAICommandList(std::deque<IAICommandUPtr>& commands)
{
	//ヘイトの総合値でだれを狙うか決める。
	AIAvator* aiAvator[3] = {
		senshi,
		ryu,
		shiro
	};
	AIAvator* targetAvator[3] = { nullptr };
	int numPlayer = 0;
	int hateTotal = 0;
	//生き残っているプレイヤーを配列に積む。
	for (int i = 0; i < 3; i++) {
		if (aiAvator[i]->GetHP() != 0) {
			hateTotal += aiAvator[i]->GetHate();
			targetAvator[numPlayer] = aiAvator[i];
			numPlayer++;
		}
	}
	if (numPlayer == 0) {
		//全員死んでる。
		//本来ここに来るのはおかしいので、ログを出す。
		TK_LOG("パーティ全滅しています。");
		return;
	}
	if (hateTotal == 0) {
		//ヘイトが0なら完全にランダムで決める。
		int t = Random().GetRandInt() % numPlayer;
		CommandAttack(commands, this, targetAvator[t]);
	}
	else {
		int t = (Random().GetRandInt() % hateTotal)+1;
		int avatorNo = 0;
		int _hate = 0;
		for( ; avatorNo < numPlayer; avatorNo++){
			_hate += aiAvator[avatorNo]->GetHate();
			if (t <= _hate) {
				//ヘイトが一定値を超えた。
				break;
			}
		}
		CommandAttack(commands, this, aiAvator[avatorNo]);
	}
}