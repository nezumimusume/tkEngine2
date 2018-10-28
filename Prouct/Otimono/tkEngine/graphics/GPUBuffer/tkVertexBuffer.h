/*!
 * @brief	頂点バッファ
 */

#pragma once

namespace tkEngine{
	/*!
	 * @brief	頂点バッファ。
	 */
	class CVertexBuffer : Noncopyable{
	public:
		CVertexBuffer();
		~CVertexBuffer();
		/*!
		 * @brief	頂点バッファの作成。
		 *@param[in]	numVertex			頂点数。
		 *@param[in]	stride				頂点ストライド。
		 *@param[in]	pSrcVertexBuffer	ソース頂点バッファ。
		 */
		bool Create( int numVertex, int stride, const void* pSrcVertexBuffer );
		/*!
		*@brief	開放。
		*@details
		* この関数はデストラクタからもコールされています。
		* 明示的なタイミングで開放したい場合に呼び出してください。
		*/
		void Release();
		/*!
		*@brief	頂点ストライドを取得。
		*/
		int GetStride() const
		{
			return m_stride;
		}
		/*!
		*@brief ID3D11Buffer*を取得。
		*/
		ID3D11Buffer*& GetBody() 
		{
			return m_vertexBuffer;
		}
	private:
		ID3D11Buffer*	m_vertexBuffer = nullptr;	//!<頂点バッファ。
		int				m_stride = 0;				//!<頂点ストライド。
	};
}