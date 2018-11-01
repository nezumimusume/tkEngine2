/*!
*@brief	HUD
*@details
* HUD、Head up displayの略。HPバーや装備品など、インゲーム中に
* 表示される2DのことをHUDと呼ぶ。
*/

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/HUD/tkHUD.h"
#include "tkEngine/HUD/tkIHUDNode.h"
#include "tkEngine/HUD/node/tkHUDNode_Sprite.h"
#include "tkEngine/HUD/node/tkHUDNode_Font.h"
#include "tkEngine/HUD/node/tkHUDNode_Dummy.h"

namespace tkEngine {
	namespace prefab {
		void CHud::Init(const wchar_t* filePath)
		{
			//HUDをロードする。
			CSkeleton skeleton;
			//中身はただのスケルトン
			skeleton.Load(filePath);
			//ワールド行列を構築しておく。
			skeleton.Update(CMatrix::Identity);
			for (int i = 0; i < skeleton.GetNumBones(); i++) {
				auto bone = skeleton.GetBone(i);
				if (wcsncmp(bone->GetName(), HUD_SPRITE_NODE_NAME_PREFIX, HUD_SPRITE_NODE_NAME_PREFIX_LENGTH) == 0) {
					//Spriteノードを作成。
					auto spriteNode = std::make_shared<CHUDNode_Sprite>(bone);
					m_nodes.push_back(spriteNode);
				}
				else if (wcsncmp(bone->GetName(), HUD_FONT_NODE_NAME_PREFIX, HUD_FONT_NODE_NAME_PREFIX_LENGTH) == 0) {
					//Fontノードを作成。
					auto fontNode = std::make_shared<CHUDNode_Font>(bone);
					m_nodes.push_back(fontNode);
				}
				else {
					//ダミーノードを作成。
					auto dummyNode = std::make_shared<CHUDNode_Dummy>(bone);
					m_nodes.push_back(dummyNode);
				}
			}
			//親子関係を構築。
			for (int i = 0; i < skeleton.GetNumBones(); i++) {
				auto bone = skeleton.GetBone(i);
				for (auto childBone : bone->GetChildren()) {
					m_nodes[i]->AddChildren(
						m_nodes[childBone->GetNo()]
					);
				}
			}
			//ルート。
			m_rootNode = m_nodes[0];
			//名前キーでソートする。
			std::sort(m_nodes.begin(), m_nodes.end(), [](auto lnode, auto rnode)
			{
				return lnode->GetNameKey() < rnode->GetNameKey();
			});
		}
		bool CHud::Start()
		{
			return true;
		}
		void CHud::Update()
		{
			m_rootNode->UpdateWrapper();
		}
		void CHud::PostRender(CRenderContext& rc)
		{
			m_rootNode->PostRenderWrapper(rc);
		}
		
		IHUDNode* CHud::FindNode(const wchar_t* name)
		{
			int nameKey = CUtil::MakeHash(name);
			return FindNode(nameKey);
		}
		
		IHUDNode* CHud::FindNode(int nameKey)
		{
			auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&](const auto& n) {
				return n->GetNameKey() == nameKey; 
			});
			
			if (it != m_nodes.end()) {
				return it->get();
			}
			return nullptr;

		}
		
		void CHud::QueryFontNode(std::function< void(CHUDNode_Font& font) > queryFunc)
		{
			for (auto& node : m_nodes) {
				auto fontNode = dynamic_cast<CHUDNode_Font*>(node.get());
				if (fontNode != nullptr)
				{
					//問い合わせ関数を呼び出す。
					queryFunc(*fontNode);
				}
			}
		}
	}
}