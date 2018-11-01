/*!
 *@brief	FootIK
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/animation/postProcess/tkAnimationFootIK.h"
#include "tkEngine/graphics/tkSkeleton.h"

namespace tkEngine{
	namespace {
		//衝突したときに呼ばれる関数オブジェクト(地面用)
		struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
		{
			bool isHit = false;									//衝突フラグ。
			CVector3 hitPos = CVector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
			CVector3 startPos = CVector3::Zero;					//レイの始点。
			float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。

																//衝突したときに呼ばれるコールバック関数。
			virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character) {
					//キャラクタ属性のコリジョンと衝突した。
					return 0.0f;
				}
				//衝突点の法線を引っ張ってくる。
				CVector3 hitNormalTmp = *(CVector3*)&convexResult.m_hitNormalLocal;
				//上方向と法線のなす角度を求める。
				float angle = hitNormalTmp.Dot(CVector3::Up);
				angle = fabsf(acosf(angle));
				if (angle < CMath::PI * 0.3f		//地面の傾斜が54度より小さいので地面とみなす。
					|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //もしくはコリジョン属性が地面と指定されている。
					) {
					//衝突している。
					isHit = true;
					CVector3 hitPosTmp = *(CVector3*)&convexResult.m_hitPointLocal;
					//衝突点の距離を求める。。
					CVector3 vDist;
					vDist.Subtract(hitPosTmp, startPos);
					float distTmp = vDist.Length();
					if (dist > distTmp) {
						//この衝突点の方が近いので、最近傍の衝突点を更新する。
						hitPos = hitPosTmp;
						dist = distTmp;
					}
				}
				return 0.0f;
			}
		};

	}
	CAnimationFootIK::CAnimationFootIK()
	{
	}
	CAnimationFootIK::~CAnimationFootIK()
	{
	}
	
	bool CAnimationFootIK::CalcIKTargetPosition(const SFoot& foot, CVector3& targetPosition)
	{
		//エフェクタの親の座標から、コライダーの位置を決める。
		if (foot.m_bone->GetParentId() == -1) {
			//ターゲットは見つからなかった。。
			return false;
		}
		CMatrix m = foot.m_bone->GetWorldMatrix();
		CVector3 effectorPos = { m.m[3][0], m.m[3][1], m.m[3][2] };

		//レイを作成する。
		btTransform btRayStart, btRayEnd;
		btRayStart.setIdentity();
		btRayEnd.setIdentity();
		//始点はカプセルコライダーの中心。
		btRayStart.setOrigin({ effectorPos.x, effectorPos.y, effectorPos.z });
		//10cm下に向けてレイを飛ばす。
		btRayEnd.setOrigin(
		{ 
			effectorPos.x,
			effectorPos.y - UnitCM(5.0f),
			effectorPos.z
		});

		SweepResultGround callback;
		callback.startPos.Set(btRayStart.getOrigin());
		PhysicsWorld().ConvexSweepTest((const btConvexShape*)foot.m_collider.GetBody(), btRayStart, btRayEnd, callback);
		if (callback.isHit == false) {
			return false;
		}
		targetPosition = callback.hitPos;
		//エフェクタ―との距離を調べる。
		CVector3 diff = targetPosition - effectorPos;
		if (diff.LengthSq() < 0.01f) {
			//エフェクタ―と目標座標が近すぎるｒ。
			return false;
		}
		return true;
	}
	void CAnimationFootIK::Init(CSkeleton* skeleton)
	{
		m_skeleton = skeleton;
	}
	
	void CAnimationFootIK::Enable(const SFootIKParam& param)
	{
		m_isEnable = true;
		int boneId = m_skeleton->FindBoneID(param.footBoneName_0);
		m_foots[0].m_bone = m_skeleton->GetBone(boneId);
		m_foots[0].m_collider.Create(param.footCapsuleColliderRadius_0, param.footCapsuleColliderHeight_0);
		
		boneId = m_skeleton->FindBoneID(param.footBoneName_1);
		m_foots[1].m_bone = m_skeleton->GetBone(boneId);
		m_foots[1].m_collider.Create(param.footCapsuleColliderRadius_1, param.footCapsuleColliderHeight_1);

		m_rootBoneId = m_skeleton->FindBoneID(param.rootBoneName);
	}
	void CAnimationFootIK::SolverCCD_IK(const SFoot& foot, CVector3 targetPosition)
	{
		//仮仮。５回計算する。
		const int iteration = 1;
		for (int i = 0; i < iteration; i++) {
			int parentId = foot.m_bone->GetParentId();
			CMatrix m = foot.m_bone->GetWorldMatrix();
			CVector3 effectorPos = { m.m[3][0], m.m[3][1], m.m[3][2] };
			CBone* currentBone = nullptr;
			while (parentId != m_rootBoneId) {
				currentBone = m_skeleton->GetBone(parentId);
				CMatrix m = currentBone->GetWorldMatrix();
				CVector3 currentBonePos = {m.m[3][0], m.m[3][1], m.m[3][2]};
				//
				CVector3 toEffectorPos = effectorPos - currentBonePos;
				toEffectorPos.Normalize();
				CVector3 toTargetPos = targetPosition - currentBonePos;
				toTargetPos.Normalize();
				//回転軸を求める。
				CVector3 rotateAxis;
				rotateAxis.Cross(toEffectorPos, toTargetPos);
				rotateAxis.Normalize();

				//適正位置に合わせるための回転行列を求める。
				float angle = acos(min(1.0f, toEffectorPos.Dot(toTargetPos)));
				CQuaternion qRot;
				qRot.SetRotation(rotateAxis, angle);
				CMatrix mAddRot;
				mAddRot.MakeRotationFromQuaternion(qRot);
				//回転行列ができたので、ワールドに反映。
				m.m[3][0] = 0.0f;
				m.m[3][1] = 0.0f;
				m.m[3][2] = 0.0f;
				//追加の回転を加える。
				m = m * mAddRot;
				m.m[3][0] = currentBonePos.x;
				m.m[3][1] = currentBonePos.y;
				m.m[3][2] = currentBonePos.z;
				
				//ローカル行列を求める。
				CMatrix localMat;
				parentId = currentBone->GetParentId();
				if (parentId != -1) {
					CMatrix toParentSpaceMatrix = m_skeleton->GetBone(parentId)->GetWorldMatrix();
					toParentSpaceMatrix.Inverse();
					localMat = m * toParentSpaceMatrix;
					parentId = currentBone->GetParentId();
					currentBone->SetLocalMatrix(localMat);
				}
			}
			//ルートボーンから下の骨のワールド行列の再計算を行う。
			CSkeleton::UpdateBoneWorldMatrix(*currentBone, m_skeleton->GetBone(m_rootBoneId)->GetWorldMatrix());
		}
	}
	void CAnimationFootIK::SolverPIK(const SFoot& foot, CVector3 targetPosition)
	{
		//関節にパーティクルを置いていく。
		
	}
	void CAnimationFootIK::Update()
	{
		if (m_isEnable == false) {
			//FootIKが無効。
			return;
		}
		for (auto& foot : m_foots) {
			//エフェクタの目標座標を求める。
			CVector3 targetPosition;
			if (CalcIKTargetPosition(foot, targetPosition) == false) {
				//目標座標が求まらなかった。
				continue;
			}
#if 0
			//CCD-IK法を使用してIK
			SolverCCD_IK(foot, targetPosition);
#else
			//PIKを使用してIK
			SolverPIK(foot, targetPosition);
#endif
		}
	}
}