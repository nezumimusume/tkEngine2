/*!
 * @brief	スキンモデルレンダラー
 */

#pragma once

#include "tkEngine/culling/tkObjectFrustumCulling.h"

namespace tkEngine{
namespace prefab{
	/*!
	 * @brief	3Dモデルを表示する機能を提供するクラス。
	 *@details
	 * このクラスはIGameObjectを継承しているため、NewGOを使用して、\n
	 * インスタンスを生成する必要があります。\n
	 * インスタンスが不要になったらDeleteGOを使用して破棄してください。
	 *@code
		
		//インスタンスを生成～絵を出すまでのサンプルコード。
		//まずはNewGOを実行。
		prefab::CSkinModelRender* render = NewGO<prefab::CSkinModelRender>(0);

		//続いてファイルパスを指定してロードすれば、以降のフレームで3Dモデルが表示されるようになります。
		render->Init( L"modelData/test.cmo" );

	 *@endcode
	 */
	class CSkinModelRender : public IGameObject {
	public:
		
		/*!
		 * @brief	コンストラクタ。
		 */
		CSkinModelRender() {}
		/// <summary>
		/// DeleteGOが実行されたときに呼ばれる。
		/// </summary>
		void OnDestroy() override;
		/*!
		* @brief	初期化。
		*@param[in]	filePath			ロードしたいcmoファイルのファイルパス。
		*@param[in]	animationClips		アニメーションクリップの配列の先頭アドレス
		*@param[in]	numAnimClip			アニメーションクリップの数
		*@param[in]	enUpdateAxis		fbxの上方向。
		*@param[in] maxInstance			インスタンスの数。インスタンシングを行いたい場合に指定してください。
		*/
		void Init(const wchar_t* filePath,
			CAnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
			int maxInstance = 1 );
		
		/*!
		*@brief	描画の直前に呼び出されるフック関数オブジェクトを設定する。
		*@details
		* 描画の直前に特殊な処理を行いたい場合に使用してください。
		*@param[in]	func	フック関数オブジェクト。
		*@code
			//サンプルコード
			skinModelRender->SetPreDrawFookFunction([&]( CRenderContext& rc, CSkinModel& model){
				printf("描画コールの直前に呼ばれるよ");
			});
		*@endcode
		*/
		void SetPreDrawFookFunction(CSkinModel::OnDrawFookFunction func)
		{
			m_skinModel.SetPreDrawFookFunction(func);
		}
		/*!
		*@brief	描画が終わった後にフックしたい関数を設定する。
		*@details
		* 描画の直後に特殊な処理を行いたい場合に使用してください。
		*@param[in]	func	フック関数。例 void Fook(CRenderContext& rc, CSkinModel& model);
		*@code
			//サンプルコード
			skinModelRender->SetPostDrawFookFunction([&]( CRenderContext& rc, CSkinModel& model){
				printf("描画コールの直後に呼ばれるよ");
			});
		*@endcode
		*/
		void SetPostDrawFookFunction(CSkinModel::OnDrawFookFunction func)
		{
			m_skinModel.SetPostDrawFookFunction(func);
		}
	
