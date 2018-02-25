#pragma once

#include "ICommand.h"

///////////////////////////////////////////
/*!
 *@brief	歩くコマンド。
 */
 ///////////////////////////////////////////
class Command_Walk : public ICommand
{
public:
	Command_Walk()
	{
	}
	~Command_Walk()
	{
	}

	/*!
	 *@brief	コマンドを実行。
	 */
	void Execute(GameActor& actor) override;
};
///////////////////////////////////////////
/*!
*@brief	走るコマンド。
*/
///////////////////////////////////////////
class Command_Run : public ICommand
{
public:
	Command_Run()
	{
	}
	~Command_Run()
	{
	}
	void Execute(GameActor& actor) override;
};
///////////////////////////////////////////
/*!
*@brief	右に旋回するコマンド。
*/
///////////////////////////////////////////
class Command_TurnRight : public ICommand
{
public:
	Command_TurnRight()
	{
	}
	~Command_TurnRight()
	{
	}
	void Execute(GameActor& actor) override;
};
///////////////////////////////////////////
/*!
*@brief	左に旋回するコマンド。
*/
///////////////////////////////////////////
class Command_TurnLeft : public ICommand
{
public:
	Command_TurnLeft()
	{
	}
	~Command_TurnLeft()
	{
	}
	void Execute(GameActor& actor) override;
};
///////////////////////////////////////////
/*!
*@brief	待機コマンド。
*/
///////////////////////////////////////////
class Command_Idle : public ICommand
{
public:
	Command_Idle()
	{
	}
	~Command_Idle()
	{
	}
	void Execute(GameActor& actor) override;
};




