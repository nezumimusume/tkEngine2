#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}
bool Player::Start()
{
	//アクターを検索。
	m_gameActor = FindGO<GameActor>("暗殺者");
	return true;
}
void Player::Update()
{
	ICommandPtr command;
	//前進[XBoxコントローラ　: Y、キーボード : I ]
	//後退[XBoxコントローラ　: B、キーボード : K ]
	//右  [XBoxコントローラ　: X、キーボード : L ]
	//左  [XBoxコントローラ　: A、キーボード : J ]
	
	//Question 1 Command.h,cppに右斜め前に移動するコマンドを追加して、移動できるようにしなさい。
	//			 右斜め前は前進＋右のキー入力で行いなさい。
	//Question 2 Command.h,cppに左斜め前に移動するコマンドを追加して、移動できるようにしなさい。
	//			 左斜め前は前進＋左のキー入力で行いなさい。
	if (Pad(0).IsPress(enButtonY)
		&& Pad(0).IsPress(enButtonX)) {
		command = std::make_shared<Command_WalkForwardRight>();
	}else if (Pad(0).IsPress(enButtonY)) {
		//前に進むコマンドを生成。
		command =std::make_shared<Command_WalkForward>();
	}else if (Pad(0).IsPress(enButtonB)) {
		//後ろに進むコマンドを生成。
		command = std::make_shared<Command_WalkBack>();
	}
	else if (Pad(0).IsPress(enButtonX)) {
		//右に進むコマンドを生成。
		command = std::make_shared<Command_WalkRight>();
	}
	else if (Pad(0).IsPress(enButtonA)) {
		//左に進むコマンドを生成。
		command = std::make_shared<Command_WalkLeft>();
	}
	
	if (command != nullptr) {
		//コマンドが生成されている。
		if (m_currentCommandPos != m_commands.size() - 1) {
			//現在のコマンド位置より後ろは削除する。
			m_commands.resize(m_currentCommandPos + 1);
		}
		command->Execute(*m_gameActor);
		//コマンドの履歴に積む。
		m_commands.push_back(command);
		m_currentCommandPos++;
	}
	//アンドゥ
	if (GetAsyncKeyState('Z') 
		&& GetAsyncKeyState(VK_CONTROL) 
		&& m_currentCommandPos >= 0
	) {
		//アンドゥを行う。
		m_commands[m_currentCommandPos]->Undo(*m_gameActor);
		m_currentCommandPos--;
	}
	
	//リドゥ
	if (GetAsyncKeyState('Y')
		&& GetAsyncKeyState(VK_CONTROL)
		&& m_currentCommandPos + 1 < m_commands.size()
		) {
		
		//リドゥを行う。
		m_currentCommandPos++;
		m_commands[m_currentCommandPos]->Execute(*m_gameActor);
	}
	
}