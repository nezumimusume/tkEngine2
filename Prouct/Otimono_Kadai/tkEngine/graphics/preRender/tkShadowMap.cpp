/*!
 * @brief	シャドウマップ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkShadowMap.h"
#include "tkEngine/graphics/preRender/tkShadowCaster.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"

namespace tkEngine{
	
	CShadowMap::CShadowMap()
	{
	}
	
	CShadowMap::~CShadowMap()
	{
	}
	
	bool CShadowMap::Init(const SShadowRenderConfig& config)
	{
		Release();
		m_lightHeight = config.lightHeight;
		m_isEnable = config.isEnable;
		m_far = m_lightHeight * 2.0f;
		m_near = m_lightHeight * 0.2f;
		if (config.farPlane > 0.0f) {
			m_far = config.farPlane;
		}
		if (config.nearPlane > 0.0f) {
			m_near = config.nearPlane;
		}
		m_softShadowLevel = config.softShadowLevel;

		m_shadowCbEntity.depthOffset[0] = config.depthOffset[0];
		m_shadowCbEntity.depthOffset[1] = config.depthOffset[1];
		m_shadowCbEntity.depthOffset[2] = config.depthOffset[2];
		if (m_isEnable == false) {
			return true;
		}
		int wh[][2] = {
			{ config.shadowMapWidth, config.shadowMapHeight},
			{ config.shadowMapWidth, config.shadowMapHeight >> 1},
			{ config.shadowMapWidth >> 1, config.shadowMapHeight >> 1},
		};
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		for(int i = 0; i < NUM_SHADOW_MAP; i++ ){
			m_shadowMapRT[i].Create(
				wh[i][0], 
				wh[i][1], 
				1, 
				1,
				DXGI_FORMAT_R32_FLOAT,
				DXGI_FORMAT_D32_FLOAT,
				multiSampleDesc
			);
			m_shadowCbEntity.texOffset[i].x = 1.0f / wh[i][0];
			m_shadowCbEntity.texOffset[i].y = 1.0f / wh[i][1];
		}

		m_shadowCb.Create(&m_shadowCbEntity, sizeof(m_shadowCbEntity));
		return true;
	}
	
	void CShadowMap::Release()
	{
		m_shadowCb.Release();
		for (auto& rt : m_shadowMapRT) {
			rt.Release();
		}
	}
	
	void CShadowMap::Entry(IShadowCaster* caster)
	{
		if (!m_isEnable) {
			return;
		}
		m_shadowCaster.push_back(caster);
	}
	
	void CShadowMap::Update()
	{
		if (!m_isEnable) {
			return;
		}
		//シーンをレンダリング使用としているカメラを使って、ライトカメラの回転を求める。
		CVector3 cameraDirXZ = MainCamera().GetForward();
		if (fabs(cameraDirXZ.x) < FLT_EPSILON && fabsf(cameraDirXZ.z) < FLT_EPSILON) {
			//ほぼ真上をむいている。
			return;
		}
		/*cameraDirXZ.y = 0.0f;
		cameraDirXZ.Normalize();*/
		//ライトビュー行列の回転成分をを計算する。
		CVector3 lightViewForward = m_lightDirection;
		CVector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//ほぼ真上。
			lightViewUp.Cross(lightViewForward, CVector3::Right);
		}
		else {
			lightViewUp.Cross(lightViewForward, CVector3::Up);
		}
		lightViewUp.Normalize();
		CVector3 lgihtViewRight;
		lgihtViewRight.Cross(lightViewUp, lightViewForward);
		lgihtViewRight.Normalize();
		
		CMatrix lightViewRot;
		//ライトビューの横を設定する。
		lightViewRot.m[0][0] = lgihtViewRight.x;
		lightViewRot.m[0][1] = lgihtViewRight.y;
		lightViewRot.m[0][2] = lgihtViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//ライトビューの上を設定する。
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//ライトビューの前を設定する。
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;

		float shadowAreaTbl[] = {
			m_lightHeight * 0.4f,
			m_lightHeight * 0.8f,
			m_lightHeight * 1.6f
		};

		//ライトビューのターゲットを計算。
		CVector3 lightTarget;
		lightTarget = MainCamera().GetPosition();
		lightTarget.y = MainCamera().GetTarget().y;
		lightTarget += cameraDirXZ ;
		CVector3 lightPos = lightTarget + m_lightDirection * -m_lightHeight;
		
		SShadowCb shadowCB;
		float nearPlaneZ = 0.0f;
		float farPlaneZ ;
		CVector3 cameraUp;
		cameraUp.Cross(MainCamera().GetRight(), MainCamera().GetForward());
		//視推台を分割するようにライトビュープロジェクション行列を計算する。
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			farPlaneZ = nearPlaneZ + shadowAreaTbl[i];
			CMatrix mLightView = CMatrix::Identity;
			float halfViewAngle = MainCamera().GetViewAngle() * 0.5f;
			//視推台の8頂点をライト空間に変換してAABBを求めて、正射影の幅と高さを求める。
			float w, h;
			
			CVector3 v[8];
			{
				float t = tan(halfViewAngle);
				CVector3 toUpperNear, toUpperFar;
				toUpperNear = cameraUp * t * nearPlaneZ;
				toUpperFar = cameraUp * t * farPlaneZ;
				t *= MainCamera().GetAspect();
				//近平面の中央座標を計算。
				CVector3 vWk = MainCamera().GetPosition() + cameraDirXZ * nearPlaneZ;
				lightPos = vWk;
				v[0] = vWk + MainCamera().GetRight() * t * nearPlaneZ + toUpperNear;
				v[1] = v[0] - toUpperNear * 2.0f;

				v[2] = vWk + MainCamera().GetRight() * -t * nearPlaneZ + toUpperNear;
				v[3] = v[2] - toUpperNear * 2.0f;

				//遠平面の中央座標を計算。
				vWk = MainCamera().GetPosition() + cameraDirXZ * farPlaneZ;
				lightPos += vWk;
				v[4] = vWk + MainCamera().GetRight() * t * farPlaneZ + toUpperFar;
				v[5] = v[4] - toUpperFar * 2.0f;
				v[6] = vWk + MainCamera().GetRight() * -t * farPlaneZ + toUpperFar;
				v[7] = v[6] - toUpperFar * 2.0f;

				//ライト行列を作成。
				lightPos *= 0.5f;
				lightPos += m_lightDirection * -m_lightHeight;
				
				mLightView = lightViewRot;
				mLightView.m[3][0] = lightPos.x;
				mLightView.m[3][1] = lightPos.y;
				mLightView.m[3][2] = lightPos.z;
				mLightView.m[3][3] = 1.0f;
				mLightView.Inverse(mLightView);	//ライトビュー完成。
				//視推台を構成する8頂点が計算できたので、ライト空間に座標変換して、AABBを求める。
				CVector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				CVector3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
				for (auto& vInLight : v) {
					mLightView.Apply(vInLight);
					vMax.Max(vInLight);
					vMin.Min(vInLight);
					
				}
				w = vMax.x - vMin.x;
				h = vMax.y - vMin.y;
			}
			CMatrix proj;
			proj.MakeOrthoProjectionMatrix(
				w,	//ちょい太らせる。
				h,
				m_near,
				m_far
			);
			m_LVPMatrix[i] = mLightView * proj;
			m_shadowCbEntity.mLVP[i] = m_LVPMatrix[i];
			
			nearPlaneZ = farPlaneZ;
		}
	}
	/*!
	*@brief	シャドウマップへ書き込み。
	*/
	void CShadowMap::RenderToShadowMap(CRenderContext& rc)
	{
		rc.SetRenderStep(enRenderStep_RenderToShadowMap);
		if (!m_isEnable) {
			return;
		}
		BeginGPUEvent(L"enRenderStep_RenderToShadowMap");
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			rc.PSUnsetShaderResource(enSkinModelSRVReg_ShadowMap_0 + i);
		}

		CRenderTarget* oldRenderTargets[MRT_MAX];
		unsigned int numRenderTargetViews;
		rc.OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			BeginGPUEvent(L"Render 3DModel");
			CRenderTarget* renderTargets[] = {
				&m_shadowMapRT[i]
			};

			rc.OMSetRenderTargets(1, renderTargets);
			rc.RSSetViewport(
				0.0f, 0.0f, 
				(float)m_shadowMapRT[i].GetWidth(),
				(float)m_shadowMapRT[i].GetHeight()
			);
			float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha
			rc.ClearRenderTargetView(0, ClearColor);

			for (auto& caster : m_shadowCaster) {
				caster->Render(rc, m_LVPMatrix[i]);
			}
			EndGPUEvent();

		}
		m_shadowCaster.clear();
		//レンダリングターゲットを差し戻す。
		rc.OMSetRenderTargets(numRenderTargetViews, oldRenderTargets);
		//@todo レンダリングステートはリストアする方法に変更する。
		rc.RSSetViewport(0.0f, 0.0f, (float)GraphicsEngine().GetFrameBufferWidth(), (float)GraphicsEngine().GetFrameBufferHeight());
		EndGPUEvent();
	}
	/*!
	*@brief	影を落とすためのパラメータをGPUに転送する。
	*/
	void CShadowMap::SendShadowReceiveParamToGPU(CRenderContext& rc)
	{
		rc.UpdateSubresource(m_shadowCb, &m_shadowCbEntity);
		rc.PSSetConstantBuffer(enSkinModelCBReg_Shadow, m_shadowCb);
		//テクスチャを転送。
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			rc.PSSetShaderResource(enSkinModelSRVReg_ShadowMap_0 + i, m_shadowMapRT[i].GetRenderTargetSRV());
		}
	}
}