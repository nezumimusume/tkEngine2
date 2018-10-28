/*!
 * @brief	クリティカルセクション。
 */

#pragma once

#include <mutex>

namespace tkEngine{
	/*!
	 * @brief	クリティカルセクション。
	 * @details
	 *  再入可能クリティカルセクションです。
	 */
	class CCriticalSection{
	public:
		bool TryLock()
		{
			bool isLock = true;
			std::thread::id thisThreadID = std::this_thread::get_id();
			if (thisThreadID != m_lockThreadId) {
				//ロックを行っているスレッドと現在のスレッドが違う。
				//ロック出来るか試す。
				if (m_mutex.try_lock()) {
					isLock = true;
				}
			}
			else {
				//再入ロック。
				isLock = true;
			}
			if (isLock) {
				//ロックできた。
				m_lockThreadId = std::this_thread::get_id();
				m_lockCount++;
			}
			return isLock;
		}
		void Lock()
		{
			//ロックが取れなかった。
			std::thread::id thisThreadID = std::this_thread::get_id();
			if (thisThreadID != m_lockThreadId) {
				//ロックを行っているスレッドと現在のスレッドが違う。
				//ロックが取れるまで寝る。
				m_mutex.lock();
			}
			//ロック取得できた。
			m_lockThreadId = std::this_thread::get_id();
			m_lockCount++;
		}

		void Unlock()
		{
			m_lockCount--;
			if (m_lockCount == 0) {
				//アンロック。
				m_lockThreadId = std::thread::id();
				m_mutex.unlock();
			}
		}
	private:
		std::mutex		m_mutex;
		std::mutex		m_lockThreadIdMutex;	//lockThreadIdを排他処理するためのミューテックス
		std::thread::id m_lockThreadId;			//ロックしているスレッドのID
		int				m_lockCount = 0;		//ロックカウント。
	};
}