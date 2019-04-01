#pragma once
class Game : public IGameObject
{
public:
	//////////////////////////////////////
	// メンバ関数。
	//////////////////////////////////////
	Game();
	~Game();
	bool Start();
	void Update();
	//////////////////////////////////////
	// メンバ変数。
	//////////////////////////////////////
	prefab::CSkinModelRender* m_unityChanModelRender = nullptr;
	prefab::CSkinModelRender* m_bgModelRender = nullptr;
	prefab::CSkinModelRender* m_starModelRender = nullptr;
	prefab::CDirectionLight* m_directionLight = nullptr;

	CVector3 m_unityChanPosition = cv3Zero;
};

