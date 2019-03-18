/*!
 *@brief	レンダリングターゲット切り替えのヘルパークラス。
 */

#pragma once

namespace tkEngine{
	/// <summary>
	/// レンダリングターゲット切り替えクラス。
	/// </summary>
	class CChangeRenderTarget {
		bool m_isRestoreRenderTarget = false;	//レンダリングターゲットを元に戻す？
		CRenderTarget* m_oldRenderTargets[MRT_MAX] = { nullptr };
		unsigned int m_numRenderTargetViews = 0;
		CRenderContext* m_renderContext = nullptr;
	public:
		CChangeRenderTarget(CRenderContext& rc, CRenderTarget& rt, bool isRestoreRenderTarget = false)
		{
			CRenderTarget* rts[] = {
				&rt
			};
			Init(rc, 1, rts, isRestoreRenderTarget);
		}
		CChangeRenderTarget(CRenderContext& rc, int numRenderTargets, CRenderTarget* rts[], bool isRestoreRenderTarget = false)
		{
			Init(rc, numRenderTargets, rts, isRestoreRenderTarget);
		}
		~CChangeRenderTarget()
		{
			if (m_isRestoreRenderTarget == true) {
				//レンダリングターゲットを元に戻す。
				//レンダリングターゲットを戻す。
				m_renderContext->OMSetRenderTargets(m_numRenderTargetViews, m_oldRenderTargets);
				if (m_oldRenderTargets[0] != nullptr) {
					m_renderContext->RSSetViewport(
						0.0f, 0.0f, (float)m_oldRenderTargets[0]->GetWidth(), (float)m_oldRenderTargets[0]->GetHeight());
				}
			}
		}
	private:
		void Init(CRenderContext& rc, int numRenderTargets, CRenderTarget* rts[], bool isRestoreRenderTarget)
		{
			m_renderContext = &rc;
			m_isRestoreRenderTarget = isRestoreRenderTarget;
			if (m_isRestoreRenderTarget == true) {
				//レンダリングターゲットを元に戻すのでバックアップを取っておく。
				rc.OMGetRenderTargets(m_numRenderTargetViews, m_oldRenderTargets);
			}
			
			rc.OMSetRenderTargets(numRenderTargets, rts);
			if (rts[0] != nullptr) {
				rc.RSSetViewport(0.0f, 0.0f, (float)rts[0]->GetWidth(), (float)rts[0]->GetHeight());
			}
		}
	};
}