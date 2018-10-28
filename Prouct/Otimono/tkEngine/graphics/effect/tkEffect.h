/*!
 * @brief	エフェクト
 */

#pragma once

namespace tkEngine{
	namespace prefab {
		/*!
		 * @brief	エフェクト。
		 */
		class CEffect : public IGameObject{
		public:
			CEffect();
			~CEffect();
			/*!
			 *@brief	解放。
			 */
			void Release();
			/*!
			 *@brief	エフェクトを再生。
			 *@param[in]	filePath		再生するエフェクトのファイルパス。
			 */
			void Play(const wchar_t* filePath);
			/*!
			 *@brief	座標を設定。
			 */
			void SetPosition(const CVector3& pos)
			{
				m_position = pos;
			}
			/*!
			 *@brief	回転の設定。
			 */
			void SetRotation(const CQuaternion& rot)
			{
				m_rotation = rot;
			}
			/*!
			 *@brief	拡大率を設定。
			 */
			void SetScale(const CVector3& scale)
			{
				m_scale = scale;
			}
			/*!
			 *@brief	エフェクトが再生中か判定。
			 */
			bool IsPlay() const
			{
				return GraphicsEngine().GetEffectEngine().GetEffekseerManager().GetShown(m_handle);
			}
			/*!
			 *@brief	更新。
			 */
			void Update() override;
		private:
			Effekseer::Effect*	m_effect = nullptr;
			Effekseer::Handle	m_handle = -1;
			CVector3 m_position = CVector3::Zero;			//!<座標。
			CQuaternion m_rotation = CQuaternion::Identity;	//!<回転。
			CVector3 m_scale = CVector3::One;				//!<拡大率。
		};
	}
}