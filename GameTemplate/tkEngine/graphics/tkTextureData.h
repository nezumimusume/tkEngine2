/*!
 * @brief	テクスチャデータ。
 */

#pragma once


namespace tkEngine{
	/*!
	 * @brief	テクスチャデータ。
	 */
	class CTextureData : Noncopyable{
	public:
		CTextureData()
		{
		}
		~CTextureData()
		{
		}
		/*!
		 *@brief	テクスチャを読み込み。
		 */
		bool Load(const LPCWSTR fileName);
		/*!
		*@brief	テクスチャを保存。
		*/
		bool Save(const LPCWSTR fileName, size_t mip, size_t item, size_t slice);
		/*!
		*@brief	ビットマップイメージを取得。
		*/
		const DirectX::Image* GetImage(size_t mip, size_t item, size_t slice) const
		{
			return m_scratchImage.GetImage(mip, item, slice);
		}
		/*!
		*@brief	ビットマップイメージを取得。
		*/
		const DirectX::Image* GetImages() const
		{
			return m_scratchImage.GetImages();
		}
		/*!
		*@brief	画像イメージの数を取得。
		*/
		int GetNumImages() const
		{
			return m_scratchImage.GetImageCount();
		}
		/*!
		*@brief	画像のメタデータを取得。
		*/
		const DirectX::TexMetadata& GetMetadata() const
		{
			return m_texMetadata;
		}
	private:
		DirectX::TexMetadata		m_texMetadata;		//!<テクスチャメタデータ。
		DirectX::ScratchImage		m_scratchImage;		//!<
	};
}