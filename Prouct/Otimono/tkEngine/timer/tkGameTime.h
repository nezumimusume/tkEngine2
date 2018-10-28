/*!
 *@brief	ゲーム時間クラス。
 * シングルトン。
 */
#pragma once

#include "tkEngine/timer/tkStopwatch.h"
#include <queue>

namespace tkEngine{
	/*!
	 *@brief	ゲーム時間。
	 */
	class CGameTime{
		CGameTime() 
		{
		}
		~CGameTime()
		{
		}
	public:
		static CGameTime& GetInstance()
		{
			static CGameTime t;
			return t;
		}
		/*!
		 *@brief	1フレームの経過時間を取得(単位・秒)。
		 */
		const float GetFrameDeltaTime() const
		{
			//固定FPSにする。可変は要検討。
			//return 1.0f / 30.0f;
			return m_frameDeltaTime;
		}
		void PushFrameDeltaTime(float deltaTime)
		{
			m_frameDeltaTimeQue.push_back(deltaTime);
			if (m_frameDeltaTimeQue.size() > 30.0f) {
				float totalTime = 0.0f;
				for (auto time : m_frameDeltaTimeQue) {
					totalTime += time;
				}
				//平均値をとる。
				m_frameDeltaTime = min(1.0f / 30.0f, totalTime / m_frameDeltaTimeQue.size());
				m_frameDeltaTimeQue.pop_front();
			}
		}
		
	private:
		std::list<float> m_frameDeltaTimeQue;
		float		m_frameDeltaTime = 1.0f / 60.0f;		//1フレームの経過時間。
	};
	static CGameTime& GameTime()
	{
		return CGameTime::GetInstance();
	}
}
