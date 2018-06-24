#pragma once

class Player;

class PlayerSilhouette : public IGameObject
{
public:
	PlayerSilhouette();
	~PlayerSilhouette();
	bool Start() override;
	void Update() override;
	void ForwardRender(CRenderContext& rc) override;
private:
	CSkinModel* m_playerModel = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	CShader m_psShader;	//シルエット描画で使用するピクセルシェーダー。
	CShaderResourceView m_texture;	//シルエットで表示するテクスチャ
	Player* m_player = nullptr;		//プレイヤー。
};

