/*!
 * @brief	スキンモデルレンダラー
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModelRender.h"


namespace tkEngine{
namespace prefab{

	void CSkinModelRender::Init(
		const wchar_t* filePath,
		CAnimationClip* animationClips,
		int numAnimationClips ,
		CSkinModel::EnFbxUpAxis fbxUpAxis)
	{
		m_enFbxUpAxis = fbxUpAxis;
		m_skinModelData.Load(filePath);
		m_skinModel.Init(m_skinModelData);
		m_frustumCulling.Init(MainCamera());
		InitAnimation(animationClips, numAnimationClips);
	}
	void CSkinModelRender::InitAnimation(CAnimationClip* animationClips, int numAnimationClips)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) {
			m_animation.Init(m_skinModel, m_animationClips, m_numAnimationClips);
		}
	}
	/*!
	 * @brief	更新前に呼ばれる関数。
	 */
	bool CSkinModelRender::Start() 
	{
		return true;
	}
	/*!
	 * @brief	更新。
	 */
	void CSkinModelRender::Update()
	{
		m_skinModel.Update(m_position, m_rotation, m_scale, m_enFbxUpAxis);
		if (m_isFrustumCulling == true) {
			m_skinModel.UpdateBoundingBox();
			m_frustumCulling.Execute(m_skinModel.GetBoundingBox());
		}
	}
	/*!
	 * @brief	描画。
	 */
	void CSkinModelRender::Render(CRenderContext& rc)
	{
		if (m_isFrustumCulling == true 
			&& m_frustumCulling.IsCulling()
			&& ( rc.GetRenderStep() == enRenderStep_Render3DModelToScene || rc.GetRenderStep() == enRenderStep_RenderGBuffer )
		) {
			//描画しないンゴ。
			return;
		}
		m_skinModel.Draw(rc);
	}
}
}