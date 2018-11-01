/*!
 * @brief	Mapper
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	Mapper
	 *@details
	 * GPUBufferのメモリにアクセスするために使用して下さい。
	 */
	template<class TBuffer>
	class CMapper : Noncopyable {
		D3D11_MAPPED_SUBRESOURCE	m_mappedResource;		//!<MappedResource
		CRenderContext&				m_renderContext;		//!<
		TBuffer&					m_buffer;				//!<バッファ。
		
	public:
		CMapper(
			CRenderContext& renderContext, 
			TBuffer& buffer, 
			UINT subresource = 0, 
			D3D11_MAP mapType = D3D11_MAP_READ, 
			UINT mapFlags = 0
		) :
			m_renderContext(renderContext),
			m_buffer(buffer)
		{
			m_renderContext.Map(m_buffer, subresource, mapType, mapFlags, m_mappedResource);
		}
		~CMapper()
		{
			if (m_mappedResource.pData != nullptr) {
				m_renderContext.Unmap(m_buffer, 0);
			}
		}
		
		void* GetData()
		{
			return m_mappedResource.pData;
		}
	};
}