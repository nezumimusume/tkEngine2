#include "stdafx.h"
#include "Shiromadoushi.h"
#include "IAICommand.h"
#include "Game.h"

/////////////////////////////////////////////////
// 白魔導士が使えるコマンド一覧
/////////////////////////////////////////////////
/*!
*@	攻撃する。
*@details
* ランダムにモンスターを攻撃します。
*/
#define Attack()
/*!
*@brief	ホイミ(回復呪文)をかける。
*@param	target	魔法をかける対象。
*/
#define Hoimi(target)
/*!
*@brief		ザオリク(蘇生呪文)をかける。
*
*@param		target		魔法をかける対象
*/
#define Zaoriku(target)
/*!
*@brief	バイキルト(攻撃力アップ呪文)を唱える。
*@details
*ターゲットの攻撃力が大幅にアップする。
*@param		target		魔法をかける対象
*/
#define Baikiruto(target)
/*!
*@brief	スクルト(防御力アップ呪文)を唱える。
*@details
*パーティ全体の防御力が大幅にアップする。
*/
#define Sukuruto()
/*!
*@brief	天使の祈りをかける。
*@details
* 白魔導士自身にしかかけることがない、特殊な祈り。
* 一度だけ死亡しても自動的に蘇生する、白魔導士の奥義。
*@param	target	魔法をかける対象
*/
#define TenshiNoInori()

#include "Omajinai.h"

/*!
*@brief	白魔導士のAI
*@param		senshi		戦士
*@param		ryu			竜騎士
*@param		shiro		白魔導士。
*/
void Shiromadoushi::AI(
	AIAvator& senshi, 
	AIAvator& ryu,
	AIAvator& shiro 
)
{
	/////////////////////////////////////////////////
	// ここから～
	/////////////////////////////////////////////////


	if (shiro.isTenshiNoInori == false) {
		TenshiNoInori();
	}
	//もしも戦士のHPが1000以下ならホイミをかける。
	if (senshi.hp < 1000) {
		
	}
	//もしも白魔導士のHPが1000以下ならホイミをかける。
	if (shiro.hp < 1000) {
		Hoimi(shiro);
	}
	if (ryu.hp == 0) {
		Zaoriku(ryu);
	}
	//パーティにスクルトをかける。
	Sukuruto();
	//リューサンにバイキルトをかける。
	Baikiruto(ryu);
	/////////////////////////////////////////////////
	// ～ここまでの間に白魔導士のAIを書く！
	/////////////////////////////////////////////////
}