		/*!
		* @brief	アニメーションを再生。
		*@param[in]	clipNo	アニメーションクリップの番号。コンストラクタに渡したanimClipListの並びとなる。
		*@param[in]	interpolateTime		補完時間(単位：秒)
		*/
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/*!
		* @brief	アニメーションの再生中？
		*@retval	true		アニメーションを再生中。
		*@retval	false		アニメーションは再生していない。
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/*!
		* @brief	シャドウキャスターのフラグを設定。
	    * @details
		* シャドウキャスターとは、影を落とす側のモデルのことです。\n
		* 例えば、プレイヤーの影を地面に落としたい場合は\n
		* プレイヤーはシャドウキャスターとなります。
		*@param[in]	flag	シャドウキャスターのフラグ。
		*@details
		*この引数にtrueが渡された場合、このインスタンスが描画するモデルはシャドウキャスターになります。
		*/
		void SetShadowCasterFlag(bool flag)
		{
			m_skinModel.SetShadowCasterFlag(flag);
		}
		/*!
		* @brief	シャドウレシーバーのフラグを設定。
		* @details
		* シャドウキャスターとは、影を落とされる側のモデルのことです。\n
		* 例えば、プレイヤーの影を地面に落としたい場合は\n
		* 地面ははシャドウレシーバーとなります。
		*@param[in]	flag	シャドウレシーバーのフラグ。
		*@details
		*この引数にtrueが渡された場合、このインスタンスが描画するモデルはシャドウレシーバーになります。
		*/
		void SetShadowReceiverFlag(bool flag)
		{
			m_skinModel.SetShadowReceiverFlag(flag);
		}
		/*!
		* @brief	自己発光色を設定。
		*@param[in]	color	自己発光色。
		*/
		void SetEmissionColor(CVector3 color)
		{
			m_skinModel.SetEmissionColor(color);
		}
		/*!
		*@brief	フリーズさせているボーンの平行移動量を取得。
		*/
		CVector3 GetFreezeBoneTranslate() const
		{
			return m_animation.GetFreezeBoneTranslate();
		}
		/*!
		*@brief	アニメーションイベントを監視する関数を登録。
		*@details
		* アニメーションイベントが打ち込まれたフレームまでアニメーションを\n
		* 再生すると、登録されている関数が呼び出されます。
		*@param[in]		eventListener		イベントを監視する関数オブジェクト。
		*/
		void AddAnimationEventListener(std::function<void(const wchar_t* clipName, const wchar_t* eventName)> eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/*!
		*@brief	座標を設定。
		*@details
		* 3Dモデルを表示したいワールド座標を指定してください。
		*@param[in]	position		ワールド座標。
		*/
		void SetPosition(const CVector3& position)
		{
			m_position = position;
		}
		/*!
		*@brief	回転を設定。
		*@details
		* 3Dモデルのワールド空間での回転を表す、クォータニオンを指定してください。
		*@param[in]	rotation		ワールド空間での回転クォータニオン。
		*/
		void SetRotation(const CQuaternion& rotation)
		{
			m_rotation = rotation;
		}
		/*!
		*@brief	拡大率を設定。
		*@details
		* 3Dモデルのワールド空間での拡大率を指定してください。
		*@param[in] scale			ワールド空間での拡大率。( 1, 1, 1 )で等倍。
		*/
		void SetScale(const CVector3& scale)
		{
			m_scale = scale;
		}
		/*!
		*@brief	ワールド空間での座標、回転、拡大をすべて設定
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*@param[in] scale		拡大。
		*/
		void SetPRS(const CVector3& trans, const CQuaternion& rot, const CVector3& scale)
		{
			SetPosition(trans);
			SetRotation(rot);
			SetScale(scale);
		}
		/*!
		*@brief	インスタンシングデータを更新。
		*@details
		* インスンタシング描画を行っている場合は、この関数を使って\n
		* ワールド空間での座標、回転、拡大を指定してください。\n
		* 例えば、100体でインスタンシング描画を行っている場合は\n
		* １フレームに100回、この関数を呼び出す必要があります。
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*@param[in] scale		拡大。
		*/
		void UpdateInstancingData(const CVector3& trans, const CQuaternion& rot, const CVector3& scale)
		{
			m_skinModel.UpdateInstancingData(trans, rot, scale);
		}
		/*!
		*@brief	スキンモデルデータを取得。
		*@return	スキンモデルデータ。
		*/
		CSkinModelData& GetSkinModelData() 
		{
			return m_skinModelData;
		}
		/*!
		*@brief	スキンモデルデータを取得。const参照版。
		*@return	スキンモデルデータ。
		*/
		const CSkinModelData& GetSkinModelData() const
		{
			return m_skinModelData;
		}
		/*!
		*@brief	スキンモデル取得。
		*@return	スキンモデル。
		*/
		CSkinModel& GetSkinModel()
		{
			return m_skinModel;
		}
		/*!
		*@brief	スキンモデル取得。const参照版。
		*@return	スキンモデル。
		*/
		const CSkinModel& GetSkinModel() const
		{
			return m_skinModel;
		}
		/*!
		*@brief	視推台カリングを行うかどうかのフラグを設定。
		*@details
		* このフラグをtrueにすると画面に映っていないオブジェクトは表示されなくなります。
		*/
		void SetFrustumCulling(bool flag)
		{
			m_isFrustumCulling = flag;
		}
		/*!
		*@brief	モデルマテリアルの検索。
		*@param[in]	findEffect		マテリアルを見つけた時に呼ばれるコールバック関数
		*/
		void FindMaterial(std::function<void(CModelEffect*)> findMaterial) const
		{
			m_skinModel.FindMaterial(findMaterial);
		}
		/*!
		*@brief	アニメーションの更新フラグを設定。
		*@param flag	更新フラグ
		*@details
		*このフラグにfalseを設定すると、アニメーションの再生が止まります。
		*/
		void SetUpdateAnimationFlag(bool flag)
		{
			m_isUpdateAnimation = flag;
		}
		/*!
		*@brief	フォワードレンダリングのフラグを設定。
		*@details
		* フラグを立てるとフォワードレンダリングを行います。
		* 物理ベースレンダリング以外の特殊なレンダリングを行いたい場合は有効にしてください。
		*/
		void SetForwardRenderFlag(bool flag)
		{
			m_isForwardRender = flag;
		}
		void FindVertexPosition(std::function<void(CVector3* pos)>);
		/*!
		*@brief	ボーンを検索する。
		*@param[in]	boneName	ボーンの名前。
		*@return	見つかったボーンのアドレス。見つからなかった場合はnullptrを返します。
		*@details
		* この関数は検索を行うため、処理が重いです。
		* Update関数などで毎フレーム呼び出すのではなく、
		* 初期化関数などで、クラスのメンバ変数にキャッシュするのをオススメします。
		*/
		CBone* FindBone(const wchar_t* boneName) const
		{
			const auto& skeleton = m_skinModelData.GetSkeleton();
			auto boneId = skeleton.FindBoneID(boneName);
			if (boneId != -1) {
				return skeleton.GetBone(boneId);
			}
			return nullptr;

		}
	private:
		void ForwardRender(CRenderContext& rc) override final;
		/*!
		* @brief	更新前に呼ばれる関数。
		*/
		bool Start() override final;
		/*!
		* @brief	事前更新。
		*/
		void PreUpdate() override final
		{
			if (m_skinModelData.IsAvailable() == true) {
				m_skinModel.BeginUpdateInstancingData();
			}
		}
		/*!
		* @brief	更新。
		*/
		void Update() override final;
		/*!
		* @brief	遅延更新。
		*/
		void PostUpdate() override final
		{
			if (m_skinModelData.IsAvailable() == true) {
				m_skinModel.EndUpdateInstancingData();
			}
		}
		
		/*!
		* @brief	アニメーションの初期化。
		*/
		void InitAnimation(CAnimationClip* animationClips, int numAnimationClips);
	private:
		bool						m_isForwardRender = false;		//!<フォワードレンダリング。
		CAnimationClip*				m_animationClips = nullptr;			//!<アニメーションクリップ。
		int							m_numAnimationClips = 0;			//!<アニメーションクリップの数。
		CVector3 					m_position = CVector3::Zero;		//!<座標。
		CQuaternion	 				m_rotation = CQuaternion::Identity;	//!<回転。
		CVector3					m_scale = CVector3::One;			//!<拡大率。
		EnFbxUpAxis					m_enFbxUpAxis = enFbxUpAxisZ;		//!<FBXの上方向。
		CAnimation					m_animation;						//!<アニメーション。
		CSkinModel					m_skinModel;						//!<スキンモデル。
		CSkinModelData				m_skinModelData;					//!<スキンモデルデータ。
		const std::wstring			m_filePath;							//!<ファイルパス。
		bool						m_isFrustumCulling = false;			//!<フラスタムカリングを行う？
		CObjectFrustumCulling		m_frustumCulling;					//!<フラスタムカリング。
		bool						m_isUpdateAnimation = true;			//!<アニメーションを更新する？
	};
}
}