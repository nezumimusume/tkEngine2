/*!
 * @brief	静的オブジェクト。
 */


#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/physics/tkPhysicsStaticObject.h"

namespace tkEngine{
	CPhysicsStaticObject::CPhysicsStaticObject()
	{
	}
	CPhysicsStaticObject::~CPhysicsStaticObject()
	{
		Release();
	}
	/*!
	* @brief	解放。
	*/
	void CPhysicsStaticObject::Release()
	{
		PhysicsWorld().RemoveRigidBody(m_rigidBody);
	}
	/*!
	* @brief	静的物理オブジェクト作成処理の共通処理。
	*/
	void CPhysicsStaticObject::CreateCommon(CVector3 pos, CQuaternion rot, std::unique_ptr<ICollider> collider)
	{
		Release();
		m_collider = std::move(collider);
		RigidBodyInfo rbInfo;
		rbInfo.collider = m_collider.get();
		rbInfo.mass = 0.0f;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		m_rigidBody.Create(rbInfo);
		PhysicsWorld().AddRigidBody(m_rigidBody);
	}
	void CPhysicsStaticObject::CreateMeshObject(prefab::CSkinModelRender* skinModelRender, CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		CreateMeshObject(skinModelRender->GetSkinModel(), pos, rot, scale);
	}
	void CPhysicsStaticObject::CreateMeshObject(CSkinModel& skinModel, CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		
		CMatrix mScale;
		mScale.MakeScaling(scale);
		auto meshCollider = std::make_unique<CMeshCollider>();
		meshCollider->CreateFromSkinModel(skinModel, &mScale);
		CreateCommon(
			pos,
			rot,
			std::move(meshCollider)
		);
		
	}
	/*!
	* @brief	ボックス形状の静的物理オブジェクトを作成。
	*@param[in]	pos			座標。
	*@param[in]	rot			回転。
	*@param[in]	size		サイズ。
	*/
	void CPhysicsStaticObject::CreateBox(CVector3 pos, CQuaternion rot, CVector3 size)
	{
		Release();
	}
}