/*!
 * @brief	GPUバッファクラス。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	GPUBuffer
	 */
	class CGPUBuffer : Noncopyable{
	public:
		CGPUBuffer();
		virtual ~CGPUBuffer();
		/*!
		* @brief	GPUBufferを作成。
		*@param[in]	pInitData		初期データ。
		*@param[in]	bufferDesc		バッファ定義。
		*/
		bool Create(const void* pInitData, D3D11_BUFFER_DESC& bufferDesc);
		/*!
		 * @brief	ID3D11Buffer*を取得。
		 */
		ID3D11Buffer*& GetBody() 
		{
			return m_gpuBuffer;
		}
		/*!
		*@brief	開放。
		*@details
		* この関数はデストラクタからもコールされています。
		* 明示的なタイミングで開放したい場合に呼び出してください。
		*/
		void Release();
	private:
		ID3D11Buffer*		m_gpuBuffer = nullptr;	//!<GPUBuffer
	};
}