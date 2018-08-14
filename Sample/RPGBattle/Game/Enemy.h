#pragma once
#include "AIAvator.h"

class Enemy : public AIAvator
{
public:
	Enemy();
	~Enemy();
	void Init(LevelObjectData& objData);
	void PlayAnimation(int animNo, float interpolateTime) override
	{
		m_currentAnimNo = animNo;
		m_skinModelRender->PlayAnimation(animNo, interpolateTime);
	}
	bool IsPlayAnimation()
	{
		return m_skinModelRender->IsPlayingAnimation();
	}
	int GetCurrentAnimationNo() const
	{
		return m_currentAnimNo;
	}
	void GenerateAICommandList(std::deque<IAICommandUPtr>& commands) override;
private:
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

private:
	int m_currentAnimNo = 0;
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CShaderResourceView m_specMap;	//スペキュラマップ。
	CAnimationClip m_anmClips[enAnimation_Num];
};

