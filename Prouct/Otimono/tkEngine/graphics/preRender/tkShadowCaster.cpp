/*!
 * @brief	シャドウキャスター。
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/preRender/tkShadowCaster.h"

namespace tkEngine{

////////////////////////////////////////////////////////////////
// シャドウキャスター(スキンモデル版)
////////////////////////////////////////////////////////////////
	CShadowCaster_SkinModel::CShadowCaster_SkinModel() :
		m_skinModel(nullptr)
	{
	}
	CShadowCaster_SkinModel::~CShadowCaster_SkinModel()
	{
		//シャドウマップに登録されているかもしれないので、削除する。
		GraphicsEngine().GetShadowMap().Remove(this);
	}
	void CShadowCaster_SkinModel::Create(CSkinModel* model)
	{
		m_skinModel = model;
	}
	void CShadowCaster_SkinModel::Render(CRenderContext& renderContext, const CMatrix& mLVP)
	{
		if (!m_skinModel) {
			return;
		}
		m_skinModel->Draw(renderContext, mLVP, CMatrix::Identity);
	}
}