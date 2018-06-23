/*!
 * @brief	良く使うレンダリングステートをまとめたもの。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkPresetRenderState.h"


namespace tkEngine{
	ID3D11BlendState*	AlphaBlendState::disable = nullptr;	//!<アルファブレンディングが無効。
	ID3D11BlendState*	AlphaBlendState::trans = nullptr;	//!<半透明合成用のステート。
	ID3D11BlendState*	AlphaBlendState::add = nullptr;		//!<加算合成用のステート。
	
	void AlphaBlendState::Init(CGraphicsEngine& ge)
	{
		
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		ID3D11Device* pd3d = ge.GetD3DDevice();

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_BLUE | D3D11_COLOR_WRITE_ENABLE_GREEN;
		pd3d->CreateBlendState(&blendDesc, &add);

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		pd3d->CreateBlendState(&blendDesc, &trans);

		blendDesc.RenderTarget[0].BlendEnable = false;
		pd3d->CreateBlendState(&blendDesc, &disable);
	}

	ID3D11DepthStencilState* DepthStencilState::disable = nullptr;			//!<すべて無効。
	ID3D11DepthStencilState* DepthStencilState::SceneRender = nullptr;		//!<3Dモデルを描画する時の基本的な深度ステンシルステート。
	ID3D11DepthStencilState* DepthStencilState::spriteRender = nullptr;		//!<2D描画する時の基本的な深度ステンシルステート。
	ID3D11DepthStencilState* DepthStencilState::defferedRender = nullptr;	//!<ディファードレンダリングを行う基本的な深度ステンシルステート。
	void DepthStencilState::Init(CGraphicsEngine& ge)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		ID3D11Device* pd3d = ge.GetD3DDevice();
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		pd3d->CreateDepthStencilState(&desc, &SceneRender);

		desc.DepthEnable = false;
		pd3d->CreateDepthStencilState(&desc, &defferedRender);
		
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		pd3d->CreateDepthStencilState(&desc, &disable);
		pd3d->CreateDepthStencilState(&desc, &spriteRender);
	}
	ID3D11RasterizerState*	RasterizerState::sceneRender = nullptr;		//!<3Dモデルを描画する時の基本的なラスタライザステート。
	ID3D11RasterizerState*	RasterizerState::spriteRender = nullptr;	//!<2D描画する時の基本的なラスタライザステート。
	
	void RasterizerState::Init(CGraphicsEngine& ge)
	{
		D3D11_RASTERIZER_DESC desc = {}	;
		ID3D11Device* pd3d = ge.GetD3DDevice();
		desc.CullMode = D3D11_CULL_FRONT;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;

		pd3d->CreateRasterizerState(&desc, &sceneRender);
		pd3d->CreateRasterizerState(&desc, &spriteRender);

	}
}