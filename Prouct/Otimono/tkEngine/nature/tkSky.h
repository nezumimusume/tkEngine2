#pragma once

namespace tkEngine{
namespace prefab{
	/// <summary>
	/// 空を描画できるゲームオブジェクト
	/// </summary>
	/// <remarks>
	/// キューブマップを利用した空の描画行われます(SkyCubeMap)。
	/// SkyCubeMapを利用すると、高解像度の空のレンダリングが可能になります。
	/// </remarks>
	class CSky final : public IGameObject
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CSky();
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CSky();
		/// <summary>
		/// 開始処理。
		/// </summary>
		/// <returns>trueを返したら初期化完了。</returns>
		bool Start() override ;
		/// <summary>
		/// 空の大きさを設定。
		/// </summary>
		/// <param name="scale">空の大きさ</param>
		void SetScale(float scale)
		{
			m_scale = { scale, scale, scale };
			m_isDirty = true;
		}
		/// <summary>
		/// 空の大きさを設定(ベクトル版)
		/// </summary>
		/// <param name="scale">空の大きさ</param>
		void SetScale(CVector3 scale)
		{
			m_scale = scale;
			m_isDirty = true;
		}
		/// <summary>
		/// 自己発光カラーを設定。
		/// </summary>
		/// <param name="emissionColor">自己発光カラー</param>
		void SetEmissionColor(CVector3 emissionColor)
		{
			m_emissionColor = emissionColor;
			m_isDirty = true;
		}
		/// <summary>
		/// 空の位置を決める。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(CVector3 pos)
		{
			m_position = pos;
			m_isDirty = true;
		}
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update() override;
		/// <summary>
		/// 空のキューブマップのファイルパスを設定。
		/// </summary>
		/// <param name="filepath">ファイルパス</param>
		/// <remarks>
		/// 任意のキューブマップに変更したい場合に、使用してください。
		/// 変更がなければ、デフォルトのキューブマップがロードされます。
		/// この関数を利用する場合は、インスタンスを作成した直後に利用してください。
		/// それ以外のタイミングで利用すると、すでにキューブマップがロード済みとなっているため
		/// 変更することはできません。
		/// </remarks>
		/// <example>
		/// キューブマップを変更するサンプルコード
		/// </example>
		/// <code>
		/// auto sky = NewGO<prefab::CSky>(0);							//空のインスタンスを作成。
		/// sky->SetSkyCubeMapFilePath(L"modelData/mySkyCubeMap.dds");	//キューブマップを差し替え。
		/// </code>
		void SetSkyCubeMapFilePath(const wchar_t* filepath)
		{
#ifndef TK_MASTER
			if (m_isStart == true) {
				TK_WARNING("SkyCubeMapのロードが完了しているため"
					"キューブマップの差し替えはできません。"
					"SetSkyCubeMapFilePath関数のコメントを参照して、適切なタイミングで本関数を利用してください。");
			}
#endif
			m_skyCubeMapFilePath = filepath;
		}
		/// <summary>
		/// 空のモデルのファイルパスを設定。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// /// <remarks>
		/// 任意の空のモデルに変更したい場合に、使用してください。
		/// 変更がなければ、デフォルトの空がロードされます。
		/// この関数を利用する場合は、インスタンスを作成した直後に利用してください。
		/// それ以外のタイミングで利用すると、すでに空のモデルがロード済みとなっているため
		/// 変更することはできません。
		/// </remarks>
		/// <example>
		/// モデルを変更するサンプルコード
		/// </example>
		/// <code>
		/// auto sky = NewGO<prefab::CSky>(0);					//空のインスタンスを作成。
		/// sky->SetSkyModelFilePath(L"modelData/mySky.cmo");	//モデルを差し替え。
		/// </code>
		void SetSkyModelFilePath(const wchar_t* filePath)
		{
#ifndef TK_MASTER
			if (m_isStart == true) {
				TK_WARNING("空のモデルのロードが完了しているため"
					"空のモデルの差し替えはできません。"
					"SetSkyModelFilePath関数のコメントを参照して、適切なタイミングで本関数を利用してください。");
			}
#endif
			m_skyModelFilePath = filePath;
		}
	private:
		prefab::CSkinModelRender* m_skinModelRender = nullptr;	//モデル描画のゲームオブジェクト。
		CShaderResourceView m_skyCube;							//スカイキューブマップのSRV
		CShader m_psSkyShader;									//空用のシェーダー。
		CVector3 m_scale = { 8000.0f, 8000.0f, 8000.0f };		//空の拡大率。
		CVector3 m_position = CVector3::Zero;					//座標。
		CVector3 m_emissionColor = CVector3::Zero;				//自己発光カラー。
		bool m_isDirty = false;									//ダーティフラグ。
		std::wstring m_skyCubeMapFilePath = L"modelData/preset/skyCubeMap.dds";		//空のキューブマップのファイルパス。
		std::wstring m_skyModelFilePath = L"modelData/preset/sky.cmo";				//空のcmoファイルのファイルパス。
	};
}
}
