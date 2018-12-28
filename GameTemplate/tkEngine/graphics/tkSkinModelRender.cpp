/*!
 * @brief	スキンモデルレンダラー
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelRender.h"


namespace tkEngine{
namespace prefab{

	void CSkinModelRender::Init(
		const wchar_t* filePath,
		CAnimationClip* animationClips,
		int numAnimationClips ,
		EnFbxUpAxis fbxUpAxis,
		int maxInstance )
	{
		m_enFbxUpAxis = fbxUpAxis;
		m_skinModelData.Load(filePath);
		m_skinModel.Init(m_skinModelData, maxInstance);
		m_frustumCulling.Init(MainCamera());
		InitAnimation(animationClips, numAnimationClips);
	}
	void CSkinModelRender::InitAnimation(CAnimationClip* animationClips, int numAnimationClips)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) {
			m_animation.Init(m_skinModel, m_animationClips, m_numAnimationClips);
		}
	}
	void CSkinModelRender::OnDestroy()
	{
		auto& shadowMap = GraphicsEngine().GetShadowMap();
		shadowMap.Remove(&m_skinModel.GetShadowCaster());
		auto& gBuffer = GraphicsEngine().GetGBufferRender();
		gBuffer.RemoveSkinModel(&m_skinModel);
	}
	/*!
	 * @brief	更新前に呼ばれる関数。
	 */
	bool CSkinModelRender::Start() 
	{
		return true;
	}
	/*!
	 * @brief	更新。
	 */
	void CSkinModelRender::Update()
	{
		if (m_skinModelData.IsAvailable() == false) {
			//m_skinModelDataが利用不可。
			//多分初期化の時にエラーが出ているはずです。エラーを直してください。
			return;
		}
		if (m_isFrustumCulling == true) {
			m_skinModel.UpdateBoundingBox();
			m_frustumCulling.Execute(m_skinModel.GetBoundingBox());
		}
		
		m_skinModel.Update(m_position, m_rotation, m_scale, m_enFbxUpAxis, m_isForwardRender);
	}
	/*!
	 * @brief	描画。
	 */
	void CSkinModelRender::ForwardRender(CRenderContext& rc)
	{
		if (m_skinModelData.IsAvailable() == false) {
			//m_skinModelDataが利用不可。
			//多分初期化の時にエラーが出ているはずです。エラーを直してください。
			return;
		}
		if (m_isForwardRender == false
			|| (m_isFrustumCulling == true
				&& m_frustumCulling.IsCulling())
			) {
			return;
		}
		m_skinModel.Draw(rc); 
	}
	void CSkinModelRender::FindVertexPosition(std::function<void(CVector3* pos)> func)
	{
		m_skinModelData.FindMesh([&](const auto& mesh) {
			ID3D11DeviceContext* deviceContext = GraphicsEngine().GetD3DImmediateDeviceContext();
			//頂点バッファを作成。
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					TK_WARNING("Failed map vertexBuffer");
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->vertexBuffer->GetDesc(&bufferDesc);
				int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
				char* pData = reinterpret_cast<char*>(subresource.pData);
				CVector3* pos;
				for (int i = 0; i < vertexCount; i++) {
					pos = reinterpret_cast<CVector3*>(pData);
					func(pos);
					pData += mesh->vertexStride;
				}
				//頂点バッファをアンロック
				deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			}
		});
	}

}
}