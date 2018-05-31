/*!
 * @brief	スキンモデルレンダラー
 */

#pragma once

#include "tkEngine/culling/tkObjectFrustumCulling.h"

namespace tkEngine{
namespace prefab{
	class CSkinModelRender : public IGameObject {
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		CSkinModelRender() {}
		/*!
		* @brief	初期化。
		*@param[in]	filePath	ファイルパス。
		*@param[in]	animationClips		アニメーションクリップ。
		*@param[in]	numAnimClip			アニメーションクリップの数。
		*@param[in]	enUpdateAxis		fbxの上方向。
		*/
		void Init(const wchar_t* filePath,
			CAnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			CSkinModel::EnFbxUpAxis fbxUpAxis = CSkinModel::enFbxUpAxisZ);
		
		/*!
		 * @brief	更新前に呼ばれる関数。
		 */
		bool Start() override final;
		/*!
		 * @brief	更新。
		 */
		void Update() override final;
		/*!
		 * @brief	描画。
		 */
		void Render(CRenderContext& rc) override final;
		/*!
		* @brief	アニメーションを再生。
		*@param[in]	clipNo	アニメーションクリップの番号。コンストラクタに渡したanimClipListの並びとなる。
		*@param[in]	interpolateTime		補完時間(単位：秒)
		*/
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/*!
		* @brief	アニメーションの再生中？
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/*!
		* @brief	シャドウキャスターのフラグを設定。
		*@param[in]	flag	シャドウキャスターのフラグ。
		*/
		void SetShadowCasterFlag(bool flag)
		{
			m_skinModel.SetShadowCasterFlag(flag);
		}
		/*!
		* @brief	シャドウレシーバーのフラグを設定。
		*@param[in]	flag	シャドウレシーバーのフラグ。
		*/
		void SetShadowReceiverFlag(bool flag)
		{
			m_skinModel.SetShadowReceiverFlag(flag);
		}
		/*!
		*@brief	フリーズさせているボーンの平行移動量を取得。
		*@details
		* 最終ポーズのアニメーションだけ。
		*/
		CVector3 GetFreezeBoneTranslate() const
		{
			return m_animation.GetFreezeBoneTranslate();
		}
		/*!
		*@brief	アニメーションイベントリスナーを登録。
		*@return
		* 登録されたリスナー。
		*/
		void AddAnimationEventListener(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/*!
		*@brief	座標を設定。
		*/
		void SetPosition(const CVector3& position)
		{
			m_position = position;
		}
		/*!
		*@brief	回転を設定。
		*/
		void SetRotation(const CQuaternion& rotation)
		{
			m_rotation = rotation;
		}
		/*!
		*@brief	拡大率を設定。
		*/
		void SetScale(const CVector3& scale)
		{
			m_scale = scale;
		}
		/*!
		*@brief	スキンモデルデータを取得。
		*/
		CSkinModelData& GetSkinModelData() 
		{
			return m_skinModelData;
		}
		const CSkinModelData& GetSkinModelData() const
		{
			return m_skinModelData;
		}
		/*!
		*@brief	スキンモデル取得。
		*/
		CSkinModel& GetSkinModel()
		{
			return m_skinModel;
		}
		const CSkinModel& GetSkinModel() const
		{
			return m_skinModel;
		}
		/*!
		*@brief	視推台カリングを行うかどうかのフラグを設定・
		*@details
		* このフラグをtrueにすると画面に映っていないオブジェクトは表示されなくなります。
		*/
		void SetFrustumCulling(bool flag)
		{
			m_isFrustumCulling = flag;
		}
		/*!
		*@brief	モデルマテリアルの検索。
		*@param[in]	findEffect		マテリアルを見つけた時に呼ばれるコールバック関数
		*/
		void FindMaterial(CSkinModelData::OnFindMaterial findMaterial) const
		{
			m_skinModel.FindMaterial(findMaterial);
		}
		/*!
		*@brief	アニメーションの更新フラグを設定。
		*/
		void SetUpdateAnimationFlag(bool flag)
		{
			m_isUpdateAnimation = flag;
		}
		/*!
		* @brief	アニメーションの初期化。
		*/
		void InitAnimation(CAnimationClip* animationClips, int numAnimationClips);
	private:
		CAnimationClip*				m_animationClips = nullptr;			//!<アニメーションクリップ。
		int							m_numAnimationClips = 0;			//!<アニメーションクリップの数。
		CVector3 					m_position = CVector3::Zero;		//!<座標。
		CQuaternion	 				m_rotation = CQuaternion::Identity;	//!<回転。
		CVector3					m_scale = CVector3::One;			//!<拡大率。
		CSkinModel::EnFbxUpAxis		m_enFbxUpAxis = CSkinModel::enFbxUpAxisZ;	//!<FBXの上方向。
		CAnimation					m_animation;						//!<アニメーション。
		CSkinModel					m_skinModel;						//!<スキンモデル。
		CSkinModelData				m_skinModelData;					//!<スキンモデルデータ。
		const std::wstring			m_filePath;							//!<ファイルパス。
		bool						m_isFrustumCulling = false;			//!<フラスタムカリングを行う？
		CObjectFrustumCulling		m_frustumCulling;					//!<フラスタムカリング。
		bool						m_isUpdateAnimation = true;			//!<アニメーションを更新する？
	};
}
}