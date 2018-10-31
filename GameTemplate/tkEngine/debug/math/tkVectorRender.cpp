/*!
 * @brief	ベクトルレンダラー
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/debug/math/tkVectorRender.h"

namespace tkEngine{
	namespace prefab {
		static const int MAX_RENDER = 1000;	//最大で1000個のベクトルを描画できる。
		bool CVectorRender::Start()
		{
			m_modelData.Load(L"modelData/preset/vector.cmo");
			m_model.Init(m_modelData, MAX_RENDER);
			m_model.SetShadowCasterFlag(true);
			return true;
		}
		bool CVectorRender::IsRenderVector(const SRenderData& renderData) const
		{
			if (m_renderVectorNameList.empty() == true) {
				//描画するベクトルの名前が指定されていない。
				return true;
			}
			//空ではない。
			auto it = std::find(
				m_renderVectorNameList.begin(),
				m_renderVectorNameList.end(),
				renderData.name
			);
			//終端まで行かなかった。
			return it != m_renderVectorNameList.end();
		}
		void CVectorRender::ForwardRender(CRenderContext& rc)
		{
			if (m_isRender == true) {
				m_model.BeginUpdateInstancingData();
				for (const auto& renderData : m_renderData) {
					if (IsRenderVector(renderData) == false) {
						//このベクトルは描画しない。
						continue;
					}
					
					CQuaternion rot;
					rot.SetRotation(CVector3::AxisZ, renderData.vector);
					float s = renderData.vector.Length();
					m_model.UpdateInstancingData(
						renderData.origin,
						rot,
						{ s * 0.2f, s * 0.2f, s }
					);
				}
				m_model.EndUpdateInstancingData(true);

				m_model.Draw(rc);
			}
		}
		void CVectorRender::PostRender(CRenderContext& rc)
		{
			if (m_isRender == true) {
				m_font.SetShadowParam(true, 1.0f, CVector4::Black);
				m_font.Begin(rc);
				for (const auto& renderData : m_renderData) {
					if (IsRenderVector(renderData) == false) {
						//このベクトルは描画しない。
						continue;
					}
					CVector4 pos;
					pos.Set(renderData.origin);
					const auto& vpMatrix = MainCamera().GetViewProjectionMatrix();
					vpMatrix.Apply(pos);
					pos.x /= pos.w;
					pos.y /= pos.w;
					pos.z /= pos.w;
					
					//スクリーン空間に変換する。
					float half_w = GraphicsEngine().Get2DSpaceScreenWidth() * 0.5f;
					float half_h = GraphicsEngine().Get2DSpaceScreenHeight() * 0.5f;

					pos.x *= half_w;
					pos.y *= half_h;

					//ベクトルデータを表示。
					wchar_t text[256];
					swprintf_s(
						text,
						L"vec[ %.3f, %.3f, %.3f ]",
						renderData.vector.x,
						renderData.vector.y,
						renderData.vector.z
					);
					m_font.Draw(text, { pos.x, pos.y - 4.0f }, CVector4::White, 0.0f, 0.4f, { 0.0f, 1.0f });
					//向きデータを表示。
					auto dir = renderData.vector;
					dir.Normalize();
					text[256];
					swprintf_s(
						text,
						L"dir[ %.3f, %.3f, %.3f ]",
						dir.x,
						dir.y,
						dir.z
					);
					m_font.Draw(text, { pos.x, pos.y - 20.0f }, CVector4::White, 0.0f, 0.4f, { 0.0f, 1.0f });
					//スカラーを表示。
					text[256];
					swprintf_s(
						text,
						L"scalar[ %.3f ]",
						renderData.vector.Length()
					);
					m_font.Draw(text, { pos.x, pos.y - 36.0f }, CVector4::White, 0.0f, 0.4f, { 0.0f, 1.0f });
				}
				m_font.End(rc);
			}
			m_renderData.clear();
		}
	}
}