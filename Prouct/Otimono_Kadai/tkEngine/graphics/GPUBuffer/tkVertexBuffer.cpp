/*!
 * @brief	頂点バッファ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/GPUBuffer/tkVertexBuffer.h"
#include "tkEngine/tkEngine.h"

namespace tkEngine{
	CVertexBuffer::CVertexBuffer()
	{
	}
	CVertexBuffer::~CVertexBuffer()
	{
		Release();
	}
	bool CVertexBuffer::Create( int numVertex, int stride, const void* pSrcVertexBuffer )
	{
		Release();
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = stride * numVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = pSrcVertexBuffer;

		HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
		if (FAILED(hr)) {
			return false;
		}
		m_stride = stride;
		return true;
	}
	void CVertexBuffer::Release()
	{
		if (m_vertexBuffer != nullptr) {
			m_vertexBuffer->Release();
			m_vertexBuffer = nullptr;
		}
	}
}