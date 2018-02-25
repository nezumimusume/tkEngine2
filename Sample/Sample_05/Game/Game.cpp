#include "stdafx.h"
#include "Game.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "tkEngine/light/tkPointLight.h"

Game::Game()
{
}


Game::~Game()
{
}

void Game::OnDestroy()
{
	//削除。
	DeleteGO(m_directionLig);
}
/*!
*@brief	ポイントライトの初期化。
*/
void Game::InitPointLight()
{
	//ポイントライトを64個設置する。
	for (int i = 0; i < 100; i++) {
		//ポイントライトを生成。
		prefab::CPointLight* ptLight = NewGO<prefab::CPointLight>(0);
		//ポイントライトの座標をランダムで決定する。
		CVector3 pos;
		pos.x = CMath::Lerp((float)Random().GetRandDouble(), 200.0f, -200.0f);
		pos.y = CMath::Lerp((float)Random().GetRandDouble(), -5.0f, 0.0f);
		pos.z = CMath::Lerp((float)Random().GetRandDouble(), 200.0f, -200.0f);
		ptLight->SetPosition(pos);
		//ポイントライトのカラーをランダムで決定する。
		CVector3 color;
		color.x = CMath::Lerp((float)Random().GetRandDouble(), 300.0f, 1000.0f);
		color.y = CMath::Lerp((float)Random().GetRandDouble(), 300.0f, 1000.0f);
		color.z = CMath::Lerp((float)Random().GetRandDouble(), 300.0f, 1000.0f);
		ptLight->SetColor(color);
		//ポイントライトの減衰パラメータを設定する・
		CVector3 attn = CVector3::Zero;
		attn.x = 40.0f;		//5mまで光の影響を与える。
		attn.y = 1.0f;
		ptLight->SetAttn(attn);
		//ポイントライトのリストに積む。
		m_pointLightList.push_back(ptLight);

	}
}
bool Game::Start()
{
	//ディレクションライト(太陽光みたいなもの)をシーンに追加。
	m_directionLig = NewGO<prefab::CDirectionLight>(0);
	//ライトの方向を設定。
	m_directionLig->SetDirection({ 0.707f, -0.707f, 0.0f });
	//ライトの色を設定。
	m_directionLig->SetColor({ 10.5f, 10.5f, 10.5f, 1.0f });

	//ポイントライトを追加。
	InitPointLight();

	//カメラを設定。
	MainCamera().SetTarget({ 0.0f, -10.0f, 0.0f });
	MainCamera().SetPosition({ 0.0f, 50.0f, 100.0f });
	MainCamera().Update();

	//キャラクタ用のモデルデータをロード。
	m_charaSkinModelData.Load(L"modelData/Thethief_H.cmo");
	m_charaSkinModel.Init(m_charaSkinModelData);
	
	//背景用のモデルデータをロード。
	m_bgSkinModelData.Load(L"modelData/background.cmo");
	m_bgSkinModel.Init(m_bgSkinModelData);
	
	return true;
}
void Game::Update()
{
	//ポイントライトを回す。
	CQuaternion addRot;
	addRot.SetRotationDeg(CVector3::AxisY, 1.0f);
	for (auto& pt : m_pointLightList) {
		CVector3 pos =pt->GetPosition();
		addRot.Multiply(pos);
		pt->SetPosition(pos);
	}
	//ワールド行列を更新。
	m_charaSkinModel.Update({ 0.0f, -30.0f, 0.0f }, CQuaternion::Identity, {2.0f, 2.0f, 2.0f});	//キャラが小さいのでちょっと大きくしてる。
	m_bgSkinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void Game::Render(CRenderContext& rc)
{
	m_charaSkinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
	m_bgSkinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}