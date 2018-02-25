#include "stdafx.h"
#include "MapChip.h"


MapChip::MapChip()
{
}


MapChip::~MapChip()
{
}
void MapChip::Init(
	const wchar_t* modelFilePath,
	CVector3 pos,
	CVector3 scale,
	CQuaternion rotation
){
	m_skinModelData.Load(modelFilePath);
	m_skinModel.Init(m_skinModelData);
	m_position = pos;
	m_scale = scale;
	m_rotation = rotation;

	//メッシュコライダーを作成。
	m_meshCollider.CreateFromSkinModel(m_skinModel, nullptr);
	//剛体の情報を作成。
	RigidBodyInfo rbInfo;
	rbInfo.pos = m_position;
	rbInfo.rot = m_rotation;
	rbInfo.collider = &m_meshCollider;
	rbInfo.mass = 0.0f;							//質量を0にすると動かない剛体になる。
												//背景などの動かないオブジェクトは0を設定するとよい。
	m_rigidBody.Create(rbInfo);					//作成した情報を使って剛体を作成する。
	PhysicsWorld().AddRigidBody(m_rigidBody);	//作成した剛体を物理ワールドに追加する。

}
bool MapChip::Start()
{	
	return true;
}
void MapChip::Update()
{
	m_skinModel.Update(m_position, m_rotation, m_scale);
}
void MapChip::Render(CRenderContext& rc)
{
	m_skinModel.Draw(rc, MainCamera().GetViewMatrix(), MainCamera().GetProjectionMatrix());
}
