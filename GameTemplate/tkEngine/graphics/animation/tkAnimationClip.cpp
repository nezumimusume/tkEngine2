/*!
 *@brief	アニメーションクリップ。
 */

#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/graphics/animation/tkAnimationClip.h"

namespace tkEngine {
	/*!
	*@brief
	*/
	CAnimationClip::~CAnimationClip()
	{
	}
	/*!
	*@brief	アニメーションクリップをロード。
	*@param[in]	filePath	ファイルパス。
	*@param[in]	clipName	クリップ名。
	*/
	void CAnimationClip::Load(const wchar_t* filePath, const wchar_t* clipName)
	{
		auto fp = _wfopen(filePath, L"rb");
		if (fp == nullptr) {
			TK_WARNING_MESSAGE_BOX_W(L"アニメーションクリップのオープンに失敗しました。ファイルパス : %s\n"
									 L"原因として、下記の２点が考えられます。\n"
									 L"① ファイルパスが間違えている。\n"
									 L"② Assetsフォルダの中にファイルが存在しない。\n"
									 L"この２点を確認して、問題が存在しない場合は、一度VisualStudioのビルド/リビルドを行ってみてください。\n", filePath);
			
			return;
		}
		if (clipName != nullptr) {
			m_clipName = clipName;
		}
		//アニメーションクリップのヘッダーをロード。
		AnimClipHeader header;
		fread(&header, sizeof(header), 1, fp);
		
		if (header.numAnimationEvent > 0) {
			m_animationEvent = std::make_unique<CAnimationEvent[]>(header.numAnimationEvent);
			//アニメーションイベントがあるなら、イベント情報をロードする。
			for (auto i = 0; i < (int)header.numAnimationEvent; i++) {
				AnimationEvent animEvent;
				fread(&animEvent, sizeof(animEvent), 1, fp);
				//イベント名をロードする。
				static char eventName[256]; 
				static wchar_t wEventName[256];
				fread(eventName, animEvent.eventNameLength+1, 1, fp);
				mbstowcs(wEventName, eventName, 255);
				m_animationEvent[i].SetInvokeTime(animEvent.invokeTime);
				m_animationEvent[i].SetEventName(wEventName);
			}
		}
		m_numAnimationEvent = header.numAnimationEvent;

		//中身をごそっとロード。
		auto keyframes = std::make_unique<KeyframeRow[]>(header.numKey);
		fread(keyframes.get(), sizeof(KeyframeRow), header.numKey, fp);
		fclose(fp);
		for (auto i = 0; i < (int)header.numKey; i++) {
			auto keyframe = std::make_unique<Keyframe>();
			keyframe->boneIndex = keyframes[i].boneIndex;
			keyframe->transform = CMatrix::Identity;
			keyframe->time = keyframes[i].time;
			for (auto j = 0; j < 4; j++) {
				keyframe->transform.m[j][0] = keyframes[i].transform[j].x;
				keyframe->transform.m[j][1] = keyframes[i].transform[j].y;
				keyframe->transform.m[j][2] = keyframes[i].transform[j].z;
			}
			m_keyframes.push_back(std::move(keyframe));
		}

		//ボーンインデックスごとのキーフレームの連結リストを作成する。
		m_keyFramePtrListArray.resize(MAX_BONE);
		for (auto& keyframe : m_keyframes) {
			m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe.get());
			if (m_topBoneKeyFramList == nullptr) {
				m_topBoneKeyFramList = &m_keyFramePtrListArray[keyframe->boneIndex];
			}
		}
		m_loaded = true;
	}

#if BUILD_LEVEL!=BUILD_LEVEL_MASTER
	void CAnimationClip::Upconverte(const wchar_t* filePath)
	{
		auto fp = _wfopen(filePath, L"rb");
		//ファイルサイズを調べる。
		fseek(fp, 0, SEEK_END);
		auto file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		//全部ドカッとコピー。
		auto data = std::make_unique<char[]>(file_size);
		fread(&data[0], file_size, 1, fp);
		fclose(fp);

		//書き込みモードで開く。
		fp = _wfopen(filePath, L"wb");
		auto numKey = reinterpret_cast<int*>(&data[0]);
		//先頭4バイトを描き込む。
		fwrite(&data[0], 4, 1, fp);
		//続いてアニメーションイベント数を描き込む。
		int numAnimEvent = 0;
		fwrite(&numAnimEvent, 4, 1, fp);
		//残りをがさっと書き込む。
		fwrite(&data[4], file_size - 4, 1, fp);
		fclose(fp);
	}
#endif
}

