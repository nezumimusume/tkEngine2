#pragma once
class Star : public IGameObject
{
public:
	Star();
	~Star();
	void Update() ;
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position = CVector3::Zero;
	CQuaternion m_rotation = CQuaternion::Identity;
};

