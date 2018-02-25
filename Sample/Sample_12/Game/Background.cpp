#include "stdafx.h"
#include "Background.h"


Background::Background()
{
}


Background::~Background()
{
}
void Background::OnDestroy()
{
	//物理ワールドから削除。
	PhysicsWorld().RemoveRigidBody(m_rigidBody);
}
bool Background::Start()
{
	//モデルをロード。
	m_skinModelData.Load(L"modelData/background.cmo");
	m_skinModel.Init(m_skinModelData);
	//メッシュコライダーを作成。
	m_meshCollider.CreateFromSkinModel(m_skinModel, nullptr);
	//剛体の情報を作成。
	RigidBodyInfo rbInfo;
	rbInfo.pos = CVector3::Zero;
	rbInfo.rot = CQuaternion::Identity;
	rbInfo.collider = &m_meshCollider;
	rbInfo.mass = 0.0f;							//質量を0にすると動かない剛体になる。
												//背景などの動かないオブジェクトは0を設定するとよい。
	m_rigidBody.Create(rbInfo);					//作成した情報を使って剛体を作成する。
	PhysicsWorld().AddRigidBody(m_rigidBody);	//作成した剛体を物理ワールドに追加する。
	return true;
}
void Background::Update()
{
	m_skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void Background::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
