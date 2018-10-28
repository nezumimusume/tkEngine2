/*!
 *@brief	ShaderResourceView
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/GPUView/tkUnorderedAccessView.h"
#include "tkEngine/graphics/GPUBuffer/tkStructuredBuffer.h"
#include "tkEngine/tkEngine.h"


namespace tkEngine{
	CUnorderedAccessView::CUnorderedAccessView()
	{
	}
	CUnorderedAccessView::~CUnorderedAccessView()
	{
	}
	void CUnorderedAccessView::Release()
	{
		if(m_uav != nullptr)
		{
			m_uav->Release();
			m_uav = nullptr;
		}
	}
	bool CUnorderedAccessView::Create(CStructuredBuffer& structuredBuffer)
	{
		Release();
		ID3D11Buffer* pBuf = structuredBuffer.GetBody();
		if(pBuf != nullptr){
			D3D11_BUFFER_DESC descBuf;
	    	ZeroMemory( &descBuf, sizeof(descBuf) );
	   		pBuf->GetDesc( &descBuf );
	   		
	   		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		    ZeroMemory( &desc, sizeof(desc) );
		    desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		    desc.Buffer.FirstElement = 0;
		    
		    desc.Format = DXGI_FORMAT_UNKNOWN;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
	        desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride; 
	        
			HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateUnorderedAccessView(pBuf, &desc, &m_uav);
			if (FAILED(hr)) {
				return false;
			}
		}
		//有効な印。
		m_isValid = true;
		return true;
	}
	/*!
	*@brief	Texture用のUAVを作成。
	*@param[in]	texture		テクスチャ。
	*/
	bool CUnorderedAccessView::Create(ID3D11Texture2D* texture)
	{
		Release();
		if (texture != nullptr) {
			D3D11_TEXTURE2D_DESC texDesc;
			texture->GetDesc(&texDesc);
			D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
			ZeroMemory(&UAVDesc, sizeof(UAVDesc));
			UAVDesc.Buffer.FirstElement = 0;
			UAVDesc.Buffer.NumElements = texDesc.Width * texDesc.Height;
			UAVDesc.Buffer.Flags = 0;
			UAVDesc.Format = texDesc.Format;
			UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

			HRESULT hr = GraphicsEngine().GetD3DDevice()->CreateUnorderedAccessView(texture, &UAVDesc, &m_uav);
			if (FAILED(hr)) {
				return false;
			}
		}
		//有効な印。
		m_isValid = true;
		return true;
	}
}