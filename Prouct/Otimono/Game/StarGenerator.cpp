#include "stdafx.h"
#include "StarGenerator.h"
#include "Star.h"
#include "GameObjectName.h"
StarGenerator::StarGenerator()
{
}


StarGenerator::~StarGenerator()
{
}

void StarGenerator::Update()
{
	//星生成のインターバルタイム。(単位：秒)
	const float STAR_GENERATE_INTERVAL = 2.0f;
	m_timer += GameTime().GetFrameDeltaTime();

	if (m_timer > STAR_GENERATE_INTERVAL) {
		//一定時間たったので星を生成する。
		NewGO<Star>(0, STAR_NAME);
		m_timer = 0.0f;
	}
}
