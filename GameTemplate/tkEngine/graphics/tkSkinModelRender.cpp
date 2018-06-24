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
		EnFbxUpAxis fbxUpAxis,
		int maxInstance )
	{
		m_enFbxUpAxis = fbxUpAxis;
		m_skinModelData.Load(filePath);
		m_skinModel.Init(m_skinModelData, maxInstance);
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
		if (m_isFrustumCulling == true) {
			m_skinModel.UpdateBoundingBox();
			m_frustumCulling.Execute(m_skinModel.GetBoundingBox());
		}
		
		m_skinModel.Update(m_position, m_rotation, m_scale, m_enFbxUpAxis, m_isForwardRender);
	}
	/*!
	 * @brief	描画。
	 */
	void CSkinModelRender::ForwardRender(CRenderContext& rc)
	{
		if (m_isForwardRender == false
			|| (m_isFrustumCulling == true
				&& m_frustumCulling.IsCulling())
			) {
			return;
		}
		m_skinModel.Draw(rc); 
	}
}
}