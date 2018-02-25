#pragma once

#include "GameActor.h"


/*!
 *@brief	コマンドのインターフェースクラス。
 */
class ICommand
{
public:
	ICommand()
	{
	}
	virtual ~ICommand()
	{
	}
	/*!
	 *@brief	コマンドを実行。
	 */
	virtual void Execute(GameActor& player) = 0;
};

