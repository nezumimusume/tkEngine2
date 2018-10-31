#pragma once
class Sky final : public IGameObject
{
public:
	Sky();
	~Sky();
	bool Start() override ;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	
	CShaderResourceView m_skyCube;
	CShader m_psSkyShader;	//空用のシェーダー。
};

