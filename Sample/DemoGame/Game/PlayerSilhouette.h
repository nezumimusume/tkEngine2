#pragma once
class PlayerSilhouette : public IGameObject
{
public:
	PlayerSilhouette();
	~PlayerSilhouette();
	bool Start() override;
	void Update() override;
	void Render(CRenderContext& rc) override;
private:
	CSkinModel* m_playerModel = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	CShader m_psShader;	//シルエット描画で使用するピクセルシェーダー。
	CShaderResourceView m_texture;	//シルエットで表示するテクスチャ
};

