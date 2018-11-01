/*!
 *@brief	FootIK
 */

#pragma once

#include "tkEngine/physics/tkCapsuleCollider.h"


namespace tkEngine{
	class CSkeleton;
	class CBone;

	/*!
	 *@brief	FootIK
	 *@details
	 * 足が地面にめり込まないようにするする処理。
	 * CCD-IK法を使用しています。
	 */
	class CAnimationFootIK{
	private:
		/*!
		*@brief	足。
		*/
		struct SFoot {
			CBone*			m_bone;			//足のボーン。
			CCapsuleCollider m_collider;	//エンドエフェクタに設定されているボーン。
		};
	public:
		/*!
		*@brief	FootIKのパラメータ。
		*/
		struct SFootIKParam {
			const wchar_t* footBoneName_0 = nullptr;	//!<足のボーンの名前0。
			const wchar_t* footBoneName_1 = nullptr;	//!<足のボーンの名前1。
			const wchar_t* rootBoneName = nullptr;		//!<このIKのルートとなる骨の名前。スケルトンのルートではなく、IKの計算が終わらせたいボーンの名前を指定。骨盤とかお尻のボーンになると思われる。
			float footCapsuleColliderHeight_0 = 0.0f;	//!<カプセルコライダーの高さ0。
			float footCapsuleColliderHeight_1 = 0.0f;	//!<カプセルコライダーの高さ1。
			float footCapsuleColliderRadius_0 = 0.0f;	//!<カプセルコライダーの半径0。
			float footCapsuleColliderRadius_1 = 0.0f;	//!<カプセルコライダーの半径1。
		};
		/*!
		 *@brief	コンストラクタ。
		 */
		CAnimationFootIK();
		/*!
		 *@brief	デストラクタ。
		 */
		~CAnimationFootIK();
		/*!
		*@brief	初期化。
		*/
		void Init(CSkeleton* skeleton);
		/*!
		 *@brief	更新。
		 */
		void Update();
		/*!
		*@brief	FootIKを有効にする。
		*/
		void Enable(const SFootIKParam& param);
		
		/*!
		*@brief	FootIKを無効にする。
		*/
		void Disable()
		{
			m_isEnable = false;
		}
	private:
		/*!
		 *@brief	CCD-IK法を使ったFootIKを実行。
		 */
		void SolverCCD_IK(const SFoot& foot, CVector3 targetPosition);
		/*!
		 *@brief	PIK法を使ったFootIKを実行。
		 */
		void SolverPIK(const SFoot& foot, CVector3 targetPosition);
		bool CalcIKTargetPosition(const SFoot& foot, CVector3& targetPosition);
	private:
		int				m_rootBoneId = -1;	//!<ルートのボーンID
		CSkeleton*		m_skeleton;		//!<スケルトン。
		SFoot			m_foots[2];		//!<足。
		bool			m_isEnable = false;	//!<FootIKが有効？
		
	};
}