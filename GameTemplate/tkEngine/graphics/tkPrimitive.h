/*!
 *@brief	プリミティブ
 */

#pragma once

#include "tkEngine/graphics/GPUBuffer/tkVertexBuffer.h"
#include "tkEngine/graphics/GPUBuffer/tkIndexBuffer.h"

namespace tkEngine{
	/*!
	 *@brief	プリミティブ。
	 */
	class CPrimitive {
	public:
		/*!
		 *@brief	コンストラクタ。
		 */
		CPrimitive();
		/*!
		 *@brief	デストラクタ。
		 */
		~CPrimitive();
		/*!
		 *@brief	プリミティブ作成。
		 *@param[in]	topology			トポロジー。
		 *@param[in]	numVertex			頂点の数。
		 *@param[in]	vertexStride		頂点ストライド。
		 *@param[in]	pSrcVertexBuffer	ソースとなる頂点バッファ。
		 *@param[in]	numIndex			インデックスの数。
		 *@param[in]	indexType			インデックスの型。
		 *@param[in]	pSrcIndexBuffer		ソースとなるインデックスバッファ。
		 *@retval	true	作成成功
		 *@retval	false	作成失敗。
		 */
		bool Create(
			D3D_PRIMITIVE_TOPOLOGY topology,
			int numVertex,
			int vertexStride,
			void* pSrcVertexBuffer,
			int numIndex,
			CIndexBuffer::EnIndexType indexType,
			void* pSrcIndexBuffer
		);
		/*!
		 *@brief	明示的にリリース。
		 */
		void Release();
		/*!
		*@brief	描画。
		*/
		void Draw(CRenderContext& rc);
		/*!
		*@brief	描画。
		*/
		void Draw(CRenderContext& rc, int numVertex);
		/*!
		*@brief	頂点バッファを取得。
		*/
		CVertexBuffer& GetVertexBuffer() 
		{
			return m_vertexBuffer;
		}
	private:
		CVertexBuffer			m_vertexBuffer;		//!<頂点バッファ。
		CIndexBuffer			m_indexBuffer;		//!<インデックスバッファ。
		D3D_PRIMITIVE_TOPOLOGY	m_topology;			//!<トポロジー。
	};
}