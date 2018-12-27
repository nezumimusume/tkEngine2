/*!
 *@brief	スキンモデルデータ。
 */

#pragma once

#include "tkEngine/graphics/tkSkeleton.h"

namespace tkEngine{
	class CAnimationClip;
	typedef std::unique_ptr<CAnimationClip>	CAnimationClipPtr;
	/*!
	*@brief	モデルエフェクト。
	*/
	class CModelEffect : public DirectX::IEffect {
	protected:
		std::wstring m_materialName;	//!<マテリアル名。
		CShader* m_pVSShader = nullptr;
		CShader* m_pPSShader = nullptr;
		CShader m_vsShader;
		CShader m_psShader;
		CShader m_vsShaderInstancing;		//!<頂点シェーダー。インスタンシング用。
		CShader m_vsRenderToDepthShader;	//!<Z値書き込み用の頂点シェーダー。
		CShader m_vsRenderToDepthShaderInstancing;	//!<Z値書き込み用の頂点シェーダー。インスタンシング用。
		CShader m_psRenderToDepthShader;	//!<Z値書き込み用のピクセルシェーダー。
		CShader m_psRenderToGBufferShader;	//!<G-Buffer書き込み用のピクセルシェーダー。
		
		ID3D11ShaderResourceView* m_diffuseTex = nullptr;
		ID3D11ShaderResourceView* m_normalMap = nullptr;
		ID3D11ShaderResourceView* m_specularMap = nullptr;
		ID3D11ShaderResourceView* m_skyCubeMap = nullptr;
		bool isSkining;
		/*!
		* @brief	マテリアルパラメータ。
		* @details 
		* こいつを変更する場合は、modelCB.hのMaterialParamCbも変更するように。
		*/
		struct MaterialParam {
			int hasNormalMap;				//!<法線マップある？
			int hasSpecularMap;				//!<スペキュラマップある？
			float anisotropic;				//!<異方性反射。
			unsigned int materialID = 0;	//!<マテリアルID。
		};
		static const int NUM_POINT_LIGHT = 1024;
		MaterialParam m_materialParam;				//マテリアルパラメータ。
		CConstantBuffer m_materialParamCB;			//マテリアルパラメータ用の定数バッファ。
		CRenderContext* m_renderContext = nullptr;	//レンダリングコンテキスト。
		int m_numInstance = 1;						//描画するインスタンスの数。
	public:
		CModelEffect()
		{
			m_materialParam.anisotropic = 0.5f;
			m_materialParamCB.Create(&m_materialParam, sizeof(m_materialParam));
			m_psShader.Load("shader/model.fx", "PSMain", CShader::EnType::PS);
			m_psRenderToDepthShader.Load("shader/model.fx", "PSMain_RenderDepth", CShader::EnType::PS);
			m_psRenderToGBufferShader.Load("shader/model.fx", "PSMain_RenderGBuffer", CShader::EnType::PS);
			m_pPSShader = &m_psShader;
		}
		virtual ~CModelEffect()
		{
			if (m_diffuseTex) {
				m_diffuseTex->Release();
			}
			if (m_normalMap) {
				m_normalMap->Release();
			}
			if (m_specularMap) {
				m_specularMap->Release();
			}
			if (m_skyCubeMap) {
				m_skyCubeMap->Release();
			}
		}
		void __cdecl Apply(ID3D11DeviceContext* deviceContext) override;
		
