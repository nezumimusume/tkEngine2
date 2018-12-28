/*!
 * @brief	モデル関係の構造体ヘッダ。
 */
 
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float3 Tangent  : TANGENT;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR;
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord	: TEXCOORD0;
    float3 Tangent	: TANGENT;
    uint4  Indices  : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 Pos			: TEXCOORD1;		//ワールド座標系での頂点座標。
	float3 posInView	: TEXCOORD2;		//カメラ空間での座標。
	float4 posInProj	: TEXCOORD3;		//正規化座標系での座標。
};
/*!
 *@brief	Z値の書き込みのみを行うピクセルシェーダーへの入力。
 *@details
 * 現在はシャドウマップ作成とZPrepassで使用されています。
 */
struct PSInput_RenderToDepth{
	float4 Position 	: SV_POSITION;
	float4 posInProj	: TEXCOORD0;		//正規化座標系での座標。
};

/*!
 * @brief	G-Buffer描画用のピクセルシェーダーからの出力。
 */
struct PSOutput_RenderGBuffer{
	float4 albedo		: SV_Target0;		//アルベド
	float3 normal		: SV_Target1;		//法線。
	float2 spacular		: SV_Target2;		//スペキュラ。
	float shadow		: SV_Target3;		//シャドウ。
	float2 depth		: SV_Target4;		//深度値。
	float3 tangent		: SV_Target5;		//接ベクトル。
	float materialID	: SV_Target6;		//マテリアルID。
	float3 emissionColor : SV_Target7;		//自己発光色。
};

/*!
 * @brief	ディレクションライト。
 */
struct SDirectionLight{
	float4	color;			//!<ライトのカラー。
	float3	direction;		//!<ライトの方向。
	unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与える相手を識別するマテリアルIDグループ。0xFFFFFFFFですべてのオブジェクトにライトを当てる。
};

//ポイントライト。
struct SPointLight {
	float3	position;		//位置。
	float3	positionInView;	//カメラ座標系でのポイントライトの位置。
	float4	color;			//カラー。
	float4  attn;			//減衰定数。(xを小さくするとライトの明るさが増す、yとzが大きくなるとライトが遠くまで届くようになる。)
	unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与える相手を識別するマテリアルIDグループ。0xFFFFFFFFですべてのオブジェクトにライトを当てる。
};
