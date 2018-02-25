#pragma once
//キャラクターコントローラーを使用する場合は、下記のヘッダーファイルのインクルードが必要。
#include "tkEngine/character/tkCharacterController.h"
class ICommand;
class GameActor : public IGameObject
{
private:

	enum EnAnimationClip {
		enAnimationClip_idle,	//待機アニメーション。
		enAnimationClip_run,	//走りアニメーション。
		enAnimationClip_walk,	//歩きアニメーション。
		enAnimationClip_num,	//アニメーションクリップの総数。
	};
public:
	GameActor();
	~GameActor();
	void Update();
	void Render(CRenderContext& rc);
	bool Start();

	
	/*!
	 *@brief	歩く。
	 */
	void Walk(CVector3 move);
	
	/*!
	 *@brief	座標を取得。
	 */
	CVector3 GetPosition() const
	{
		return m_position;
	}
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(pos);
	}
	/*!
	 *@brief	命令を追加。
	 */
	void AddCommand(ICommand* command)
	{
		m_commands.push_back(command);
		m_commandsRecord.push_back(command);	//これはおまけ気にしなくておｋ。
	}
	/*!
	 *@brief	レコードの再生開始。
	 *@details
	 * オマケオマケ。
	 */
	void PlayRecord()
	{
		//全部初期値に戻す。
		m_moveSpeedDown = CVector3::Zero;
		m_position = CVector3::Zero;
		m_rotation = CQuaternion::Identity;
		m_moveDir = CVector3::Back;
		m_charaCon.SetPosition(m_position);
		//レコード再生中のを立てる。
		m_isPlayRecord = true;
	}
	bool IsPlayRecord() const
	{
		return m_isPlayRecord;
	}
private:
	//アニメーションを初期化。
	void InitAnimation();
	/*!
	 *@brief	アニメーションを再生。
	 */
	void PlayAnimation(EnAnimationClip nextClip)
	{
		if (m_currentAnimationClip != nextClip) {
			m_animation.Play(nextClip, 0.2f);
			m_currentAnimationClip = nextClip;
		}
	}
	/*!
	 *@brief	コマンドを実行。
	 */
	void ExecuteCommand();
	/*!
	 *@brief	レコードを再生。
	 */
	void ExecuteRecord();
private:
	
	EnAnimationClip				m_currentAnimationClip;		//現在のアニメーションクリップ。
	CAnimationClip				m_animationClip[enAnimationClip_num];	//アニメーションクリップ。
	CAnimation					m_animation;				//アニメーション。
	CSkinModelData				m_skinModelData;			//スキンモデルデータ。
	CSkinModel					m_skinModel;				//スキンモデル。
	CVector3					m_position = CVector3::Zero;	//座標。
	CQuaternion					m_rotation = CQuaternion::Identity;	//回転。
	CCharacterController		m_charaCon;					//キャラクターコントローラー。
	CVector3					m_moveDir = CVector3::Back;	//進行方向。
	prefab::CDirectionLight*	m_directionLig = nullptr;
	CShaderResourceView			m_normalMapSRV;				//!<法線マップのSRV。
	CShaderResourceView			m_specularMapSRV;			//!<スペキュラマップのSRV。
	CShaderResourceView			m_wnormalMapSRV;			//!<武器の法線マップのSRV。
	CShaderResourceView			m_wspecularMapSRV;			//!<武器のスペキュラマップのSRV。
	CVector3					m_moveSpeedDown = CVector3::Zero;
	std::deque<ICommand*>		m_commands;					//!<命令リスト。

	///////////////////////////////////////////////////////////////////////
	// オマケだよ。
	///////////////////////////////////////////////////////////////////////
	bool						m_isPlayRecord;
	std::deque<ICommand*>		m_commandsRecord;			//!<全コマンドの記録。
};

