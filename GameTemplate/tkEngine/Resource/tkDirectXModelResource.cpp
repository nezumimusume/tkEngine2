#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/resource/tkDirectXModelResource.h"
#include "tkEngine/graphics/tkSkinModelData.h"

namespace tkEngine {
	/*!
	*@brief
	*  エフェクトファクトリ。
	*/
	class CSkinModelEffectFactory : public DirectX::EffectFactory {
	public:
		CSkinModelEffectFactory(ID3D11Device* device) :
			EffectFactory(device) {}
		std::shared_ptr<DirectX::IEffect> __cdecl CreateEffect(const EffectInfo& info, ID3D11DeviceContext* deviceContext)override
		{
			std::shared_ptr<CModelEffect> effect;
			if (info.enableSkinning) {
				//スキニングあり。
				effect = std::make_shared<CSkinModelEffect>();
			}
			else {
				//スキニングなし。
				effect = std::make_shared<CNonSkinModelEffect>();
			}
			effect->SetMatrialName(info.name);
			if (info.diffuseTexture && *info.diffuseTexture)
			{
				ID3D11ShaderResourceView* texSRV;
				DirectX::EffectFactory::CreateTexture(info.diffuseTexture, deviceContext, &texSRV);
				effect->SetDiffuseTexture(texSRV);
			}
			return effect;
		}

		void __cdecl CreateTexture(const wchar_t* name, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView** textureView) override
		{
			return DirectX::EffectFactory::CreateTexture(name, deviceContext, textureView);
		}
	};

	bool CDirectXModelResource::m_isInstanced = false;
	CDirectXModelResource::CDirectXModelResource()
	{
		TK_ASSERT(m_isInstanced == false, "CDirectXModelDataManagerのインスタンスを複数作ることはできません。");
		m_isInstanced = true;
	}
	CDirectXModelResource::~CDirectXModelResource()
	{
		m_isInstanced = false;
	}
	DirectX::Model* CDirectXModelResource::Load(const wchar_t* filePath, CSkeleton& skeleton)
	{
		auto nameKey = CUtil::MakeHash(filePath);
		auto it = m_resourceMap.find(nameKey);
		if (it != m_resourceMap.end()) {
			//使いまわす。
			return it->second.get();
		}
		//新規。
		auto effectFactory = CSkinModelEffectFactory(GraphicsEngine().GetD3DDevice());

		//ボーンを発見したときのコールバック。
		auto onFindBone = [&](
			const wchar_t* boneName,
			const VSD3DStarter::Bone* bone,
			std::vector<int>& localBoneIDtoGlobalBoneIDTbl
			) {
			int globalBoneID = skeleton.FindBoneID(boneName);
			if (globalBoneID == -1) {
				TK_WARNING("BoneID wasn't found!!!!");
				return;
			}
			localBoneIDtoGlobalBoneIDTbl.push_back(globalBoneID);
		};
		try {
			auto model = DirectX::Model::CreateFromCMO(
				GraphicsEngine().GetD3DDevice(),
				filePath,
				effectFactory,
				false,
				false,
				onFindBone
			);
			auto pRawModel = model.get();
			m_resourceMap.insert(std::pair<int, DirectXModelPtr>(nameKey, std::move(model)));
			return pRawModel;
		}
		catch (std::exception& exception) {
			if (strcmp(exception.what(), DirectX::Model::NOT_BUILD_SKELETON_EXCEPTION_MESSAGE) == 0) {
				//スケルトンが構築されていないエラーが起きた。
				char cFilePath[256];
				wcstombs(cFilePath, filePath, 255);
				char cTksFilePath[256];
				strcpy(cTksFilePath, cFilePath);
				auto ext = strstr(cTksFilePath, ".cmo");
				ext[1] = 't';
				ext[2] = 'k';
				ext[3] = 's';
				TK_WARNING_MESSAGE_BOX("%s\n"
					"スケルトンデータのロードに失敗しました。\n"
					"Assetsフォルダを確認して、%sと同じ場所に%sが存在するか確認してください。\n"
					"もし存在している場合は、tksファイルの出力ミスが考えられます。\n", exception.what(), cFilePath, cTksFilePath);
			}
			else if (strcmp(exception.what(), DirectX::Model::NOT_LOADED_CMO_EXCEPTION_ESSAGE) == 0) {
				//CMOファイルの読み込みエラーが起きた。
				char cFilePath[256];
				wcstombs(cFilePath, filePath, 255);
				TK_WARNING_MESSAGE_BOX("%s\n"
					"cmoファイルのロードに失敗しました。\n"
					"Assetsフォルダを確認して、ファイル名を間違っていないか確認をお願いします。\n"
					"ロードしようとしたファイル ＝　%s\n"
					, exception.what(), cFilePath);
			}
			else if (strcmp(exception.what(), DirectX::Model::NOT_ASSIGN_MATERIAL_TO_MESH) == 0) {
				//CMOファイルの読み込みエラーが起きた。
				char cFilePath[256];
				wcstombs(cFilePath, filePath, 255);
				TK_WARNING_MESSAGE_BOX("%s\n"
					"cmoファイルのロードに失敗しました。\n"
					"マテリアルが割り当てられていないポリゴンが存在します。\n"
					"モデルデータの確認をお願いします。\n"
					"ロードしようとしたファイル ＝　%s\n"
					, exception.what(), cFilePath);
			}
			else {
				//それ以外
				TK_WARNING_MESSAGE_BOX("%s\n", exception.what());
			}
		}
		return nullptr;
		
	}
}