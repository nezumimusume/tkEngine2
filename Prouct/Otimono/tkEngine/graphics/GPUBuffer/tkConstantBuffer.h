/*!
 * @brief	定数バッファ
 */

#pragma once

#include "tkEngine/graphics/GPUBuffer/tkGPUBuffer.h"

namespace tkEngine{
	/*!
	 * @brief	定数バッファ。
	 */
	class CConstantBuffer : public CGPUBuffer{
	public:
		CConstantBuffer();
		~CConstantBuffer();
		/*!
		*@brief ConstantBufferを作成。
		*@param[in]	pInitData		初期データ。
		*@param[in]	bufferSize		バッファサイズ。
		*/
		bool Create(const void* pInitData, int bufferSize );		
	};
}