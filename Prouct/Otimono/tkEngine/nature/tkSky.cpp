#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/nature/tkSky.h"

namespace tkEngine {
	namespace prefab {
		CSky::CSky()
		{
		}


		CSky::~CSky()
		{
			DeleteGO(m_skinModelRender);
		}

		bool CSky::Start()
		{
			m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
			m_skinModelRender->Init(L"modelData/sky.cmo");
			//空は特殊なレンダリングを行うので、Forwardレンダリングの描画パスで描画する。
			m_skinModelRender->SetForwardRenderFlag(true);
			m_skyCube.CreateFromDDSTextureFromFile(L"modelData/preset/skyCubeMap.dds");
			//参照カウンタを上げておかないと、解放済みのテクスチャにアクセスしちゃう。
			m_skyCube.AddRef();
			m_psSkyShader.Load("shader/model.fx", "PSMain_SkyCube", CShader::EnType::PS);
			//マテリアルにクエリをかけて、シェーダーとテクスチャを差し替えるぜ。
			m_skinModelRender->FindMaterial([&](CModelEffect* mat) {
				mat->SetRender3DModelPSShader(m_psSkyShader);
				mat->SetDiffuseTexture(m_skyCube.GetBody());
			});
			return true;
		}
		void CSky::Update()
		{
			if (m_isDirty == true) {
				//更新の必要あり。
				m_skinModelRender->SetScale(m_scale);
				m_skinModelRender->SetEmissionColor(m_emissionColor);
				m_isDirty = false;
			}
		}
	}
}