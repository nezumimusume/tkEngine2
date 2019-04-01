/// <summary>
/// シャドウマップの基底クラス。
/// </summary>
#pragma once

namespace tkEngine{
	class IShadowCaster;
	/// <summary>
	/// シャドウマップの基底クラス。
	/// </summary>
	class IShadowMap : Noncopyable{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		IShadowMap() {}
		/// <summary>
		/// デストラクタ。
		/// </summary>
		virtual ~IShadowMap() {}
		/// <summary>
		/// シャドウキャスターの登録。
		/// </summary>
		/// <param name="caster">キャスター</param>
		void Entry(IShadowCaster* caster)
		{
			if (!m_isEnable) {
				return;
			}
			m_shadowCaster.push_back(caster);
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="caster">キャスター</param>
		void Remove(IShadowCaster* caster)
		{
			auto it = std::find(m_shadowCaster.begin(), m_shadowCaster.end(), caster);
			if (it != m_shadowCaster.end()) {
				//見つけたので削除。
				m_shadowCaster.erase(it);
			}
		}
		/// <summary>
		/// 描画処理。
		/// </summary>
		void RenderToShadowMap(CRenderContext& rc);
		/// <summary>
		/// 影生成処理を無効にする。
		/// </summary>
		void Disable()
		{
			m_isEnable = false;
		}
		/// <summary>
		/// 影生成処理を有効にする。
		/// </summary>
		void Enable()
		{
			m_isEnable = true;
		}
	protected:
		/// <summary>
		/// 影生成が有効か判定する。
		/// </summary>
		/// <remark>
		/// 
		/// </remark>
		bool IsEnable() const
		{
			return m_isInited && m_isEnable;
		}
	private:
		/// <summary>
		/// 派生クラスで実装する描画処理。
		/// </summary>
		virtual void RenderToShadowMapImp(CRenderContext& rc) = 0;
	protected:
		bool m_isEnable = false;
		bool m_isInited = false;
		std::vector<IShadowCaster*> m_shadowCaster;			//!<シャドウキャスター。

	};
}
