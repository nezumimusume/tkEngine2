#pragma once

class Player;
class Background;
class GameCamera;

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
	*@brief	削除されるときに呼ばれる。
	*@details	CGameManager::DeleteGameObjectを呼んだときに実行されます。
	* デストラクタより前に実行されます。
	*/
	void OnDestroy();
	////////////////////////////////////////////
	//メンバ変数とか
	////////////////////////////////////////////
	prefab::CSoundSource* m_bgmSoundSource = nullptr;	//BGM用のサウンドソース。
};

