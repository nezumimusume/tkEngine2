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
	/*!
	 *@brief	事前アップデート。
	 */
	void PreUpdate() override;
	
	/*!
	 *@brief	遅延更新。
	 */
	void PostUpdate() override;
	/*!
	*@brief	描画
	*/
	void Render(CRenderContext& renderContext) override;
	void UpdateWorldMatrix(const CVector3& trans,	const CQuaternion& rot,	const CVector3& scale );
	
private:
	CSkinModelData	m_skinModelData;	//!<スキンモデルデータ。
	CSkinModel		m_skinModel;		//!<スキンモデル。	
	int m_numStar = 0;
};

