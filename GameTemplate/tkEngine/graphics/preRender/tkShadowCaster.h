/*!
 * @brief	シャドウキャスター。
 */
 
#pragma once

namespace tkEngine{
	class CSkinModel;
	/*!
	 * @brief	シャドウキャスターのインターフェースクラス。
	 */
	class IShadowCaster{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		IShadowCaster()
		{
		}
		/*!
		 * @brief	デストラクタ。
		 */
		virtual ~IShadowCaster()
		{
		}
		/*!
		* @brief	描画。
		*/
		virtual void Render(CRenderContext& renderContext, const CMatrix& mLVP) = 0;
	};
	/*!
	* @brief	シャドウキャスター(CSkinModel版)
	*/
	class CShadowCaster_SkinModel : public IShadowCaster {
	public:
		CShadowCaster_SkinModel();
		~CShadowCaster_SkinModel();
		/*!
		* @brief	スキンモデルから作成。
		*/
		void Create(CSkinModel* model);
		/*!
		* @brief	描画。
		*/
		void Render(CRenderContext& renderContext, const CMatrix& mLVP) override final;
		
	private:
		CSkinModel*		m_skinModel;	//!<スキンモデル。
	};
}
