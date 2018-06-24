#pragma once
//星の描画クラス。
class StarRenderer : public IGameObject
{
public:
	StarRenderer();
	~StarRenderer();
	/*!
	 *@brief	初期化。
	 *@param[in]	numStar		星の数。
	 */
	void Init(int numStar)
	{
		m_numStar = numStar;
	}
	/*!
	 *@brief	開始時に呼ばれる処理。
	 */
	bool Start() override;
	/*!
	*@brief	更新
	*/
	void Update() override
	{
	}

	void UpdateWorldMatrix(const CVector3& trans,	const CQuaternion& rot,	const CVector3& scale );
	
private:
	prefab::CSkinModelRender* m_modelRender = nullptr;	//!<モデルレンダラ。
	int m_numStar = 0;
};

