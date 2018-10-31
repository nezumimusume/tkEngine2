#pragma once
/// <summary>
/// タイトルクラス。
/// </summary>
class Title final : public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
private:
	prefab::CSpriteRender* m_spr = nullptr;
};

