/*!
*@brief	HUD
*@details
* HUD、Head up displayの略。HPバーや装備品など、インゲーム中に
* 表示される2DのことをHUDと呼ぶ。
*/
#pragma once

#include "tkEngine/HUD/tkIHUDNode.h"
#include "tkEngine/util/tkUtil.h"

namespace tkEngine {
	static const wchar_t* HUD_FONT_NODE_NAME_PREFIX = L"font_";										//HUDシステムのfontノードの名前のプレフィックス。
	static const size_t HUD_FONT_NODE_NAME_PREFIX_LENGTH = wcslen(HUD_FONT_NODE_NAME_PREFIX);		//HUDシステムのfontノードの名前のプレフィックスの長さ。
	static const wchar_t* HUD_SPRITE_NODE_NAME_PREFIX = L"sprite_";									//HUDシステムのspriteノードの名前のプレフィックス。
	static const size_t HUD_SPRITE_NODE_NAME_PREFIX_LENGTH = wcslen(HUD_SPRITE_NODE_NAME_PREFIX);	//HUDシステムのspriteノードの名前のプレフィックスの長さ。
	class CHUDNode_Font;
	namespace prefab {
		/*!
		*@brief	HUD
		*@details
		* HUD、Head up displayの略。HPバーや装備品など、インゲーム中に
		* 表示される2DのことをHUDと呼ぶ。
		*/
		class CHud : public IGameObject {
		public:
			/*!
			 *@brief	初期化。
			 *@param[in]	filePath	HUDデータのファイルパス。
			 */
			void Init(const wchar_t* filePath);
			/*!
			 *@brief	Updateが呼ばれる直前で一回だけ呼ばれる関数。
			 *			GameObjectManagerから自動的に呼ばれるので、
			 *			明示的に呼び出す必要はない。
			 */
			bool Start() override;
			/*!
			 *@brief	毎フレーム呼ばれる関数。
			 *			GameObjectManagerから自動的に呼ばれるので、
			 *			明示的に呼び出す必要はない。
			 */
			void Update() override;
			/*!
			*@brief	Render関数が実行された後で呼ばれる遅延描画処理
			*			GameObjectManagerから自動的に呼ばれるので、
			*			明示的に呼び出す必要はない。
			*/
			void PostRender(CRenderContext& rc) override;
			/*!
			*@brief	ノードを検索。
			*@param[in]		name		検索したいノードの名前。
			*@return	見つかったノードの弱参照。
			*/
			IHUDNode* FindNode(const wchar_t* name);
			/*!
			*@brief	ノードを検索。
			*@param[in]		nameKey		検索したいノードの名前キー。
			*@return	見つかったノードの弱参照。
			*/
			IHUDNode* FindNode(int nameKey);
			/*!
			*@brief	フォントノードに対してクエリを実行。
			*@param[in]		queryFunc	フォントノードへの問い合わせ関数。
			*/
			void QueryFontNode(std::function< void(CHUDNode_Font& font) > queryFunc);
		private:
			
			IHUDNodeSharedPtr m_rootNode;	//ルートノード。
			std::vector<IHUDNodeSharedPtr>	m_nodes;		//ノードのリスト。
		};
	}
}