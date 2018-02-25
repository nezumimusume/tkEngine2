#pragma once

#include "tkEngine/graphics/effect/tkEffect.h"

class Game : public IGameObject
{
public:
	
	bool Start() override;
	void Update() override;
	void OnDestroy() override;
	prefab::CSkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダラー。
	CAnimationClip m_animClip[1];					//アニメーションクリップ。
	
	CVector3 m_pos = CVector3::Zero;
};

