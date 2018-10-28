/*!
 * @brief	テクスチャデータ。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkTextureData.h"


namespace tkEngine{
	/*!
	 *@brief	テクスチャを読み込み。
	 */
	bool CTextureData::Load(const LPCWSTR fileName)
	{
		const wchar_t* ext = wcsstr(fileName, L".");
		HRESULT hr = S_OK;
		if (wcscmp(ext, L".dds") == 0){
			//DDS。
			 hr = DirectX::LoadFromDDSFile(fileName, 0, &m_texMetadata, m_scratchImage);
		}
		else if (wcscmp(ext, L".tga") == 0) {
			//TGA。
			hr = DirectX::LoadFromTGAFile(fileName, &m_texMetadata, m_scratchImage);
		}
		else {
			//それ以外。
			hr = DirectX::LoadFromWICFile(fileName, 0, &m_texMetadata, m_scratchImage);
		}
		
		if (FAILED(hr)) {
			//読み込み失敗。
			char mFileName[256];
			wcstombs(mFileName, fileName, sizeof(mFileName)-1);
			static char errorMessage[1024];
			sprintf(errorMessage, "テクスチャの読み込みに失敗しました。%s\n", mFileName);
			TK_LOG("errorMessage");
			return false;
		}

		return true;
	}
	/*!
	*@brief	テクスチャを保存。
	*/
	bool CTextureData::Save(const LPCWSTR fileName, size_t mip, size_t item, size_t slice)
	{
		const wchar_t* ext = wcsstr(fileName, L".");
		HRESULT hr = S_OK;
		if (wcscmp(ext, L".dds") == 0) {
			//DDS。
			hr = DirectX::SaveToDDSFile(*m_scratchImage.GetImage(mip, item, slice), 0, fileName );
		}
		else if (wcscmp(ext, L".tga") == 0) {
			//TGA。
			hr = DirectX::SaveToTGAFile(*m_scratchImage.GetImage(mip, item, slice), fileName);
		}
		else if(wcscmp(ext, L".png") == 0){
			//png。
			hr = DirectX::SaveToWICFile(*m_scratchImage.GetImage(mip, item, slice), 0, GUID_ContainerFormatPng, fileName);
		}
		else if (wcscmp(ext, L".jpg") == 0) {
			//jpg。
			hr = DirectX::SaveToWICFile(*m_scratchImage.GetImage(mip, item, slice), 0, GUID_ContainerFormatJpeg, fileName);
		}
		if (FAILED(hr)) {
			char mFileName[256];
			wcstombs(mFileName, fileName, sizeof(mFileName) - 1);
			static char errorMessage[1024];
			sprintf(errorMessage, "テクスチャの保存に失敗しました。%s\n", mFileName);
			TK_LOG("errorMessage");
			return false;
		}
		return true;
	}
}