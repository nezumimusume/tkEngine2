/*!
 *@brief	ライトカリング
 *@details
 * TBFRのための動的光源のタイルベースカリングを行う。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkLightCulling.h"

namespace tkEngine{
	/*!
	 *@brief	コンストラクタ。
	 */
	CLightCulling::CLightCulling()
	{
	}
	/*!
	 *@brief	デストラクタ。
	 */
	CLightCulling::~CLightCulling()
	{
	}
	/*!
	*@brief	初期化。
	*/
	void CLightCulling::Init()
	{
		m_csLightCulling.Load("shader/LightCulling.fx", "CSMain", CShader::EnType::CS);
		//カメラ用の定数バッファを作成。
		m_cameraParamCB.Create(NULL, sizeof(m_cameraParam));
	}
	/*!
	 *@brief	ライトカリングの実行。
	 */
	void CLightCulling::Render(CRenderContext& rc)
	{
		BeginGPUEvent(L"enRenderStep_LightCulling");
		rc.SetRenderStep(enRenderStep_LightCulling);
		CGraphicsEngine& ge = GraphicsEngine();
		rc.CSSetShader(m_csLightCulling);
		//深度テクスチャを設定。
		rc.CSSetShaderResource(0, GetGBufferSRV(enGBufferDepth));
		//ポイントライトのリストを設定。
		rc.CSSetShaderResource(1, ge.GetLightManager().GetPointLightsSRV());
		//出力先を設定。
		rc.CSSetUnorderedAccessView(0, ge.GetLightManager().GetPointLightsInTileUAV());

		//カメラ定数バッファの設定。
		CCamera& cam = MainCamera();
		m_cameraParam.screenParam.x = cam.GetNear();
		m_cameraParam.screenParam.y = cam.GetFar();
		m_cameraParam.screenParam.z = (float)ge.GetFrameBufferWidth();
		m_cameraParam.screenParam.w = (float)ge.GetFrameBufferHeight();
		m_cameraParam.mProj = cam.GetProjectionMatrix();
		m_cameraParam.mProjInv.Inverse(m_cameraParam.mProj);
		m_cameraParam.mViewRot.Inverse(cam.GetViewMatrix());
		m_cameraParam.mViewRot.m[3][0] = 0.0f;
		m_cameraParam.mViewRot.m[3][1] = 0.0f;
		m_cameraParam.mViewRot.m[3][2] = 0.0f;
		m_cameraParam.mViewRot.Transpose();
		m_cameraParam.numPointLight = ge.GetLightManager().GetNumPointLight();

		rc.UpdateSubresource(m_cameraParamCB, &m_cameraParam);
		rc.CSSetConstantBuffer(0, m_cameraParamCB);

		rc.Dispatch(ge.GetFrameBufferWidth() / TILE_WIDTH, ge.GetFrameBufferHeight() / TILE_WIDTH, 1);
		
		rc.CSUnsetUnorderedAccessView(0);
		rc.CSUnsetShaderResource(0);
		EndGPUEvent();
	}
}