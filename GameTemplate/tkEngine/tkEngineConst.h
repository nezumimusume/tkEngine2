/*!
 *@brief	tkEngine定数ヘッダ。
 */

#pragma once

namespace tkEngine{
	static const int MRT_MAX = 8;		//!<MRTの最大数。
	static const int MAX_BONE = 512;	//!<ボーンの最大数。
	/*!
	 *@brief	ソフトシャドウの品質レベル。
	 */
	enum class EnSoftShadowQualityLevel {
		enNone,		//!<ソフトシャドウなし。
		eSSSS,		//!<SSSS(ScreenSpaceSoftShadow)
		eSSSS_PCF,	//!<SSSS+PCF
	};
}