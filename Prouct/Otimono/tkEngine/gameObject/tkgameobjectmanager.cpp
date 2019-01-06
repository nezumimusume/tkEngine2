/*!
 *@brief	CGameObjectのマネージャ
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/gameObject/tkgameobjectmanager.h"
#include <future>
#include "tkEngine/graphics/preRender/tkPreRender.h"
#include "tkEngine/graphics/tkPresetRenderState.h"
#include "tkEngine/graphics/tkSkinModelShaderConst.h"
#include "tkEngine/graphics/tkPresetSamplerState.h"

namespace tkEngine{
	void CGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}
	void CGameObjectManager::PreUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
	}
	void CGameObjectManager::Update()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
	}
	void CGameObjectManager::PostUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
	}
	void CGameObjectManager::BeginRender(CRenderContext& rc)
	{
		float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
		CRenderTarget* renderTargets[] = {
			&GraphicsEngine().GetMainRenderTarget()
		};
	//	rc.BeginRender();
		rc.OMSetRenderTargets(1, renderTargets);
		rc.ClearRenderTargetView(0, ClearColor);
		rc.RSSetViewport(0.0f, 0.0f, (float)GraphicsEngine().GetFrameBufferWidth(), (float)GraphicsEngine().GetFrameBufferHeight());
		rc.RSSetState(RasterizerState::sceneRender);
		rc.OMSetDepthStencilState(DepthStencilState::SceneRender);
		rc.PSSetSampler(0, *CPresetSamplerState::clamp_clamp_clamp_linear);
	}
	void CGameObjectManager::ForwardPreRender(CRenderContext& rc)
	{
		BeginGPUEvent(L"enRenderStep_ForwardPreRender");
		//レンダリングステップをフォワードレンダリングに。
		rc.SetRenderStep(enRenderStep_ForwardPreRender);
		
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreForwardRenderWrapper(rc);
			}
		}
		EndGPUEvent();
	}
	void CGameObjectManager::ForwardRender(CRenderContext& rc)
	{
		BeginGPUEvent(L"enRenderStep_ForwardRender");
		rc.SetRenderStep(enRenderStep_ForwardRender);
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->ForwardRenderWrapper(rc);
			}
		}
		//物理ワールドのデバッグ描画。
		PhysicsWorld().DebubDrawWorld(rc);
		EndGPUEvent();
	}
	void CGameObjectManager::PostRender(CRenderContext& rc)
	{
		BeginGPUEvent(L"enRenderStep_Render2DToScene");

		rc.OMSetBlendState(AlphaBlendState::trans);
		rc.RSSetState(RasterizerState::spriteRender);
		rc.OMSetDepthStencilState(DepthStencilState::spriteRender);
		rc.SetRenderStep(enRenderStep_Render2DToScene);
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostRenderWrapper(rc);
			}
		}
		EndGPUEvent();
	}
	void CGameObjectManager::ExecuteFromMainThread()
	{
		//Start
		Start();
	}
	void CGameObjectManager::ExecuteFromGameThread()
	{
		
		//更新系の処理。
		{
			
			//事前アップデート。
			PreUpdate();
			//プリレンダリングの更新処理。
			GraphicsEngine().GetPreRender().Update();
			//アップデート。
			Update();
			//遅延アップデート。
			PostUpdate();
			//シーングラフを更新。
			UpdateSceneGraph();
			//ライトを更新。
			LightManager().Update();
			//エフェクトを更新。
			GraphicsEngine().GetEffectEngine().Update();
		}
		//描画系の処理。
		{
			CRenderContext& renderContext = GraphicsEngine().GetRenderContext();
			//画面をクリア
			BeginRender(renderContext);
			//プリレンダリング。
			GraphicsEngine().GetPreRender().Render(renderContext);
			//ディファードシェーディング。
			GraphicsEngine().DefferdShading(renderContext);
			//事前フォワードレンダリング。
			ForwardPreRender(renderContext);
			//フォワードレンダリング。
			ForwardRender(renderContext);
			//ポストエフェクト。
			GraphicsEngine().GetPostEffect().Render(renderContext);
			//2D的なものの描画。
			PostRender(renderContext);
		}
		ExecuteDeleteGameObjects();
	}
	void CGameObjectManager::UpdateSceneGraph()
	{
		//ワールド行列を更新。
		for (auto transform : m_childrenOfRootTransformList) {
			transform->UpdateWorldMatrixAll();
		}
	}
	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//バッファを切り替え。
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for(GameObjectList& goList : m_deleteObjectArray[preBufferNo]){
			for(IGameObject* go : goList){
				GameObjectPrio prio = go->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find( goExecList.begin(),goExecList.end(),go );
				if (it != goExecList.end()) {
					//削除リストから除外された。
					(*it)->m_isRegistDeadList = false;
					if ((*it)->IsNewFromGameObjectManager()) {
						delete (*it);
					}
				}
				goExecList.erase(it);
			}
			goList.clear();
		}
	}
	void CGameObjectManager::Init( int gameObjectPrioMax )
	{
		TK_ASSERT( gameObjectPrioMax <= GAME_OBJECT_PRIO_MAX, "ゲームオブジェクトの優先度の最大数が大きすぎます。");
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}
}