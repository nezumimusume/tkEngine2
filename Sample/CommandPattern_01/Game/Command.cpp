#include "stdafx.h"
#include "Command.h"


///////////////////////////////////////////
/*!
*@brief	前に歩くコマンド。
*/
///////////////////////////////////////////
void Command_WalkForward::Execute(GameActor& actor)
{
	m_beforePos = actor.GetPosition();
	actor.Walk({0.0f, 0.0f, -200.0f});
}

void Command_WalkForward::Undo(GameActor& actor)
{
	actor.SetPosition(m_beforePos);
}
///////////////////////////////////////////
/*!
*@brief	後ろに歩くコマンド。
*/
///////////////////////////////////////////
void Command_WalkBack::Execute(GameActor& actor)
{
	m_beforePos = actor.GetPosition();
	actor.Walk({ 0.0f, 0.0f, 200.0f });
}
void Command_WalkBack::Undo(GameActor& actor)
{
	actor.SetPosition(m_beforePos);
}
///////////////////////////////////////////
/*!
*@brief	右に歩くコマンド。
*/
///////////////////////////////////////////
void Command_WalkRight::Execute(GameActor& actor)
{
	m_beforePos = actor.GetPosition();
	actor.Walk({ -200.0f, 0.0f, 0.0f });
}
void Command_WalkRight::Undo(GameActor& actor)
{
	actor.SetPosition(m_beforePos);
}
///////////////////////////////////////////
/*!
*@brief	左に歩くコマンド。
*/
///////////////////////////////////////////
void Command_WalkLeft::Execute(GameActor& actor)
{
	m_beforePos = actor.GetPosition();
	actor.Walk({ 200.0f, 0.0f, 0.0f });
}
void Command_WalkLeft::Undo(GameActor& actor)
{
	actor.SetPosition(m_beforePos);
}

void Command_WalkForwardRight::Execute(GameActor& actor)
{
	m_beforePos = actor.GetPosition();
	actor.Walk({ -200.0f, 0.0f, -200.0f });
}
/*!
*@brief		アンドゥを実行。
*/
void Command_WalkForwardRight::Undo(GameActor& actor)
{
	actor.SetPosition(m_beforePos);
}