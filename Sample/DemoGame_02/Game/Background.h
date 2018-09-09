#pragma once
class Background : public IGameObject
{
public:
	Background();
	~Background();
	void Update() override final;
private:
	prefab::CSkinModelRender* m_modelRender = nullptr;
	CPhysicsStaticObject m_phyStaticObj;
	CQuaternion m_rotation = CQuaternion::Identity;
	CShaderResourceView m_specTex;
};

