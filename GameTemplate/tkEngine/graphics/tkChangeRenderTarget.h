/*!
 *@brief	レンダリングターゲット切り替えのヘルパークラス。
 */

#pragma once

namespace tkEngine{
	/// <summary>
	/// レンダリングターゲット切り替えクラス。
	/// </summary>
	class CChangeRenderTarget {
	public:
		CChangeRenderTarget(CRenderContext& rc, CRenderTarget& rt)
		{
			CRenderTarget* rts[] = {
				&rt
			};
			rc.OMSetRenderTargets(1, rts);
			rc.RSSetViewport(0.0f, 0.0f, (float)rts[0]->GetWidth(), (float)rts[0]->GetHeight());
		}
		~CChangeRenderTarget()
		{
		}
	};
}