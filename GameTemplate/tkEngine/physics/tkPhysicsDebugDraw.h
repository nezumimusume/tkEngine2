/*!
 * @brief	コリジョンなどのデバッグ表示処理。
 */

#pragma once
#include <array> 
namespace tkEngine{
	class CPhysicsDebugDraw : public btIDebugDraw{
		struct SConstantBuffer {
			CMatrix mView;
			CMatrix mProj;
		};
		int m_debugMode = btIDebugDraw::DBG_NoDebug;
		CRenderContext* m_renderContext = nullptr;
		CPrimitive m_primitive;
		CShader m_vs;	//頂点シェーダー。
		CShader m_ps;	//ピクセルシェーダー。
		CConstantBuffer m_cb;
		static const int VERTEX_MAX = 1000000;
		std::array<CVector4, VERTEX_MAX> m_vertexBuffer;
		
		int m_numLine = 0;
	public:
		void Init();
		
		void BeginDraw(CRenderContext& rc)
		{
			m_renderContext = &rc;
			m_numLine = 0;
		}
		
		void EndDraw();
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override
		{
		}
		void reportErrorWarning(const char* warningString) override
		{
		}
		void draw3dText(const btVector3& location, const char* textString) override
		{
		}
		void setDebugMode(int debugMode) override
		{
			m_debugMode = debugMode;
		}
		int	getDebugMode() const override
		{
			return m_debugMode;
		}
	};
}