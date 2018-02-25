#include "stdafx.h"
#include "Path.h"


CVector3 PathEdge::CalcVectorToEdge(const CVector3& point) const
{
	//視点からposに向かって伸びるベクトルv0を計算。
	CVector3 v0 = point - startPos;
	//v0をエッジ上に射影したベクトルv1を計算する。
	CVector3 v1 = direction * v0.Dot(direction);
	//v0からエッジに対して落とした垂線を計算する。
	CVector3 v3 = v1 - v0;
	return v3;
}

Path::Path()
{
}


Path::~Path()
{
}



void Path::Load(const wchar_t* filePath)
{
	CSkeleton loc;
	if (!loc.Load(filePath)) {
		//パスを読み込めなかった。
		return;
	}
	m_pointList.resize(loc.GetNumBones() - 1);
	for (int i = 1; i < loc.GetNumBones(); i++) {
		CBone* bone = loc.GetBone(i);
		const CMatrix& mat = bone->GetBindPoseMatrix();
		CVector3 pos;
		pos.x = mat.m[3][0];
		pos.y = mat.m[3][2];
		pos.z = -mat.m[3][1];
		int pointNo = _wtoi(bone->GetName());
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		if (pointNo == 0) {
			TK_WARNING_MESSAGE_BOX("パスのノードのボーンの名前が不正です。");
			m_pointList.clear();
			return;
		}
#endif
		m_pointList[pointNo-1] = pos;
	}

	//エッジを構築。
	
	m_edgeList.reserve(m_pointList.size() - 1);
	for (int i = 0; i < m_pointList.size() - 1; i++) {
		PathEdge edge;
		edge.startPos = m_pointList[i];
		edge.endPos = m_pointList[i+1];
		edge.direction = edge.endPos - edge.startPos;
		edge.direction.Normalize();
		edge.directionXZ = edge.direction;
		edge.directionXZ.y = 0.0f;
		edge.directionXZ.Normalize();
		edge.no = m_edgeList.size();
		m_edgeList.push_back(edge);
	}
}
/*!
*@brief	最近棒のエッジを検索。
*/
const PathEdge* Path::GetNearEdge(const CVector3& pos) const
{
	float dist = FLT_MAX;
	const PathEdge* nearEdge = nullptr;
	for (auto& edge : m_edgeList) {
		//v0からエッジに対して落とした垂線を計算する。
		CVector3 v = edge.CalcVectorToEdge(pos);
		v.y = 0.0f;
		float distTmp = v.Length();
		if (dist > distTmp) {
			//こちらの方が近い。
			dist = distTmp;
			nearEdge = &edge;
		}

	}
	return nearEdge;
}