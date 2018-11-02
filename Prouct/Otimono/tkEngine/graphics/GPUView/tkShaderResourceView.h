/*!
 *@brief	ShaderResourceView
 */

#pragma once

namespace tkEngine{
	class CStructuredBuffer;
	class CTextureData;
	/*!
	 *@brief	ShaderResourceView
	 *@details
	 * テクスチャやストラクチャバッファなど、シェーダーで使用するリソースで使用するビュー。
	 */
	class CShaderResourceView : Noncopyable{
	public:
		CShaderResourceView();
		~CShaderResourceView();
		/*!
		 *@brief	StructuredBuffer用のSRVを作成。
		 *@param[in]	structuredBuffer	structuredBuffer
		 */
		bool Create(CStructuredBuffer& structuredBuffer);
		/*!
		*@brief	テクスチャ用のSRVを作成。
		*@param[in]	texture		テクスチャ。
		*/
		bool Create(ID3D11Texture2D* texture);
		/*!
		*@brief	DDSファイルからテクスチャ用のSRVを作成。
		*@param[in]	fileName		ファイル名。
		*/
		bool CreateFromDDSTextureFromFile(const wchar_t* fileName);
		/*!
		 *@brief	明示的な開放。
		 *@details
		 * 明示的に開放する必要があるときに使用してください。
		 */
		void Release();
		/// <summary>
		/// 参照カウンタを上げる。
		/// </summary>
		void AddRef()
		{
			if (m_srv != nullptr) {
				m_srv->AddRef();
			}
		}
		/*!
		*@brief	ID3D11ShaderResourceViewを取得。
		*/
		ID3D11ShaderResourceView*& GetBody()
		{
			return m_srv;
		}
		/*!
		*@brief	有効なビューかどうか判定。
		*/
		bool IsValid() const
		{
			return m_isValid;
		}
		
		/*!
		*@brief SRVに関連付けされているテクスチャの情報を取得する。
		*/
		void GetTextureDesc(D3D11_TEXTURE2D_DESC& desc) const
		{
			ID3D11Texture2D* tex;
			m_srv->GetResource((ID3D11Resource**)&tex);
			tex->GetDesc(&desc);
			tex->Release();
		}
	private:
		ID3D11ShaderResourceView*	m_srv = nullptr;
		bool						m_isValid = false;		//!<有効なデータ？
	};
}