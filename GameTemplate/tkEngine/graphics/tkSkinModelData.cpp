/*!
 *@brief	スキンモデルデータ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/graphics/animation/tkAnimationClip.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

namespace tkEngine{
	
	/*!
	 *@brief
	 *  エフェクトファクトリ。
	 */
	class CSkinModelEffectFactory : public DirectX::EffectFactory {
	public:
		CSkinModelEffectFactory(ID3D11Device* device) :
			EffectFactory(device) {}
		std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info,  ID3D11DeviceContext* deviceContext)override
		{
			std::shared_ptr<CModelEffect> effect;
			if (info.enableSkinning) {
				//スキニングあり。
				effect = std::make_shared<CSkinModelEffect>();
			}
			else {
				//スキニングなし。
				effect = std::make_shared<CNonSkinModelEffect>();
			}
			effect->SetMatrialName(info.name);
			if (info.diffuseTexture && *info.diffuseTexture)
			{
				try {
					ID3D11ShaderResourceView* texSRV;
					DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
					effect->SetDiffuseTexture(texSRV);
				}
				catch (std::exception& exception) {
					if (strcmp(exception.what(), "CreateTexture") == 0) {
						TK_WARNING_MESSAGE_BOX_W(L"3Dモデルに貼られているテクスチャの作成に失敗しました。\n%ls\n\n"
												 L"fbxファイルに貼られているテクスチャが、fbxファイルと同じ場所にあるか確認をお願いします。\n" 
												 L"テクスチャがあるのに、エラーが消えない場合はビルド/リビルドを試してみてください。\n", info.diffuseTexture);

					}
				}
			}
			return effect;
		}

		void __cdecl CreateTexture( const wchar_t* name,  ID3D11DeviceContext* deviceContext,  ID3D11ShaderResourceView** textureView) override
		{
			return DirectX::EffectFactory::CreateTexture(name, deviceContext, textureView);
		}
	};
///////////////////////////////////////////////////////////////////////////
// モデルエフェクト。
///////////////////////////////////////////////////////////////////////////
	void __cdecl CModelEffect::Apply(ID3D11DeviceContext* deviceContext) 
	{
		if (m_renderContext == nullptr) {
			TK_WARNING("m_renderContext is nullptr");
			deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
		}
		else {
			EnRenderStep renderStep = m_renderContext->GetRenderStep();
			switch (renderStep)
			{
			case enRenderStep_ForwardRender:
				if (m_numInstance == 1) {
					//通常描画。
					deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
				}
				else {
					//インスタンス描画。
					deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShaderInstancing.GetBody(), NULL, 0);
				}
				deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
				break;
			case enRenderStep_RenderToShadowMap:
			case enRenderStep_ZPrepass:
				if (m_numInstance == 1) {
					deviceContext->VSSetShader((ID3D11VertexShader*)m_vsRenderToDepthShader.GetBody(), NULL, 0);
				}
				else {
					deviceContext->VSSetShader((ID3D11VertexShader*)m_vsRenderToDepthShaderInstancing.GetBody(), NULL, 0);
				}
				deviceContext->PSSetShader((ID3D11PixelShader*)m_psRenderToDepthShader.GetBody(), NULL, 0);
				break;
			case enRenderStep_RenderGBuffer:
				if (m_numInstance == 1) {
					//通常描画。
					deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
				}
				else {
					//インスタンス描画。
					deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShaderInstancing.GetBody(), NULL, 0);
				}
				deviceContext->PSSetShader((ID3D11PixelShader*)m_psRenderToGBufferShader.GetBody(), NULL, 0);
				
				break;
			default:
				break;
			}
		}
		
		deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_diffuseTex);
		m_materialParam.hasNormalMap = 0;
		if (m_normalMap != nullptr) {
			deviceContext->PSSetShaderResources(enSkinModelSRVReg_NormalMap, 1, &m_normalMap);
			m_materialParam.hasNormalMap = 1;
		}
		m_materialParam.hasSpecularMap = 0;
		if (m_specularMap != nullptr) {
			deviceContext->PSSetShaderResources(enSKinModelSRVReg_Specularmap, 1, &m_specularMap);
			m_materialParam.hasSpecularMap = 1;
		}
		deviceContext->UpdateSubresource(m_materialParamCB.GetBody(), 0, NULL, &m_materialParam, 0, 0);
		deviceContext->PSSetConstantBuffers(enSkinModelCBReg_Material, 1, &m_materialParamCB.GetBody());

	}
	CSkinModelData::CSkinModelData()
	{
	}
	CSkinModelData::~CSkinModelData()
	{
	}
	
	bool CSkinModelData::Load(const wchar_t* filePath)
	{
		m_filePath = filePath;
		m_isAvailable = false;
		CSkinModelEffectFactory effectFactory(GraphicsEngine().GetD3DDevice());
		wchar_t textureDir[256];
		wcsncpy_s(textureDir, filePath, 256);
		auto p = wcsrchr(textureDir, L'/');
		if (p == nullptr) {
			//\マークを試す。
			p = wcsrchr(textureDir, L'\\');
			if (p == nullptr) {
				//ファイルパスおかしくね？
				return false;
			}
		}
		*p = '\0';
		effectFactory.SetDirectory(textureDir);
		//スケルトンのデータを読み込み。
		std::wstring skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(L".cmo");
		skeletonFilePath.replace(pos, 4, L".tks");
		m_skeleton.Load(skeletonFilePath.c_str());

		//モデルデータをロード。
		m_modelDx = GraphicsEngine().GetDirectXModelResource().Load(filePath, m_skeleton);
		if (m_modelDx != nullptr) {
			//インスタンスが利用可能。
			m_isAvailable = true;
		}
		return true;
	}
	void CSkinModelData::FindMesh(OnFindMesh findMesh) const
	{
		if (m_modelDx != nullptr) {
			for (auto& modelMeshs : m_modelDx->meshes) {
				for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
					findMesh(mesh);
				}
			}
		}
	}
}

