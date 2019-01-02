/*!
 * @brief	エフェクトエンジン。
 */

#pragma once

#include "tkEngine/resource/tkResourceManager.h"

namespace tkEngine{
	class CPostEffect;
	using CEffectResourceManager = TResourceManager<Effekseer::Effect>;
	/*!
	 * @brief	エフェクトエンジン。
	 */
	class CEffectEngine : Noncopyable {
	public:
		CEffectEngine();
		~CEffectEngine();
		/*!
		 *@brief	解放。
		 */
		void Release();
		/*!
		 *@brief	初期化。
		 */
		void Init();
		/*!
		 *@brief	更新。
		 */
		void Update();
		/*!
		 *@brief	描画。
		 */
		void Render(CRenderContext& rc, CPostEffect* ps);
		/*!
		 *@brief	Effekseerのエフェクトを作成。
		 */
		Effekseer::Effect* CreateEffekseerEffect(const wchar_t* filePath);
		/*!
		 *@brief	Effekseerのエフェクトを再生。
		 */
		Effekseer::Handle Play(Effekseer::Effect* effect);
		/*!
		 *@brief	Effekseerマネージャの取得。
		 */
		Effekseer::Manager& GetEffekseerManager()
		{
			return *m_manager;
		}
		/*!
		 *@brief	Effekseerのエフェクトの停止。
		 */
		void Stop(Effekseer::Handle handle);
		/*!
		 *@brief	リソースマネージャを取得。
		 */
		CEffectResourceManager& GetResourceManager()
		{
			return m_resourcetManager;
		}
	private:
		CEffectResourceManager m_resourcetManager;	//リソースマネージャ。
		Effekseer::Manager*	m_manager= nullptr;
		EffekseerRenderer::Renderer*	m_renderer = nullptr;
		CRenderTarget m_addEffectBuffer;
		std::vector<int>	m_renderFlags;
		ID3D11BlendState* m_finalCombineAddBlendState = nullptr;
		CConstantBuffer m_cb;
		CShader		m_copyVS;					//!<コピー用の頂点シェーダー。
		CShader		m_copyPS;					//!<コピー用の頂点シェーダー。
	};
}