/*!
 *@brief	スキンモデル。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModel.h"
#include "tkEngine/graphics/tkSkinModelData.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

namespace tkEngine{
	CSkinModel::CSkinModel()
	{
	}
	CSkinModel::~CSkinModel()
	{
	}
	void CSkinModel::InitBoudingBox()
	{
		m_skinModelData->FindMesh([&](auto& mesh) {
			auto vMax = CVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			auto vMin = CVector3(FLT_MAX, FLT_MAX, FLT_MAX);
			auto deviceContext = GraphicsEngine().GetD3DImmediateDeviceContext();
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				auto hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					TK_WARNING("Failed map vertexBuffer");
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->vertexBuffer->GetDesc(&bufferDesc);
				auto vertexCount = (int)(bufferDesc.ByteWidth / mesh->vertexStride);
				auto pData = reinterpret_cast<char*>(subresource.pData);
				for (int i = 0; i < vertexCount; i++) {
					auto pos = *reinterpret_cast<CVector3*>(pData);
					vMax.Max(pos);
					vMin.Min(pos);
					//次の頂点へ。
					pData += mesh->vertexStride;
				}
				//頂点バッファをアンロック
				deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			}

			auto halfSize = (vMax - vMin) * 0.5f;
			m_boundingBox.Init(halfSize);
		});
	}
	void CSkinModel::Init(CSkinModelData& modelData, int maxInstance)
	{
		m_maxInstance = maxInstance;
		m_skinModelData = &modelData;
		m_cb.Create(NULL, sizeof(SVSConstantBuffer));
		m_shadowCaster.Create(this);

		if (maxInstance > 1) {
			//インスタンシング用のデータを作成。
			m_instancingData.reset(new CMatrix[maxInstance]);
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	//SRVとしてバインド可能。
			desc.ByteWidth = sizeof(CMatrix) * maxInstance;
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.StructureByteStride = sizeof(CMatrix);
			m_instancingDataSB.Create(m_instancingData.get(), desc);
		}

		InitBoudingBox();
	}
	
	void CSkinModel::UpdateWorldMatrix(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, EnFbxUpAxis enUpdateAxis)
	{
		//3dsMaxと軸を合わせるためのバイアス。
		CMatrix mBias = CMatrix::Identity;
		if (enUpdateAxis == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(CMath::PI * -0.5f);
		}
		CMatrix mScale, mTrans, mRot;
		mScale.MakeScaling(scale);
		mRot.MakeRotationFromQuaternion(rot);
		mTrans.MakeTranslation(trans);


		m_worldMatrix = mBias * mScale;
		m_worldMatrix = m_worldMatrix * mRot;
		m_worldMatrix = m_worldMatrix * mTrans;
	}
	void CSkinModel::Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, EnFbxUpAxis enUpdateAxis, bool isForwardRender)
	{
		if (m_maxInstance > 1 || m_skinModelData == nullptr) {
			//インスタンシング描画はこの関数では更新しない。
			return;
		}
		UpdateWorldMatrix(trans, rot, scale, enUpdateAxis);
		if (isForwardRender == false) {
			GraphicsEngine().GetGBufferRender().AddSkinModel(this);
		}
		if (m_isShadowCaster == true) {
			GraphicsEngine().GetShadowMap().Entry(&m_shadowCaster);
		}
		m_numInstance = 1;
		//スケルトン更新。
		m_skinModelData->GetSkeleton().Update(m_worldMatrix);
	}
	
	void CSkinModel::UpdateInstancingData(
		const CVector3& trans,
		const CQuaternion& rot,
		const CVector3& scale,
		EnFbxUpAxis enUpdateAxis )
	{	

		UpdateWorldMatrix(trans, rot, scale, enUpdateAxis);
		if (m_numInstance < m_maxInstance) {
			//インスタンシングデータを更新する。
			m_instancingData[m_numInstance] = m_worldMatrix;
			m_numInstance++;
		}
		else {
			TK_WARNING("invalid UpdateInstancingData.");
		}
		if (m_animation != nullptr) {
			m_animation->Update(GameTime().GetFrameDeltaTime());
		}
		//スケルトン更新。
		m_skinModelData->GetSkeleton().Update(m_worldMatrix);

	}
	
	void CSkinModel::EndUpdateInstancingData(bool isForwardRender)
	{
		if( m_animation != nullptr) {
			m_animation->Update(GameTime().GetFrameDeltaTime());
		}
		if (m_maxInstance > 1) {
			if (isForwardRender == false) {
				GraphicsEngine().GetGBufferRender().AddSkinModel(this);
			}
			if (m_isShadowCaster == true) {
				GraphicsEngine().GetShadowMap().Entry(&m_shadowCaster);
			}
		}
	}
	
	void CSkinModel::Draw(CRenderContext& renderContext)
	{
		Draw(renderContext, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
	}
	void CSkinModel::Draw(
		CRenderContext& renderContext, 
		const CMatrix& viewMatrix, 
		const CMatrix& projMatrix
	)
	{
		
		(void)renderContext;

		if (m_skinModelData == nullptr
			|| m_skinModelData->IsAvailable() == false
			) {
			return;
		}
		
		if (m_maxInstance > 1) {
			//インスタンシング用のデータを更新。
			renderContext.UpdateSubresource(m_instancingDataSB, m_instancingData.get());
			renderContext.VSSetShaderResource(enSkinModelSRVReg_InstanceMatrix, m_instancingDataSB.GetSRV());
		}
		
		static DirectX::CommonStates state(GraphicsEngine().GetD3DDevice());
		//定数バッファを更新。
		SVSConstantBuffer vsCb;
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		vsCb.emissionColor = m_emissionColor;
		vsCb.isShadowReceiver = m_isShadowReceiver ? 1 : 0;
		
		renderContext.UpdateSubresource(m_cb, &vsCb);
		renderContext.VSSetConstantBuffer(enSkinModelCBReg_VSPS, m_cb);
		renderContext.PSSetConstantBuffer(enSkinModelCBReg_VSPS, m_cb);
		renderContext.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
		
		m_skinModelData->GetSkeleton().Render(renderContext);

		
		m_skinModelData->FindMesh([&](auto& mesh) {
			CModelEffect* effect = reinterpret_cast<CModelEffect*>(mesh->effect.get());
			//レンダリングコンテキストをエフェクトに設定する。
			effect->SetRenderContext(renderContext);
			//インスタンスの数を設定。
			if (m_numInstance > 0) {
				effect->SetNumInstance(m_numInstance);
			}
			else {
				effect->SetNumInstance(1);
			}
			
		});
		if (m_preDrawFookFunction != nullptr) {
			//フック関数が指定されている。
			m_preDrawFookFunction(renderContext, *this);
		}
	

		if (m_numInstance > 0) {
			m_skinModelData->GetBody().Draw(
				GraphicsEngine().GetD3DDeviceContext(),
				state,
				m_worldMatrix,
				viewMatrix,
				projMatrix,
				false,
				nullptr,
				m_numInstance > 1 ? m_numInstance : 1
			);
		}

		
		if (m_postDrawFookFunction != nullptr) {
			m_postDrawFookFunction(renderContext, *this);
		
		}
	}
}
