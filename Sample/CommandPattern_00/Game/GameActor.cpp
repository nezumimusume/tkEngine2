#include "stdafx.h"
#include "GameActor.h"
#include "tkEngine/light/tkDirectionLight.h"
#include "ICommand.h"

GameActor::GameActor()
{
}


GameActor::~GameActor()
{
}
bool GameActor::Start()
{
	m_skinModelData.Load(L"modelData/Thethief_H.cmo");
	m_skinModel.Init(m_skinModelData);
	m_skinModel.SetShadowCasterFlag(true);
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		20.0,			//半径。 
		50.0f,			//高さ。
		-980.0f,		//重力、
		m_position		//初期位置。
	);
	//アニメーションを初期化。
	InitAnimation();

	//法線マップをロード。
	m_normalMapSRV.CreateFromDDSTextureFromFile(L"modelData/Thethief_N.dds");
	m_specularMapSRV.CreateFromDDSTextureFromFile(L"modelData/Thethief_S.dds");
	m_wnormalMapSRV.CreateFromDDSTextureFromFile(L"modelData/Thethief_wuqi_N.dds");
	m_wspecularMapSRV.CreateFromDDSTextureFromFile(L"modelData/Thethief_wuqi_S.dds");

	int materialID = 1;
	//マテリアルIDを設定
	m_skinModel.FindMaterial([&](auto material) {
		material->SetMaterialID(materialID);
		if (material->EqualMaterialName(L"bodyMat")) {
			//体のマテリアル。
			material->SetNormalMap(m_normalMapSRV.GetBody());
			material->SetSpecularMap(m_specularMapSRV.GetBody());
		}
		else if (material->EqualMaterialName(L"weapon")) {
			//武器。
			material->SetNormalMap(m_wnormalMapSRV.GetBody());
			material->SetSpecularMap(m_wspecularMapSRV.GetBody());
		}
	});
	//キャラライトを生成。
	m_directionLig = NewGO<prefab::CDirectionLight>(0);
	//ライトの色を設定。
	m_directionLig->SetColor({ 10.5f, 10.5f, 10.5f, 1.0f });
	m_directionLig->SetLightingMaterialIDGroup(1 << materialID);
	return true;
}
void GameActor::InitAnimation()
{
	//アニメーションクリップのロード。
	m_animationClip[enAnimationClip_idle].Load(L"animData/idle.tka");
	m_animationClip[enAnimationClip_run].Load(L"animData/run.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/walk.tka");
	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	//アニメーションを初期化。
	m_animation.Init(m_skinModelData, m_animationClip, enAnimationClip_num);
	//待機アニメーションを流す。
	PlayAnimation(enAnimationClip_idle);
	m_animation.Play(enAnimationClip_idle);
}

void GameActor::Walk()
{
	PlayAnimation(enAnimationClip_walk);
	CVector3 moveSpeed = m_moveDir * 80.0f;
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), moveSpeed);
}

void GameActor::Run()
{
	PlayAnimation(enAnimationClip_run);
	CVector3 moveSpeed = m_moveDir * 160.0f;
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), moveSpeed);
}

void GameActor::Idle()
{
	PlayAnimation(enAnimationClip_idle);
}

void GameActor::TurnLeft()
{
	PlayAnimation(enAnimationClip_walk);
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, -3.0f);
	qRot.Multiply(m_moveDir);
	m_moveDir.Normalize();
	
}

void GameActor::TurnRight()
{
	PlayAnimation(enAnimationClip_walk);
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY, 3.0f);
	qRot.Multiply(m_moveDir);
	m_moveDir.Normalize();
}
void GameActor::ExecuteCommand()
{
	if (m_commands.empty()) {
		return;
	}
	m_commands.front()->Execute(*this);
	m_commands.pop_front();
}
void GameActor::ExecuteRecord()
{
	if (m_commandsRecord.empty()) {
		//レコードの再生終了。
		m_isPlayRecord = false;
		return;
	}
	m_commandsRecord.front()->Execute(*this);
	m_commandsRecord.pop_front();
}
void GameActor::Update()
{
	if (!m_isPlayRecord) {
		//コマンドを実行。
		ExecuteCommand();
	}
	else {
		//レコード再生。
		ExecuteRecord();
	}
	//アニメーションを進める。
	m_animation.Update(GameTime().GetFrameDeltaTime());
	//キャラには女優ライトを当てる
	m_directionLig->SetDirection(MainCamera().GetForward());
	//落下処理。
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeedDown);

	float angle = atan2(m_moveDir.x, m_moveDir.z);
	m_rotation.SetRotation(CVector3::AxisY, angle);
	//ワールド行列を更新。
	CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisX, 90.0f);	//3dsMaxで設定されているアニメーションでキャラが回転しているので、補正用の回転クォータニオンを計算。
	//qRot回転とキャラの回転を乗算して合成する。
	qRot.Multiply(m_rotation, qRot);
	m_skinModel.Update(m_position, qRot, CVector3::One);
}
void GameActor::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
