#pragma once
class Game : public IGameObject
{
public:
	////////////////////////////////////////////
	//メンバ関数。
	////////////////////////////////////////////
	Game();
	~Game();
	/*!
	*@brief	Updateの直前で呼ばれる開始処理。
	*@details
	* 本関数がtrueを返すとゲームオブジェクトの準備が完了したと判断されて</br>
	* Update関数が呼ばれ出します。trueを返して以降はStart関数は呼ばれなくなります。</br>
	* ゲームオブジェクトの初期化に複数フレームかかる場合などはfalseを返して、初期化ステップなどを使って</br>
	* 適切に初期化を行ってください。
	*/
	bool Start();
	/*!
	 *@brief	更新。
	 */
	void Update();
	/*!
	 *@brief	描画。
	 */
	void Render(CRenderContext& rc);
	/*!
	*@brief	削除されるときに呼ばれる。
	*@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
	* デストラクタより前に実行されます。
	*/
	virtual void OnDestroy();
	/*!
	*@brief	ポイントライトの初期化。
	*/
	void InitPointLight();
	////////////////////////////////////////////
	//メンバ変数。
	////////////////////////////////////////////
	CSkinModel m_charaSkinModel;			//キャラクタ用のスキンモデル。
	CSkinModelData m_charaSkinModelData;	//キャラクタ用のスキンモデルデータ。
	CSkinModel m_bgSkinModel;				//背景用のキャラクタスキンモデル。
	CSkinModelData m_bgSkinModelData;		//背景用のスキンモデルデータ。

	prefab::CDirectionLight* m_directionLig = nullptr;		//ディレクションライト。
	std::vector<prefab::CPointLight*>	m_pointLightList;	//ポイントライトのリスト。
};

