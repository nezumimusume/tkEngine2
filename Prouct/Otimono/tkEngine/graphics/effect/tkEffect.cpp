/*!
 * @brief	エフェクト
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/effect/tkEffect.h"


namespace tkEngine {
	namespace prefab {
		CEffect::CEffect()
		{
		}
		CEffect::~CEffect()
		{
			Release();
		}

		void CEffect::Release()
		{
			if (m_handle != -1) {
				GraphicsEngine().GetEffectEngine().Stop(m_handle);
				m_handle = -1;
			}
		}
		
		void CEffect::Play(const wchar_t* filePath)
		{
			int nameKey = CUtil::MakeHash(filePath);
			CEffectEngine& ee = GraphicsEngine().GetEffectEngine();
			m_effect = ee.GetResourceManager().GetResource(nameKey);
			if (m_effect == nullptr) {
				//未登録。
				m_effect = ee.CreateEffekseerEffect(filePath);
				if (m_effect == nullptr) {
					TK_WARNING_MESSAGE_BOX("エフェクトのロードに失敗しました。%ls", filePath);
				}
				ee.GetResourceManager().RegistResource(nameKey, m_effect);
			}
			m_handle = GraphicsEngine().GetEffectEngine().Play(m_effect);
		}
		void CEffect::Update()
		{
			CMatrix mTrans, mRot, mScale, mBase;
			mTrans.MakeTranslation(m_position);
			mRot.MakeRotationFromQuaternion(m_rotation);
			mScale.MakeScaling(m_scale);
			mBase = mScale * mRot;
			mBase = mBase * mTrans;
			GraphicsEngine().GetEffectEngine().GetEffekseerManager().SetBaseMatrix(m_handle, mBase);
			if (IsPlay() == false) {
				//再生完了したら終わる。
				DeleteGO(this);
			}
		}
	}
}