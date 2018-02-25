/*!
 * @brief	敵のパス移動(ループ)思考。
 */

#pragma once


class Enemy;
class PathEdge;
class Path;
/*!
 * @brief	パス移動(ループ)思考。
 */
class EnemyPathMoveLoop
{
private:
	enum EnState {
		enState_FindNearEdge,		//近いエッジを検索。
		enState_MoveNearEdge,		//近いエッジに移動。
		enState_MovePath,			//パスを移動。
		enState_MovePath_Reverse,	//パスを逆方向に移動中。。
		enState_Turn,		//次のエッジに向けて旋回。

	};
public:
	EnemyPathMoveLoop(Enemy* enemy, Path& path);
	~EnemyPathMoveLoop();
	void Update();
private:
	/*!
	 *@brief	状態切り替え。
	 */
	void ChangeState(EnState nextState)
	{
		m_state = nextState;
	}
	/*!
	*@brief	ターン状態に切り替えるときはこっちを使う。
	*@param[in]	stateWhenTurnEnd	ターン終了時の次のステート。
	*@param[in]	turnDir				目的進行方向。
	*/
	void ChangeTurnState(EnState stateWhenTurnEnd, const CVector3& targetDir);

private:
	
	const PathEdge* m_currentEdge = nullptr;
	Enemy*	m_enemy = nullptr;
	Path* m_path = nullptr;
	EnState m_state = enState_FindNearEdge;
	EnState m_stateWhenTurnEnd;				//!<ターン終了時の次のステート。	
	float m_angleWhenStartTurn;				//!<ターン開始時のアングル。単位：ラジアン。
	float m_angleWhenEndTurn;				//!<ターン終了時のアングル。単位：ラジアン。
	float m_angle = 0.0f;					//!<ターン中の角度。
	
	
};