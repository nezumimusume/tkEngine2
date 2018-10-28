/*!
 * @brief	StructuredBuffer
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/GPUBuffer/tkStructuredBuffer.h"
#include "tkEngine/tkEngine.h"

namespace tkEngine{
	CStructuredBuffer::CStructuredBuffer()
	{
	}
	CStructuredBuffer::~CStructuredBuffer()
	{
	}
	void CStructuredBuffer::Release()
	{
		if (m_structuredBuffer != nullptr) {
			m_structuredBuffer->Release();
			m_structuredBuffer = nullptr;
		}
	}
	bool CStructuredBuffer::Create(const void* pInitData, D3D11_BUFFER_DESC& bufferDesc)
	{
		Release();		
		HRESULT hr;
		if (pInitData) {
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = pInitData;
			hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, &InitData, &m_structuredBuffer);
		}
		else {
			hr = GraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_structuredBuffer);
		}
		if (FAILED(hr)) {
			return false;
		}
		if (bufferDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
			m_srv.Create(*this);
		}
		if (bufferDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) {
			m_uav.Create(*this);
		}
		return true;
	}
}