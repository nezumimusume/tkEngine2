/*!
 * @brief	ライトの基底クラス。
 */

#pragma once

namespace tkEngine{
	namespace prefab {
		/*!
		 * @brief	ライトの基底クラス。
		 */
		class CLightBase : public IGameObject {
		public:
			bool Start() override final;
			void OnDestroy() override final;
			/*!
			*@brief ライトの影響を与えるマテリアルIDグループを設定する。
			*/
			void SetLightingMaterialIDGroup(unsigned int group)
			{
				m_lightingMaterialIDGroup = group;
			}
			/*!
			*@brief ライトの影響を与えるマテリアルIDグループを取得。
			*/
			unsigned int GetLightingMaterialIDGroup() const
			{
				return m_lightingMaterialIDGroup;
			}
		private:
			virtual bool StartSub() = 0;
			virtual void OnDestorySub() {}
		private:
			unsigned int m_lightingMaterialIDGroup = 0xFFFFFFFF;	//!<ライトの影響を与える相手を識別するマテリアルIDグループ。0xFFFFFFFFですべてのオブジェクトにライトを当てる。
		};
	}
}