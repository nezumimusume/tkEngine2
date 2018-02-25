/*!
 *@brief	プリミティブ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkPrimitive.h"

namespace tkEngine{
	CPrimitive::CPrimitive()
	{
	}
	CPrimitive::~CPrimitive()
	{
		Release();
	}
	void CPrimitive::Release()
	{
		m_vertexBuffer.Release();
		m_indexBuffer.Release();
	}
	bool CPrimitive::Create(
		D3D_PRIMITIVE_TOPOLOGY topology,
		int numVertex,
		int vertexStride,
		void* pSrcVertexBuffer,
		int numIndex,
		CIndexBuffer::EnIndexType indexType,
		void* pSrcIndexBuffer)
	{
		Release();
		bool result = m_vertexBuffer.Create(numVertex, vertexStride, pSrcVertexBuffer );
		if (!result) {
			TK_WARNING("プリミティブの作成に失敗しました。");
			return false;
		}
		result = m_indexBuffer.Create(numIndex, indexType, pSrcIndexBuffer);
		if (!result) {
			TK_WARNING("プリミティブの作成に失敗しました。");
			return false;
		}

		return true;
	}
	void CPrimitive::Draw(CRenderContext& rc)
	{
		//頂点バッファを設定。
		rc.IASetVertexBuffer(m_vertexBuffer);
		rc.IASetIndexBuffer(m_indexBuffer);
		//プリミティブのトポロジーを設定。
		rc.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//描画。
		rc.DrawIndexed(m_indexBuffer.GetNumIndex(), 0, 0);
	}
}