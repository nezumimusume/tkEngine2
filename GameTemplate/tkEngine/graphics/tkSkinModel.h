/*!
 *@brief	スキンモデル。
 */

#pragma once

#include "tkEngine/graphics/preRender/tkShadowCaster.h"

namespace tkEngine{
	class CSkinModelData;
	/*!
	 *@brief	スキンモデル。
	 *@details
	 * スキンあり、なしモデルの表示をサポートするクラス。</br>
	 * 複数のインスタンスを一度の関数コールで描画することができるインスタンシング描画もサポートしている。</br>
	 */
	class CSkinModel : Noncopyable{
	public:
		/*!
		*@brief	FBXの上方向。
		*/
		enum EnFbxUpAxis {
			enFbxUpAxisY,		//Y-up
			enFbxUpAxisZ,		//Z-up
		};
		typedef std::function<void(CSkeleton&)>		OnPostUpdateSkeleton;

		/*!
		*@brief QueryEffectsを実行したときのコールバックに渡される引数構造体。
		*/
		struct SQuerryEffectsCbArg {
			DirectX::ModelMeshPart*	meshPart;		//現在のエフェクト。
		};
		CSkinModel();
		~CSkinModel();
		/*!
		*@brief	初期化
		*@param[in]	modelData		モデルデータ。
		*@param[in] maxInstance		インスタンスの最大数。1より大きい場合インスタンシング描画が行われます。
		*							インスタンシング描画を行う場合は、UpdateInstance関数を使用して、
		*							インスタンス固有のデータを更新する必要があります。
		*/
		void Init(CSkinModelData& modelData, int maxInstance = 1);
		/*!
		*@brief	更新。
		*@details
		* 通常描画の場合の行列更新はこちらを使用する。
		* インスタンシング描画の場合はUpdateInstancingDataを使用する。
		*@param[in]		trans		平行移動。
		*@param[in]		rot			回転。
		*@param[in]		scale		拡大。
		*@param[in]		enUpdateAxis	fbxの上方向。
		*/
		void Update(
			const CVector3& trans, 
			const CQuaternion& rot, 
			const CVector3& scale,
			EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ
		);
		/*!
		 *@brief	インスタンシング用のデータの更新。
		 *@details
		 * インスタンシング描画の場合の行列更新はこちらを使用する。
		 * 全てのインスタンスの行列更新が完了したのち、PostUpadteInstancingDataを呼び出すように。
		 *@param[in]		trans		平行移動。
		 *@param[in]		rot			回転。
		 *@param[in]		scale		拡大。
		 *@param[in]		enUpdateAxis	fbxの上方向。

		 */
		void UpdateInstancingData(
			const CVector3& trans,
			const CQuaternion& rot,
			const CVector3& scale,
			EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ
		);
		/*!
		*@brief	インスタンスデータの更新開始時に呼び出してください。
		*/
		void BeginUpdateInstancingData()
		{
			m_numInstance = 0;
		}
		/*!
		 *@brief	全てのインスタンスデータの更新終わったら呼び出してください。
		 */
		void EndUpdateInstancingData();
		/*!
		*@brief	描画
		*@param[in] renderContext	レンダリングコンテキスト。
		*/
		void Draw(CRenderContext& renderContext);
		/*!
		*@brief	描画
		*@param[in] renderContext	レンダリングコンテキスト。
		*@param[in]	viewMatrix		ビュー行列。
		*@param[in]	projMatrix		プロジェクション行列。
		*/
		void Draw(
			CRenderContext& renderContext, 
			const CMatrix& viewMatrix, 
			const CMatrix& projMatrix
		);
		/*!
		*@brief	メッシュの検索。
		*@param[in]	findMesh		メッシュを見つけた時に呼ばれるコールバック関数
		*/
		void FindMesh(CSkinModelData::OnFindMesh findMesh) const
		{
			if (m_skinModelData != nullptr) {
				m_skinModelData->FindMesh(findMesh);
			}
		}
		/*!
		*@brief	モデルマテリアルの検索。
		*@param[in]	findEffect		マテリアルを見つけた時に呼ばれるコールバック関数
		*/
		void FindMaterial(CSkinModelData::OnFindMaterial findMaterial) const
		{
			FindMesh([&](auto& mesh) {
				CModelEffect* effect = reinterpret_cast<CModelEffect*>(mesh->effect.get());
				findMaterial(effect);
			});
		}
		/*!
		* @brief	シャドウキャスターのフラグを設定。
		*@param[in]	flag	シャドウキャスターのフラグ。
		*/
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}
		/*!
		* @brief	シャドウレシーバーのフラグを設定。
		*@param[in]	flag	シャドウレシーバーのフラグ。
		*/
		void SetShadowReceiverFlag(bool flag)
		{
			m_isShadowReceiver = flag;
		}
		/*!
		 *@brief	ワールド行列を取得。
		 */
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		*@brief	スキンモデルデータを取得。
		*/
		CSkinModelData* GetSkinModelData() const
		{
			return m_skinModelData;
		}
		/*!
		*@brief	アニメーションを設定。
		*@details
		* エンジン内部で使用されています。ユーザーは使用しないでください。
		*/
		void SetAnimation(CAnimation* animation)
		{
			m_animation = animation;
		}
	private:
		/*!
		 *@brief	ワールド行列の更新。
		 */
		void UpdateWorldMatrix(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, EnFbxUpAxis enUpdateAxis);
	private:
		struct SVSConstantBuffer {
			CMatrix mWorld;
			CMatrix mView;
			CMatrix mProj;
			CVector4 screenParam;
			int isShadowReceiver;
		};
		CAnimation* m_animation = nullptr;
		CSkinModelData*	m_skinModelData = nullptr;
		CMatrix m_worldMatrix = CMatrix::Identity;
		CConstantBuffer m_cb;			//定数バッファ。
		CShadowCaster_SkinModel m_shadowCaster;	//!<シャドウキャスター。
		bool m_isShadowCaster = false;		//!<シャドウキャスター？
		bool m_isShadowReceiver = false;	//!<シャドウレシーバー？
		CSamplerState m_samplerState;		//!<サンプラステート。@todo ひとまとめにした方がいい。
		std::unique_ptr<CMatrix[]>	m_instancingData;		//!<インスタンシング描画用のデータ。
		CStructuredBuffer	m_instancingDataSB;				//!<インスタンシング描画用のストラクチャーバッファ。
		int m_maxInstance = 1;								//!<インスタンスの最大数
		int m_numInstance = 0;								//!<インスタンスの数。
	};
}