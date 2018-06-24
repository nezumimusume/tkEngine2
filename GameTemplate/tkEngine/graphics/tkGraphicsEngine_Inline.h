#pragma once


namespace tkEngine{
	/*!
	*@brief	MSAAテクスチャのリゾルブ。
	*@details 
	* MSAAが有効になっているレンダリングターゲットのテクスチャをSRVとして使用したい場合に呼び出してください。。
	*/
	inline void CRenderTarget::ResovleMSAATexture(CRenderContext& rc)
	{
		if (m_isMSAA || m_isResolveTexture) {
			//MSAAが有効でなければ何もしない。
			rc.ResolveSubresource(m_resolveTextureMSAA, 0, m_renderTarget, 0, m_textureFormat);
		}
	}
}