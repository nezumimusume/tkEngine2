/*!
 *@brief	インデックスバッファ
 */

#pragma once

namespace tkEngine{
	/*!
	 *@brief	インデックスバッファ。
	 */
	class CIndexBuffer{
	public:
		/*!
		 *@brief	インデックスの型。
		 */
		enum EnIndexType{
			enIndexType_16,		//!<16ビットインデックス。
			enIndexType_32,		//!<32ビットインデックス。
		};
		/*!
		 *@brief	コンストラクタ。
		 */
		CIndexBuffer();
		/*!
		 *@brief	デストラクタ。
		 */
		~CIndexBuffer();
		/*!
		 *@brief	インデックスバッファを作成。
		 *@param[in]	numIndex			インデックスの数。
		 *@param[in]	indexType			インデックスの型。
		 *@param[in]	pSrcIndexBuffer		ソースインデックスバッファ。
		 */
		bool Create( int numIndex, EnIndexType indexType, const void* pSrcIndexBuffer );
		/*!
		 *@brief	開放。
		 *@details
		 * この関数はデストラクタから呼ばれています。
		 * 明示的なタイミングで開放したい場合は呼び出してください。
		 */
		void Release();
		
		/*!
		*@brief	インデックスの型を取得。
		*/
		EnIndexType GetIndexType() const
		{
			return m_indexType;
		}
		ID3D11Buffer* GetBody() const
		{
			return m_indexBuffer;
		}
		/*!
		*@brief	インデックスの数を取得。
		*/
		unsigned int GetNumIndex() const
		{
			return m_numIndex;
		}
		/*!
		*@brief	インデックスバッファが作成されているか判定。
		*/
		bool IsCreated() const
		{
			return m_isCreated;
		}
	private:
		bool			m_isCreated = false;			//!<インデックスバッファが作成されている？
		ID3D11Buffer*	m_indexBuffer = nullptr;		//!<インデックスバッファ。
		EnIndexType		m_indexType = enIndexType_16;	//!<インデックスの型。
		unsigned int	m_numIndex = 0;					//!<インデックスの数。
	};
}