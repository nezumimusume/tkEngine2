/*!
 * @brief	背景。
 */
#pragma once

#include "tkEngine/physics/tkMeshCollider.h"
#include "tkEngine/physics/tkRigidBody.h"

class Background : public IGameObject{
public:
	bool Start();
	void Update();
	void Render(CRenderContext& rc);
	void OnDestroy() override;
private:
	CSkinModel 		m_skinModel;		//!<スキンモデル。
	CSkinModelData	m_skinModelData;	//!<スキンモデルデータ。
	CRigidBody		m_rigidBody;		//!<剛体。
	CMeshCollider	m_meshCollider;		//!<メッシュコライダー。
	ID3D11RasterizerState* m_rasterizerState = nullptr;	//!<ラスタライザステート。
};