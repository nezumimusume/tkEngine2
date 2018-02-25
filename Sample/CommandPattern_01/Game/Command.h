#pragma once

#include "ICommand.h"

///////////////////////////////////////////
/*!
 *@brief	前に歩くコマンド。
 */
 ///////////////////////////////////////////
class Command_WalkForward : public ICommand
{
public:
	Command_WalkForward()
	{
	}
	~Command_WalkForward()
	{
	}

	/*!
	 *@brief	コマンドを実行。
	 */
	void Execute(GameActor& actor) override;
	/*!
	*@brief		アンドゥを実行。
	*/
	void Undo(GameActor& actor) override;
private:
	CVector3 m_beforePos = CVector3::Zero;
};
///////////////////////////////////////////
/*!
*@brief	後ろに歩くコマンド。
*/
///////////////////////////////////////////
class Command_WalkBack : public ICommand
{
public:
	Command_WalkBack()
	{
	}
	~Command_WalkBack()
	{
	}
	void Execute(GameActor& actor) override;
	/*!
	*@brief		アンドゥを実行。
	*/
	void Undo(GameActor& actor) override;
private:
	CVector3 m_beforePos = CVector3::Zero;
};
///////////////////////////////////////////
/*!
*@brief	右に歩くコマンド。
*/
///////////////////////////////////////////
class Command_WalkRight : public ICommand
{
public:
	Command_WalkRight()
	{
	}
	~Command_WalkRight()
	{
	}
	void Execute(GameActor& actor) override;
	/*!
	*@brief		アンドゥを実行。
	*/
	void Undo(GameActor& actor) override;
private:
	CVector3 m_beforePos = CVector3::Zero;
};
///////////////////////////////////////////
/*!
*@brief	左に歩くコマンド。
*/
///////////////////////////////////////////
class Command_WalkLeft : public ICommand
{
public:
	Command_WalkLeft()
	{
	}
	~Command_WalkLeft()
	{
	}
	void Execute(GameActor& actor) override;
	/*!
	*@brief		アンドゥを実行。
	*/
	void Undo(GameActor& actor) override;
private:
	CVector3 m_beforePos = CVector3::Zero;
};

class Command_WalkForwardRight : public ICommand {
public:
	Command_WalkForwardRight()
	{
	}
	~Command_WalkForwardRight()
	{
	}
	void Execute(GameActor& actor) override;
	/*!
	*@brief		アンドゥを実行。
	*/
	void Undo(GameActor& actor) override;
private:
	CVector3 m_beforePos = CVector3::Zero;
};