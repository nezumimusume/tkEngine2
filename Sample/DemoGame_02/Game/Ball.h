#pragma once
class Ball : public IGameObject
{
public:
	Ball();
	~Ball();
	void Update() override final;
private:
	prefab::CSkinModelRender* m_modelRender = nullptr;
	CVector3 m_position = CVector3::Zero;
	CPhysicsDynamicObject m_phyDynamicObject;
};