		void __cdecl GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override
		{
			*pShaderByteCode = m_vsShader.GetByteCode();
			*pByteCodeLength = m_vsShader.GetByteCodeSize();
		}
		void SetDiffuseTexture(ID3D11ShaderResourceView* tex)
		{
			m_diffuseTex = tex;
		}
		void SetNormalMap(CShaderResourceView& srv)
		{
			SetNormalMap(srv.GetBody());
		}
		void SetNormalMap(ID3D11ShaderResourceView* tex)
		{
			m_normalMap = tex;
			m_normalMap->AddRef();
		}
		void SetSpecularMap(CShaderResourceView& srv)
		{
			SetSpecularMap(srv.GetBody());
		}
		void SetSpecularMap(ID3D11ShaderResourceView* tex)
		{
			m_specularMap = tex;
			m_specularMap->AddRef();
		}
		void SetSkyCubeMap(ID3D11ShaderResourceView* tex)
		{
			m_skyCubeMap = tex;
			m_skyCubeMap->AddRef();
		}
		void SetMatrialName(const wchar_t* matName)
		{
			m_materialName = matName;
		}
		void SetRenderContext(CRenderContext& rc)
		{
			m_renderContext = &rc;
		}
		void SetNumInstance(int numInstance)
		{
			m_numInstance = numInstance;
		}
		//デフォルトのマテリアルＩＤは0
		void SetMaterialID(unsigned int materialID)
		{
			m_materialParam.materialID = materialID;
		}
		bool EqualMaterialName(const wchar_t* name) const
		{
			return wcscmp(name, m_materialName.c_str()) == 0;
		}
		/*!
		*@brief	enRenderStep_Render3DModelToSceneの時に使用される頂点シェーダーを設定。
		*  
		*/
		void SetRender3DModelVSShader(CShader& vsShader)
		{
			m_pVSShader = &vsShader;
		}
		/*!
		*@brief	enRenderStep_Render3DModelToSceneの時に使用されるピクセルシェーダーを設定。
		*
		*/
		void SetRender3DModelPSShader(CShader& psShader)
		{
			m_pPSShader = &psShader;
		}
		/*!
		*@brief	enRenderStep_Render3DModelToSceneの時に使用されるシェーダーをデフォルトシェーダーにする。
		*
		*/
		void SetRender3DModelDefaultShader()
		{
			m_pVSShader = &m_vsShader;
			m_pPSShader = &m_psShader;
		}
	};
	/*!
	*@brief
	*  スキンなしモデルエフェクト。
	*/
	class CNonSkinModelEffect : public CModelEffect {
	public:
		CNonSkinModelEffect()
		{
			m_vsShader.Load("shader/model.fx", "VSMain", CShader::EnType::VS);
			m_vsRenderToDepthShader.Load("shader/model.fx", "VSMain_RenderDepth", CShader::EnType::VS);
			m_vsRenderToDepthShaderInstancing.Load("shader/model.fx", "VSMainInstancing_RenderDepth", CShader::EnType::VS);
			m_vsShaderInstancing.Load("shader/model.fx", "VSMainInstancing", CShader::EnType::VS);
			m_pVSShader = &m_vsShader;
			isSkining = false;
		}
	};
	/*!
	*@brief
	*  スキンモデルエフェクト。
	*/
	class CSkinModelEffect : public CModelEffect {
	public:
		CSkinModelEffect()
		{
			m_vsShader.Load("shader/model.fx", "VSMainSkin", CShader::EnType::VS);
			m_vsRenderToDepthShader.Load("shader/model.fx", "VSMainSkin_RenderDepth", CShader::EnType::VS);
			m_pVSShader = &m_vsShader;
			isSkining = true;
		}
	};

	/*!
	 *@brief	スキンモデルデータ。
	 */
	class CSkinModelData : Noncopyable{
	public:
		
		/*!
		 *@brief	コンストラクタ。
		 */
		CSkinModelData();
		/*!
		 *@brief	デストラクタ。
		 */
		~CSkinModelData();
		/*!
		 *@brief	ロード。
		 *@param[in]	filePath	ファイルパス。
		 *@return	trueが返ってきたらロード成功。
		 */
		bool Load( const wchar_t* filePath );
		/*!
		*@brief	インスタンスが利用可能かどうか調べる。
		*/
		bool IsAvailable() const
		{
			return m_isAvailable;
		}
		DirectX::Model& GetBody()
		{
			return *m_modelDx;
		}
		/*!
		 *@brief	スケルトンを取得。
		 */
		CSkeleton& GetSkeleton()
		{
			return m_skeleton;
		}
		/*!
		*@brief	スケルトンを取得。
		*/
		const CSkeleton& GetSkeleton() const
		{
			return m_skeleton;
		}
		typedef std::function<void(std::unique_ptr<DirectX::ModelMeshPart>&)>		OnFindMesh;
		/*!
		 *@brief	メッシュの検索。
		 *@param[in]	findMesh		メッシュを見つけた時に呼ばれるコールバック関数
		 */
		void FindMesh(OnFindMesh findMesh) const;
	private:
		std::wstring m_filePath;
		DirectX::Model* m_modelDx = nullptr;
		CSkeleton	m_skeleton;
		bool m_isAvailable = false;	//インスタンスが利用可能かどうかのフラグ。
	};
}