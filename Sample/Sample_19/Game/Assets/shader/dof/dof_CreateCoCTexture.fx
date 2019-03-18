/*!
 *@brief	Dof(被写界深度)
 *@details
 * 被写界深度内のピクセルを表すマスク画像の作成と、各ピクセルのCoCの計算を行う。
 */

Texture2D<float4> sceneTexture : register(t0);	//シーンテクスチャ。
Texture2D<float4> depthTexture : register(t1);	//深度テクスチャ。yにカメラ空間から見た深度値が格納されている。

sampler pointSampler : register(s0);			//ポイントサンプリングを行うサンプラ。

/*!
 *@brief	ピクセルシェーダーへの入力。
 */
struct PSIn{
	float4 position : SV_Position;
	float2 uv		: TEXCOORD0;
};
/*!
 *@brief	ピクセルシェーダーからの出力。
 */
struct PSOutput{
	float4 colorAndCoc : SV_Target0;	//シーンのカラー情報とCoC。CoCはαチャンネルに格納されている。
};
/*!
 *@brief	定数バッファ。
 * この中身を変更したら、tkDof.hのSCreateCocParamCBも
 * 更新する。
 */
cbuffer cbParam : register( b0 )
{
	float4 dofRange;		//被写界深度の範囲を表すデータ。SetDofRangeParam関数で設定されるデータ。
							//xは手前ボケの減衰が開始するカメラからの距離。
							//yは手前ボケの減衰が終了するカメラからの距離。
							//zは奥ボケの増幅が開始するカメラからの距離。
							//wは奥ボケの増幅が終了するカメラからの距離。
};
/*!
 *@brief	頂点シェーダー。
 */
PSIn VSMain( float4 pos : POSITION, float2 uv : TEXCOORD0 )
{
	PSIn psIn = (PSIn)0;
	psIn.position = pos;
	psIn.uv = uv;

	return psIn;
}
/*!
 *@brief	ピクセルシェーダー。
 */
PSOutput PSMain( PSIn psIn )
{
	PSOutput psOutput = (PSOutput)0;
	//カメラ空間からの距離をサンプリング。
	float depth = depthTexture.Sample( pointSampler, psIn.uv ).y;
	//シーンのカラーをフェッチ。
	psOutput.colorAndCoc = sceneTexture.Sample( pointSampler, psIn.uv );
	
	//CoCを適当に計算する。超適当なので物理的な正しさは皆無！
	//ここで計算しているCoCは単純に手前ボケと奥ボケの範囲で、どのあたりにいるのかを表す
	//正規化された0.0～1.0の値です。
	//dofRange.w - dofRange.zとdofRange.y - dofRange.xの結果は、
	//必ず0以上になることをcpu側で保証しています。
	//そのため、0除算をシェーダー側で考慮する必要はありません。
	//詳細はCDof::SetDofRangeParam関数を参照。
	
	//まずは奥ボケ。
	float farCoc = max( 0.0f, depth - dofRange.z ) / ( dofRange.w - dofRange.z );
	//続いて手前ボケ。
	float nearCoc = max(0.0f, dofRange.y - depth ) / ( dofRange.y - dofRange.x );
	//大きいほうのcocを採用する。どちらか一方は必ず0になるね。
	float coc = max( farCoc, nearCoc );
	coc = min(1.0f, coc);
	//αにCoCを格納する！
	psOutput.colorAndCoc.a = coc;

	return psOutput;
}
