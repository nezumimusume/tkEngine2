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
	virtual void OnDestroy() {}

	////////////////////////////////////////////
	//メンバ変数や列挙とか。
	////////////////////////////////////////////
	enum EnAnimationClip {
		enAnimationClip_idle,	//待機アニメーション。
		enAnimationClip_run,	//走りアニメーション。
		enAnimationClip_walk,	//歩きアニメーション。
		enAnimationClip_Num,	//アニメーションクリップの数。
	};
	CSkinModel m_skinModel;					//スキンモデル。
	CSkinModelData m_skinModelData;			//スキンモデルデータ。
	CAnimationClip m_animClips[enAnimationClip_Num];	//アニメーションクリップ。
	CAnimation m_animation;								//アニメーション。
};

