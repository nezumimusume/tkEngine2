/*!
 * @brief	スプライトレンダラー
 */

#pragma once

namespace tkEngine {
	namespace prefab {
		class CSpriteRender : public IGameObject {
		public:
			CSpriteRender() {}
			/*!
			* @brief	初期化。
			*@param[in]	texFilePath		テクスチャファイルパス。	
			*@param[in]	w		幅。
			*@param[in]	h		高さ。
			*@param[in] isDraw3D	3D空間で描画する？
			*/
			void Init(const wchar_t* texFilePath, float w, float h, bool isDraw3D = false);
			
			/*!
			* @brief	座標を設定。
			*/
			void SetPosition(const CVector3& pos)
			{
				m_position = pos;
			}
			/*!
			* @brief	座標を取得。
			*/
			const CVector3& GetPosition() const
			{
				return m_position;
			}
			/*!
			* @brief	回転を設定。
			*/
			void SetRotation(const CQuaternion& rot)
			{
				m_rotation = rot;
			}
			/*!
			* @brief	回転を取得。
			*/
			const CQuaternion& GetRotation() const
			{
				return m_rotation;
			}
			/*!
			* @brief	ピボットを設定。
			*@param[in] pivot		基点。
			*						0.5, 0.5で画像の中心が基点。
			*						0.0, 0.0で画像の左下。
			*						1.0, 1.0で画像の右上。
			*						UnityのuGUIに準拠。
			*/
			void SetPivot( const CVector2& pivot )
			{
				m_pivot = pivot;
			}
			/*!
			* @brief	乗算カラーを設定。
			*@param[in]	mulColor	乗算カラー。
			*/
			void SetMulColor(const CVector4& mulColor)
			{
				m_sprite.SetMulColor(mulColor);
			}
		public:
			void Update() override final;
			void ForwardRender(CRenderContext& rc)override final;
			void PostRender(CRenderContext& rc) override final;
		private:
			bool m_isDraw3D = false;		//!<3D空間で描画する？
			CShaderResourceView m_texture;	//!<テクスチャ。
			CSprite	m_sprite;				//!<スプライト。
			CVector3 m_position;			//!<座標。
			CQuaternion m_rotation;			//!<回転。
			CVector3 m_scale = CVector3::One;			//!<拡大率。
			CVector2 m_pivot = CSprite::DEFAULT_PIVOT;	//!<ピボット。
		};
	}
}