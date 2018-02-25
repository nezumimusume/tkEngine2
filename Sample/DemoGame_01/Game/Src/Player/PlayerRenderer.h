/*!
 * @brief	プレイヤーレンダラー
 *@details
 * プレイヤーの描画関係の処理を行うクラスです。
 */

#pragma once

class CPlayer;

class CPlayerRenderer : public IGameObject {
public:
	CPlayerRenderer(CPlayer* pl) :
		m_player(pl)
	{
	}
	/*!
	 * @brief	スタート。
	 */
	void Awake() override;
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	/*!
	 * @brief	描画
	 *@param[in]	rc		レンダリングコンテキスト。
	 */
	void Render(CRenderContext& rc) override;
	/*!
	* @brief	スキンモデルデータの取得。
	*/
	CSkinModelData& GetSkinModelData()
	{
		return m_skinModelData;
	}
	/*!
	* @brief	スキンモデルデータの取得。
	*/
	const CSkinModelData& GetSkinModelData() const
	{
		return m_skinModelData;
	}
	/*!
	 *@brief	スキンモデルの取得。
	 */
	CSkinModel& GetSkinModel()
	{
		return m_skinModel;
	}
private:
	CSkinModelData m_skinModelData;		//!<スキンモデルデータ。
	CSkinModel m_skinModel;				//!<スキンモデル。
	CShaderResourceView m_normalMap;	//!<法線マップ。
	CShaderResourceView m_specMap;		//!<スペキュラマップ。
	CShaderResourceView m_wnormalMap;	//!<武器の法線マップ。
	CShaderResourceView m_wspecMap;		//!<武器のスペキュラマップ。
	CPlayer* m_player = nullptr;		//!<プレイヤー。	
};