#pragma once

#include "MapChip.h"

//レベル。
class Level
{
public:
	Level();
	~Level();
	/*!
	*@brief	レベルの構築。
	*@param[in]	levelDataFilePath	読み込むレベルデータのファイルパス。
	*/
	void Build(const wchar_t* levelDataFilePath);
	///////////////////////////////////////////////
	// ここからメンバ変数。
	///////////////////////////////////////////////
	std::list<MapChip*> m_mapChipList;		//マップチップのリスト。
};

