#pragma once
class Background : public IGameObject
{
public:
	Background();
	~Background();
	
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CPhysicsStaticObject m_phyStaticObject;		//静的物理オブジェクト。
};

