/*!
 *@brief	レベル。
 */

#pragma once

namespace tkEngine{
	class CMapChip;
	/*!
	* @brief	オブジェクト名。
	*/
	struct LevelObjectData {
		CVector3 position;		//<座標。
		CQuaternion rotation;	//!<回転。
		const wchar_t* name;	//!<名前。
	};
	/*!
	 *@brief	レベル。
	 */
	class CLevel : Noncopyable{
	public:
		/*!
		 * @brief	レベルを初期化。
		 *@param[in]	levelDataFilePath		tklファイルのファイルパス。
		 *@param[in] hookFunc				オブジェクトを作成する時の処理をフックするための関数オブジェクト。
		 *   フックしないならnullptrを指定すればよい、
		 * この関数オブジェクトがfalseを返すと、オブジェクトの情報から、
		 * 静的オブジェクトのMapChipクラスのインスタンスが生成されます。
		 * オブジェクトの名前などで、ドアやジャンプ台、アイテムなどの特殊なクラスのインスタンスを生成したときに、
		 * デフォルトで作成されるMapChipクラスのインスタンスが不要な場合はtrueを返してください。
		 * 例えば、フック関数の中で、渡されたオブジェクトデータの名前のモデルを描画するクラスのインスタンスを
		 * 生成したときに、falseを返してしまうと、同じモデルが二つ描画されることになります。
		 */
		void Init( const wchar_t* filePath,  std::function<bool(LevelObjectData& objData)> hookFunc);
	private:
		using CMapChipPtr = std::unique_ptr<CMapChip>;
		std::vector<CMapChipPtr>	m_mapChipPtr;		//マップチップの可変長配列。
	};
}