#include "stdafx.h"
#include "Star.h"


Star::Star()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init(L"modelData/star/star.cmo");
}


Star::~Star()
{
}
void Star::Update()
{
	//Starに座標を伝える。
	m_skinModelRender->SetPosition(m_position);
	//毎フレームY軸周りに少しづつ回す。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY, 2.0f);
	//クォータニオンの乗算は回転の加算と同じ。
	//星の回転クォータニオンに毎フレームY軸周りに２度の回転を加算していく。
	m_rotation *= qAddRot;
	m_skinModelRender->SetRotation(m_rotation);
}