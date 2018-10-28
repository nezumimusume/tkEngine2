#pragma once
class Star : public IGameObject
{
public:
	Star();
	~Star();
	void Update() override;
private:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CVector3 m_position = CVector3::Zero;
	CQuaternion m_rotation = CQuaternion::Identity;
};

