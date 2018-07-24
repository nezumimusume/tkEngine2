/*!
 * @brief	スプライト。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	スプライト。
	 */
	class CSprite : Noncopyable {
	public:
		static const CVector2	DEFAULT_PIVOT;					//!<ピボット。
		/*!
		 * @brief	コンストラクタ。
		 */
		CSprite();
		/*!
		 * @brief	デストラクタ。
		 */
		~CSprite();
		/*!
		 * @brief	初期化。
		 *@param[in]	tex		テクスチャ。
		 *@param[in]	w		幅。
		 *@param[in]	h		高さ。
		 */
		void Init(CShaderResourceView& tex, float w, float h);
		/*!
		 * @brief	テクスチャを設定。
		 *@param[in]	tex		テクスチャ。
		 */
		void SetTexture(CShaderResourceView& tex)
		{
			m_textureSRV = &tex;
		}
		/*!
		* @brief	乗算カラーを設定。
		*@param[in]	mulColor	乗算カラー。
		*/
		void SetMulColor(const CVector4& mulColor)
		{
			m_mulColor = mulColor;
		}
		/*!
		*@brief	更新
		*@param[in]	trans		平行移動。
		*@param[in]	rot			回転。
		*@param[in]	scale		拡大。
		*@param[in] pivot		基点。
		*						0.5, 0.5で画像の中心が基点。
		*						0.0, 0.0で画像の左下。
		*						1.0, 1.0で画像の右上。
		*						UnityのuGUIに準拠。
		*/
		void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot = DEFAULT_PIVOT);
		/*!
		*@brief	描画
		*@param[in]	viewMatrix		ビュー行列。
		*@param[in]	projMatrix		プロジェクション行列。
		*/
		void Draw(CRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
		
	private:
		struct SSpriteCB {
			CMatrix WVP;		//ワールドビュープロジェクション行列。
			CVector4 mulColor;	//乗算カラー。
		};
		CVector3				m_position = CVector3::Zero;	//!<座標。
		CQuaternion				m_rotation = CQuaternion::Identity;		//!<回転
		CVector3				m_scale = CVector3::One;
		CMatrix					m_world = CMatrix::Identity;	//!<ワールド行列。
		CShader					m_ps;							//!<ピクセルシェーダー。
		CShader					m_vs;							//!<頂点シェーダー。
		CVector4				m_mulColor = CVector4::White;	//!<乗算カラー。
		CPrimitive				m_primitive;					//!<プリミティブ。
		CShaderResourceView*	m_textureSRV = nullptr;			//!<テクスチャ。
		CConstantBuffer			m_cb;
		bool					m_isInited = false;				//!<初期化フラグ。
		CVector2				m_size = CVector2::Zero;		//!<サイズ。
	};
}