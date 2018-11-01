/*!
 * @brief	StructuredBuffer
 */

#pragma once

#include "tkEngine/graphics/GPUView/tkShaderResourceView.h"
#include "tkEngine/graphics/GPUView/tkUnorderedAccessView.h"
namespace tkEngine{
	/*!
	 * @brief	StructuredBuffer
	 */
	class CStructuredBuffer : Noncopyable{
	public:
		CStructuredBuffer();
		~CStructuredBuffer();
		/*!
		* @brief	StructuredBufferを作成。
		*@param[in]	pInitData		初期データ。
		*@param[in]	bufferDesc		バッファ定義。
		*/
		bool Create(const void* pInitData, D3D11_BUFFER_DESC& bufferDesc);
		/*!
		 * @brief	ID3D11Buffer*を取得。
		 */
		ID3D11Buffer*& GetBody() 
		{
			return m_structuredBuffer;
		}
		/*!
		*@brief	開放。
		*@details
		* この関数はデストラクタからもコールされています。
		* 明示的なタイミングで開放したい場合に呼び出してください。
		*/
		void Release();
		/*!
		* @brief	SRVを取得。
		*/
		CShaderResourceView& GetSRV()
		{
			return m_srv;
		}
		/*!
		* @brief	UAVを取得。
		*/
		CUnorderedAccessView& GetUAV()
		{
			return m_uav;
		}
	private:
		ID3D11Buffer*			m_structuredBuffer = nullptr;	//!<StructuredBuffer
		CShaderResourceView		m_srv;							//!<SRV
		CUnorderedAccessView	m_uav;							//!<UAV
	};
}