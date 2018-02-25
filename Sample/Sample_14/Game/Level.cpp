#include "stdafx.h"
#include "Level.h"
#include "MapChip.h"

Level::Level()
{
}


Level::~Level()
{
	for (auto& mapChip : m_mapChipList) {
		DeleteGO(mapChip);
	}
}
/*!
*@brief	レベルの作成。
*@param[in]	levelDataFilePath	読み込むレベルデータのファイルパス。
*/
void Level::Build(const wchar_t* levelDataFilePath)
{
	//今回は配置データとしてスケルトンを利用する。
	CLocData locData;
	locData.Load(levelDataFilePath);
	
	for (int i = 0; i < locData.GetNumObject(); i++) {
		
		//平行移動、回転を取得する。
		CVector3 position, scale;
		CQuaternion rotation;
		position = locData.GetObjectPosition(i);
		rotation = locData.GetObjectRotation(i);

		//ボーン名からモデルデータのファイルパスを作成する。
		const wchar_t* boneName = locData.GetObjectName(i);
		wchar_t modelDataFilePath[256];
		swprintf(modelDataFilePath, L"modelData/%s.cmo", boneName);
		MapChip* mapChip = NewGO<MapChip>(0);
		mapChip->Init(modelDataFilePath, position, CVector3::One, rotation);
		m_mapChipList.push_back(mapChip);
	}

}