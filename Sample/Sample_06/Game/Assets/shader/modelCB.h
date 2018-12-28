/*!
 * @brief	モデル用の定数バッファ。
 */
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 *@details
 * これを変更したらtkSkinModel.hのSVSConstantBufferも変更するように。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld			: packoffset(c0);
	float4x4 mView			: packoffset(c4);
	float4x4 mProj			: packoffset(c8);
	float3 emissionColor	: packoffset(c12);	//自己発光色。
	int isShadowReceiver	: packoffset(c12.w);	//シャドウレシーバー？
};

/*!
 * @brief	ライト用の定数バッファ。
 *@details
 * これを変更したらtkLightManager.hのSLightParamも変更するように。
 */
cbuffer lightCb : register(b1)
{
	float3 eyePos 			: packoffset(c0);		//カメラの位置。
	int numDirectionLight 	: packoffset(c0.w);		//ディレクションライトの数。
	float3 ambientLight		: packoffset(c1);		//アンビエントライト。
	int numPointLight		: packoffset(c1.w);		//ポイントライトの数。
	float4 screenParam		: packoffset(c2);		// スクリーンパラメータ(near, far, screenWidth, screenHeight)
	int numEmissionLight	: packoffset(c3);		//自己発光ライトの数。
};

cbuffer MaterialParamCb : register(b2){
	int hasNormalMap;			//!<法線マップある？
	int hasSpecularMap;			//!<スペキュラマップある？
	float anisotropic;			//異方性反射
	unsigned int materialID;	//!<マテリアルID。

};

#define NUM_SHADOW_MAP 3
/*!
 * @brief	シャドウマップ用の定数バッファ。
 */
cbuffer ShadowCb : register( b3 ){
	float4x4 mLVP[NUM_SHADOW_MAP];		//!<ライトビュープロジェクション行列。
	float4 texOffset[NUM_SHADOW_MAP];	//!<シャドウマップのサイズ。
	float4 depthOffset;
};

/*!
 * @brief	GBuffer用の定数バッファ。
 */
cbuffer GBufferCb : register( b4 ){
	int isPCFShadowMap;
};