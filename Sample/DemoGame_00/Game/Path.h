#pragma once
/*!
 *@brief	エッジ
 */
class PathEdge {
public:
	CVector3 startPos;		//!<始点。
	CVector3 endPos;		//!<終点。
	CVector3 direction;		//!<方向。
	CVector3 directionXZ;	//!<XZ平面での方向。
	int no;
	/*!
	 *@brief	エッジまでの最短ベクトルを計算。
	 */
	CVector3 CalcVectorToEdge(const CVector3& point) const;
};
/*!
 *@brief	パス。
 */
class Path
{
public:

	Path();
	~Path();
	/*!
	 *@brief	パスデータをロード。
	 */
	void Load(const wchar_t* filePath);
	/*!
	 *@brief	パスのポイントの数を取得。
	 */
	int GetNumPoint() const
	{
		return static_cast<int>(m_pointList.size());
	}
	/*!
	 *@brief	最近棒のエッジを検索。
	 */
	const PathEdge* GetNearEdge(const CVector3& pos)const;
	/*!
	 *@brief	エッジの数を取得。
	 */
	int GetNumEdge() const
	{
		return m_edgeList.size();
	}
	/*!
	 *@brief	エッジを取得。
	 */
	const PathEdge* GetEdge(const int no) const
	{
		return &m_edgeList[no];
	}
private:
	
	std::vector<CVector3> m_pointList;	//!<座標のリスト。
	std::vector<PathEdge> m_edgeList;		//!<エッジのリスト。
};

