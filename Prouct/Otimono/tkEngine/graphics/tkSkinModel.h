/*!
 *@brief	スキンモデル。
 */

#pragma once

#include "tkEngine/graphics/preRender/tkShadowCaster.h"
#include "tkEngine/math/tkBox.h"

namespace tkEngine{
	class CSkinModelData;
	/*!
	*@brief	FBXの上方向。
	*/
	enum EnFbxUpAxis {
		enFbxUpAxisY,		//Y-up
		enFbxUpAxisZ,		//Z-up
	};
	/*!
	 *@brief	スキンモデル。
	 *@details
	 * スキンあり、なしモデルの表示をサポートするクラス。</br>
	 * 複数のインスタンスを一度の関数コールで描画することができるインスタンシング描画もサポートしている。</br>
	 */
	class CSkinModel : Noncopyable{
	public:
		//描画を行う前に呼ばれる関数オブジェクトの型。
		using OnDrawFookFunction = std::function<void(CRenderContext&, CSkinModel&)>;
		
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
		*@param[in]		isForwardRender	フォワードレンダリングを行う？
		*/
		void Update(
			const CVector3& trans, 
			const CQuaternion& rot, 
			const CVector3& scale,
			EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ,
			bool isForwardRender = false
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
			if (m_maxInstance > 1) {
				m_numInstance = 0;
			}
		}
		/*!
		 *@brief	全てのインスタンスデータの更新終わったら呼び出してください。
		 *@param[in]		isForwardRender	フォワードレンダリングを行う？
		 */
		void EndUpdateInstancingData(bool isForwardRender = false);
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
		void FindMaterial(std::function<void(CModelEffect*)> findMaterial) const
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
		void UpdateBoundingBox(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ)
		{
			CMatrix mWorld;
			CMatrix mBias = CMatrix::Identity;
			if (enUpdateAxis == enFbxUpAxisZ) {
				//Z-up
				mBias.MakeRotationX(CMath::PI * -0.5f);
			}
			CMatrix mScale, mTrans, mRot;
			mScale.MakeScaling(scale);
			mRot.MakeRotationFromQuaternion(rot);
			mRot = mBias * mRot;
			mTrans.MakeTranslation(trans);
			mWorld = mScale * mRot;
			mWorld = mWorld * mTrans;

			m_boundingBox.Update(mWorld);
		}
		/*!
		*@brief	バウンディングボックスの更新。
		*/
		void UpdateBoundingBox()
		{
			m_boundingBox.Update(m_worldMatrix);
		}
		/*!
		*@brief	バウンディングボックス。
		*/
		CBox& GetBoundingBox()
		{
			return m_boundingBox;
		}
		/*!
		* @brief	描画コール実行の直前にフックしたい関数を設定する。
		*param[in]	func	フック関数。
		*@code
			//サンプルコード
			skinModelRender->SetPreDrawFookFunction([&]( CRenderContext& rc, CSkinModel& model){
				printf("描画コールの直前に呼ばれるよ");
			});
		*@endcode
		*/
		void SetPreDrawFookFunction(std::function<void(CRenderContext&, CSkinModel&)> func)
		{
			m_preDrawFookFunction = func;
		}
		/*!
		* @brief	描画コール実行の直後にフックしたい関数を設定する。
		*param[in]	func	フック関数。
		*@code
			//サンプルコード
			skinModelRender->SetPreDrawFookFunction([&]( CRenderContext& rc, CSkinModel& model){
				printf("描画コールの直後に呼ばれるよ");
			});
		*@endcode
		*/
		void SetPostDrawFookFunction(OnDrawFookFunction func)
		{
			m_postDrawFookFunction = func;
		}
		/// <summary>
		/// 自己発光色を設定。
		/// </summary>
		/// <param name="color">自己発光色</param>
		void SetEmissionColor(CVector3 color)
		{
			m_emissionColor = color;
		}
		/// <summary>
		/// シャドウキャスターを取得。
		/// </summary>
		/// <returns>シャドウキャスターの参照</returns>
		CShadowCaster_SkinModel& GetShadowCaster()
		{
			return m_shadowCaster;
		}
	private:
		/*!
		*@brief バウンディングボックスの初期化。
		*/
		void InitBoudingBox();
		/*!
		 *@brief	ワールド行列の更新。
		 */
		void UpdateWorldMatrix(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, EnFbxUpAxis enUpdateAxis);
	private:
		/*!
		 *@brief	頂点シェーダーで使用する定数バッファ用の構造体。
		 */
		struct SVSConstantBuffer {
			CMatrix mWorld;			//!<ワールド行列。
			CMatrix mView;			//!<ビュー行列。
			CMatrix mProj;			//!<プロジェクション行列。
			CVector3 emissionColor;	//!<自己発光色。
			int isShadowReceiver;	//!<シャドウレシーバーフラグ。
		};
		CAnimation* m_animation = nullptr;						//!<アニメーション再生処理。
		CSkinModelData*	m_skinModelData = nullptr;				//!<スキンモデルデータ。
		CMatrix m_worldMatrix = CMatrix::Identity;				//!<ワールド行列。
		CConstantBuffer m_cb;									//!<定数バッファ。
		CShadowCaster_SkinModel m_shadowCaster;					//!<シャドウキャスター。
		bool m_isShadowCaster = false;							//!<シャドウキャスター？
		bool m_isShadowReceiver = false;						//!<シャドウレシーバー？
		std::unique_ptr<CMatrix[]>	m_instancingData;			//!<インスタンシング描画用のデータ。
		CStructuredBuffer	m_instancingDataSB;					//!<インスタンシング描画用のストラクチャーバッファ。
		int m_maxInstance = 1;									//!<インスタンスの最大数
		int m_numInstance = 0;									//!<インスタンスの数。
		CBox m_boundingBox;										//!<バウンディングボックス。
		OnDrawFookFunction m_preDrawFookFunction = nullptr;		//描画の直前に呼ばれるフック関数。
		OnDrawFookFunction m_postDrawFookFunction = nullptr;	//描画が終わった後で呼ばれるフック関数。
		CVector3 m_emissionColor = CVector3::Zero;				//自己発光カラー。
	};
}