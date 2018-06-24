/*!
 * @brief	背景。
 */
#pragma once

#include "tkEngine/physics/tkMeshCollider.h"
#include "tkEngine/physics/tkRigidBody.h"

class Background : public IGameObject{
public:
	bool Start();	
	void OnDestroy() override;
private:
	prefab::CSkinModelRender* m_modelRender = nullptr;	//!<モデルレンダラ。
	
	CRigidBody		m_rigidBody;		//!<剛体。
	CMeshCollider	m_meshCollider;		//!<メッシュコライダー。
	ID3D11RasterizerState* m_rasterizerState = nullptr;	//!<ラスタライザステート。
};