/*!
 *@brief	アニメーションクリップ。
 */

#pragma once

namespace tkEngine{
	/*!
	*@brief	アニメーションクリップのヘッダー。
	*/
	struct AnimClipHeader {
		std::uint32_t numKey;				//!<キーフレームの数。
		std::uint32_t numAnimationEvent;	//!<アニメーションイベントの数。
	};
	/*!
	*@brief	アニメーションイベント
	*/
	struct AnimationEvent {
		float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
		std::uint32_t eventNameLength;		//!<イベント名の長さ。
	};
	/*!
	 *@brief	キーフレーム。
	 */
	struct Keyframe {
		std::uint32_t boneIndex;	//!<ボーンインデックス。
		float time;					//!<時間。
		CMatrix transform;			//!<トランスフォーム。
	};
	/*!
	*@brief	キーフレーム。
	*/
	struct KeyframeRow {
		std::uint32_t boneIndex;	//!<ボーンインデックス。
		float time;					//!<時間。
		CVector3 transform[4];		//!<トランスフォーム。
	};
	/*!
	*@brief	アニメーションイベント。
	*/
	class CAnimationEvent : Noncopyable {
	public:
		CAnimationEvent()
		{
		}
		/*!
		*@brief	イベント発生時間を設定。
		*/
		float GetInvokeTime() const
		{
			return m_invokeTime;
		}
		/*!
		*@brief	イベント名を取得。
		*/
		const wchar_t* GetEventName() const
		{
			return m_eventName.c_str();
		}
		/*!
		*@brief	イベント発生時間を設定。
		*/
		void SetInvokeTime(float time)
		{
			m_invokeTime = time;
		}
		/*!
		*@brief	イベント名を設定。
		*/
		void SetEventName(const wchar_t* name)
		{
			m_eventName = name;
		}
		/*!
		*@brief	イベントが発生済みか判定。
		*/
		bool IsInvoked() const
		{
			return m_isInvoked;
		}
		/*!
		*@brief	イベントが発生済みのフラグを設定する。
		*/
		void SetInvokedFlag(bool flag)
		{
			m_isInvoked = flag;
		}
	private:
		bool m_isInvoked = false;	//!<イベント発生済み？
		float m_invokeTime;			//!<イベント発生時間。
		std::wstring m_eventName;	//!<イベント名。
	};
	/*!
	 *@brief	アニメーションクリップ。
	 */
	class CAnimationClip : Noncopyable {
	public:
		/*!
		 *@brief	ボーンのフリーズ情報。
		 */
		struct SFreezeBoneInfo {
			int boneNo;		//!<フリーズさせるボーン番号。
			bool freezeX;	//!<X方向の移動をフリーズさせる？
			bool freezeY;	//!<Y方向の移動をフリーズさせる？
			bool freezeZ;	//!<Z方向の移動をフリーズさせる？
		};
		typedef std::vector<Keyframe*>		keyFramePtrList;
		/*!
		* @brief	コンストラクタ
		*/
		CAnimationClip() 
		{
			m_freezeBoneInfo.boneNo = -1;
			m_freezeBoneInfo.freezeX = false;
			m_freezeBoneInfo.freezeY = false;
			m_freezeBoneInfo.freezeZ = false;
		}
		/*!
		 *@brief	デストラクタ。
		 */
		~CAnimationClip();
		/*!
		 *@brief	アニメーションクリップをロード。
		 *@param[in]	filePath	ファイルパス。
		 *@param[in]	clipName	クリップ名。
		 */
		void Load(const wchar_t* filePath, const wchar_t* cliipName = nullptr);
#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
		/*!
		*@brief	アップコンバーター。
		*@details
		* 使い捨てコードの塊。使わないように。
		*/
		void Upconverte(const wchar_t* filePath);
#endif
		/*!
		*@brief	ループする？
		*/
		bool IsLoop() const
		{
			return m_isLoop;
		}
		/*!
		*@brief	ループフラグを設定する。
		*/
		void SetLoopFlag(bool flag)
		{
			m_isLoop = flag;
		}
		/*!
		*@brief
		*/
		const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
		{
			return m_keyFramePtrListArray;
		}
		const keyFramePtrList& GetTopBoneKeyFrameList() const
		{
			return *m_topBoneKeyFramList;
		}
		/*!
		 *@brief	クリップ名を取得。
		 */
		const wchar_t* GetName() const
		{
			return m_clipName.c_str();
		}
		/*!
		*@brief	アニメーションをフリーズするボーンの情報を設定。
		*@details
		* フリーズされたボーンのY方向の移動量はすべてのボーンから打ち消されます。
		*  →なにで使える？
		*		→アニメーションに移動量が入っているジャンプモーションとかで使える。
		*/
		void SetFreezeBoneInfo(int boneNo, bool freezeX, bool freezeY, bool freezeZ)
		{
			m_freezeBoneInfo.boneNo = boneNo;
			m_freezeBoneInfo.freezeX = freezeX;
			m_freezeBoneInfo.freezeY = freezeY;
			m_freezeBoneInfo.freezeZ = freezeZ;
		}
		/*!
		*@brief	X方向のアニメーションをフリーズしているボーンの番号を取得。
		*/
		const SFreezeBoneInfo& GetFreezeBoneInfo() const
		{
			return m_freezeBoneInfo;
		}
		/*!
		*@brief	アニメーションイベントを取得。
		*/
		std::unique_ptr<CAnimationEvent[]>& GetAnimationEvent()
		{
			return m_animationEvent;
		}
		/*!
		*@brief	アニメーションイベントの数を取得。
		*/
		int GetNumAnimationEvent() const
		{
			return m_numAnimationEvent;
		}
		/*!
		*@brief	アニメーションクリップはロード済み？
		*/
		bool IsLoaded() const
		{
			return m_loaded;
		}
	private:
		using KeyframePtr = std::unique_ptr<Keyframe>;
		std::wstring m_clipName;	//!<アニメーションクリップの名前。
		bool m_isLoop = false;	//!<ループフラグ。
		std::vector<KeyframePtr>			m_keyframes;				//キーフレーム。
		std::vector<keyFramePtrList>		m_keyFramePtrListArray;		//ボーンごとのキーフレームのリストを管理するための配列。
		std::unique_ptr<CAnimationEvent[]>	m_animationEvent;			//アニメーションイベント。
		int									m_numAnimationEvent = 0;	//アニメーションイベントの数。
		keyFramePtrList*				m_topBoneKeyFramList = nullptr;
		SFreezeBoneInfo m_freezeBoneInfo;	//!<フリーズさせるボーン情報。
		bool m_loaded = false;	//アニメーションクリップがロードされている？
	};
	using CAnimationClipPtr = std::unique_ptr<CAnimationClip>;
}