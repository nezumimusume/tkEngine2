#include "stdafx.h"
#include "Game.h"
#include "Title.h"


Game::Game()
{
	//コンストラクタはインスタンスが生成されたときに
	//自動的に呼ばれる特殊な関数。
	m_gameCamera = NewGO<GameCamera>(0);	//後で破棄するので、GameCameraのインスタンスをメンバ変数に記憶しておく。
	m_player = NewGO<Player>(0, "プレイヤー");	//後で破棄するので、Playerのインスタンスをメンバ変数に記憶しておく。
	m_background = NewGO<Background>(0);

	//一つ目のStarを作成する。
	m_star[0] = NewGO<Star>(0, "すたー");
	m_star[0]->m_position.x = 200.0f;
	m_star[0]->m_position.y = 0.0f;
	m_star[0]->m_position.z = 0.0f;

	//二つ目のStarを作成する。
	m_star[1] = NewGO<Star>(0, "すたー");
	m_star[1]->m_position.x = 400.0f;
	m_star[1]->m_position.y = 0.0f;
	m_star[1]->m_position.z = 0.0f;

	//三つ目のStarを作成する。
	m_star[2] = NewGO<Star>(0, "すたー");
	m_star[2]->m_position.x = 600.0f;
	m_star[2]->m_position.y = 0.0f;
	m_star[2]->m_position.z = 0.0f;

	//四つ目のStarを作成する。
	m_star[3] = NewGO<Star>(0, "すたー");
	m_star[3]->m_position.x = 800.0f;
	m_star[3]->m_position.y = 0.0f;
	m_star[3]->m_position.z = 0.0f;

	//五つ目のStarを作成する。
	m_star[4] = NewGO<Star>(0, "すたー");
	m_star[4]->m_position.x = 1000.0f;
	m_star[4]->m_position.y = 200.0f;
	m_star[4]->m_position.z = 0.0f;
}

Game::~Game()
{
	//コンストラクタはインスタンスが破棄されたときに
	//自動的に呼ばれる特殊な関数。
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
	DeleteGO(m_background);
	
	//Starのインスタンスを破棄する。
	for (int i = 0; i < 5; i++) {
		DeleteGO( m_star[i] );
	}
}
void Game::Update()
{
	if (Pad(0).IsPress(enButtonSelect) == true) {
		//セレクトボタンが押された。
		//Titleクラスのインスタンスを生成。
		//これでTitleクラスのコンストラクタが呼ばれる。
		NewGO<Title>(0);
		//Gameクラスのインスタンスを破棄。
		//これでGameクラスのデストラクタが呼ばれる。
		DeleteGO(this);
	}
}
