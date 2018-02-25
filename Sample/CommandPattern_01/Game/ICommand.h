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
	virtual void Execute(GameActor& actor) = 0;
	/*!
	*@brief	アンドゥを実行。
	*/
	virtual void Undo(GameActor& actor) = 0;
};

