/*!
 * @brief	ベクトルレンダラー
 */

#pragma once

namespace tkEngine{
	namespace prefab{
		/*!
		 * @brief	ベクトルレンダラー。
		 */
		class CVectorRender : public IGameObject{
		public:

			/*!
			* @brief	描画データ。
			*/
			struct SRenderData {
				std::string name;						//名前
				CVector3 vector = CVector3::One;		//ベクトル。
				CVector3 origin = CVector3::Zero;		//基点。
			};
			/*!
			* @brief	描画するベクトルを追加。
			*/
			void AddVector(SRenderData& renderData)
			{
				m_renderData.push_back(std::move(renderData));
			}
			/*!
			* @brief	描画フラグを設定。
			*/
			void SetRenderFlag(bool flag)
			{
				m_isRender = flag;
			}
			/*!
			* @brief	描画するベクトルの名前リストを指定。
			*/
			void SetRenderVectorNameList(const std::vector<std::string>& list)
			{
				m_renderVectorNameList = list;
			}
		private:
			bool IsRenderVector( const SRenderData& renderData ) const;
			bool Start() override;
			void ForwardRender( CRenderContext& rc ) override;
			void PostRender(CRenderContext& rc) override;
		private:
			bool m_isRender = false;					//!<描画フラグ。
			std::vector<SRenderData>	m_renderData;	//!<描画データ。
			CSkinModel m_model;							//!<モデル。
			CSkinModelData m_modelData;					//!<モデルデータ。
			CFont m_font;								//!<フォント。
			std::uint32_t m_renderTagMask;				//!<描画するタグのマスク
			std::vector<std::string>	m_renderVectorNameList;
		};
	}

}
