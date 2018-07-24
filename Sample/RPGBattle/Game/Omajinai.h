#pragma once


#undef Attack
#undef Hoimi
#undef Zaoriku
#undef Baikiruto
#undef Sukuruto
#undef TenshiNoInori

/*!
*@	攻撃する。
*@details
* ランダムにモンスターを攻撃します。
*/
#define Attack()	CommandAttackEnemy(m_commandsTmp, this)
	
/*!
*@brief	ホイミ(回復呪文)をかける。
*@param	target	魔法をかける対象。
*/
#define Hoimi(target)	CommandHoim(m_commandsTmp, this, &target)
	
/*!
*@brief		ザオリク(蘇生呪文)をかける。
*
*@param		target		魔法をかける対象
*/
#define Zaoriku(target)	CommandZaoriku(m_commandsTmp, this, &target)
/*!
*@brief	バイキルト(攻撃力アップ呪文)を唱える。
*@details
*ターゲットの攻撃力が大幅にアップする。
*@param		target		魔法をかける対象
*/
#define Baikiruto(target)	CommandBaikiruto(m_commandsTmp, this, &target)
/*!
*@brief	スクルト(防御力アップ呪文)を唱える。
*@details
*パーティ全体の防御力が大幅にアップする。
*/
#define Sukuruto()	CommandSukuruto(m_commandsTmp, this)
/*!
*@brief	天使の祈りをかける。
*@details
* 白魔導士自身にしかかけることがない、特殊な祈り。
* 一度だけ死亡しても自動的に蘇生する、白魔導士の奥義。
*@param	target	魔法をかける対象
*/
#define TenshiNoInori()	CommandTenshiNoInori(m_commandsTmp, this)