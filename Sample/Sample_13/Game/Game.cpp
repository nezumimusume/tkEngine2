#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	
	return true;
}
/*!
*@brief	削除されるときに呼ばれる。
*@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
* デストラクタより前に実行されます。
*/
void Game::OnDestroy()
{
	//サウンドソースのインスタンスを破棄するとBGMは停止する。
	DeleteGO(m_bgmSoundSource);
}
void Game::Update()
{
	prefab::CSoundSource* ss;
	if (Pad(0).IsTrigger(enButtonA)) {
		ss = NewGO<prefab::CSoundSource>(0);
		ss->Init("sound/PlayerAttack_00.wav");
		ss->Play(false);	//ループフラグをfalseに設定して再生すると、
							//再生が終了するとCSoundSourcenのインスタンスは自動的に削除されるので
							//インスタンスをメンバ変数などで保持する必要はない。
	}
	else if (Pad(0).IsTrigger(enButtonB)) {
		ss = NewGO<prefab::CSoundSource>(0);
		ss->Init("sound/PlayerAttack_01.wav");
		ss->Play(false);
	}else if (Pad(0).IsTrigger(enButtonX)) {
		ss = NewGO<prefab::CSoundSource>(0);
		ss->Init("sound/PlayerDead.wav");
		ss->Play(false);
	}
	else if (Pad(0).IsTrigger(enButtonY)) {
		if (m_bgmSoundSource == nullptr) {
			//BGMを流していない。
			//サウンドソースを作成。
			m_bgmSoundSource = NewGO<prefab::CSoundSource>(0);
			//初期化。
			m_bgmSoundSource->Init("sound/bgm.wav");
			//ループフラグをtrueにして再生しているので、
			//音を止めるときは明示的にインスタンスを破棄する必要がある。
			m_bgmSoundSource->Play(true);
		}
		else {
			//BGMを削除。
			DeleteGO(m_bgmSoundSource);
			m_bgmSoundSource = nullptr;
		}
	}
}
