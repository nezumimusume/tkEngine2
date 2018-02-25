/*!
 * @brief	プレイヤーの旋回処理。
 */

#pragma once

class CPlayer;

class CPlayerTurn : public IGameObject{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	CPlayerTurn(CPlayer* pl) :
		m_player(pl)
	{
	}
	/*!
	 * @brief	更新。
	 */
	void Update() override final;
private:
	CPlayer* m_player = nullptr;
};