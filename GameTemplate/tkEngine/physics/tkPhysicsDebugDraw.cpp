/*!
 * @brief	コリジョンなどのデバッグ表示処理。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/physics/tkPhysicsDebugDraw.h"


namespace tkEngine {
	void CPhysicsDebugDraw::Init() 
	{
		
		//ラインストリップのテスト。
		m_primitive.Create(
			D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			static_cast<int>(m_vertexBuffer.size()),
			sizeof(CVector4),
			&m_vertexBuffer[0],
			0,
			CIndexBuffer::enIndexType_32,
			nullptr
		);
		m_vs.Load( "shader/linePrimitive.fx", "VSMain", CShader::EnType::VS );
		m_ps.Load( "shader/linePrimitive.fx", "PSMain", CShader::EnType::PS );
		m_cb.Create(nullptr, sizeof(SConstantBuffer));
	}
	void CPhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		int baseIndex = m_numLine * 2;
		m_vertexBuffer[baseIndex].Set( CVector3(from.x(), from.y(), from.z()) );
		m_vertexBuffer[baseIndex+1].Set(CVector3(to.x(), to.y(), to.z()));
		m_numLine++;
		
	}
	void CPhysicsDebugDraw::EndDraw()
	{
		//定数バッファの更新。
		SConstantBuffer cb;
		cb.mView = MainCamera().GetViewMatrix();
		cb.mProj = MainCamera().GetProjectionMatrix();
		m_renderContext->UpdateSubresource(m_cb, &cb);

		m_renderContext->VSSetShader(m_vs);
		m_renderContext->PSSetShader(m_ps);
		m_renderContext->VSSetConstantBuffer(0, m_cb);
		m_renderContext->UpdateSubresource(m_primitive.GetVertexBuffer(), &m_vertexBuffer[0]);
		m_primitive.Draw(*m_renderContext, m_numLine*2);
	}
}

