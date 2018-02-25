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
	~CPlayerRenderer();
	/*!
	 * @brief	スタート。
	 */
	void Awake() override;
	/*!
	 * @brief	更新。
	 */
	void Update() override;
	/*!
	* @brief	スキンモデルデータの取得。
	*/
	CSkinModelData& GetSkinModelData()
	{
		return m_skinModelRender->GetSkinModelData();
	}
	/*!
	* @brief	スキンモデルデータの取得。
	*/
	const CSkinModelData& GetSkinModelData() const
	{
		return m_skinModelRender->GetSkinModelData();
	}
	/*!
	* @brief	スキンモデルレンダラの取得。
	*/
	prefab::CSkinModelRender* GetSkinModelRender()
	{
		return m_skinModelRender;
	}
private:
	/*CSkinModelData m_skinModelData;		//!<スキンモデルデータ。
	CSkinModel m_skinModel;				//!<スキンモデル。*/
	prefab::CSkinModelRender* m_skinModelRender = nullptr;
	CShaderResourceView m_normalMap;	//!<法線マップ。
	CShaderResourceView m_specMap;		//!<スペキュラマップ。
	CShaderResourceView m_wnormalMap;	//!<武器の法線マップ。
	CShaderResourceView m_wspecMap;		//!<武器のスペキュラマップ。
	CPlayer* m_player = nullptr;		//!<プレイヤー。	
};