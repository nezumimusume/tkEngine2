/*!
 * @brief	サンプラステート。
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	サンプラステート。
	 */
	class CSamplerState : Noncopyable{
	public:
		CSamplerState()
		{
		}
		~CSamplerState()
		{
			Release();
		}
		/*!
		 * @brief	サンプラステートを作成。
		 */
		bool Create(const D3D11_SAMPLER_DESC& desc);
		/*!
		 * @brief	明示的な開放処理。
		 */
		void Release()
		{
			if(m_samplerState != nullptr){
				m_samplerState->Release();
				m_samplerState = nullptr;
			}
		}
		/*!
		 * @brief	サンプラステートを取得。
		 */
		ID3D11SamplerState*& GetBody() 
		{
			return m_samplerState;
		}
	private:
		ID3D11SamplerState*		m_samplerState = nullptr;
	};
}