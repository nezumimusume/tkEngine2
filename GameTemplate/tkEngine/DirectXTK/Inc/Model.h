//--------------------------------------------------------------------------------------
// File: Model.h
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#else
#include <d3d11_1.h>
#endif

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <memory>
#include <functional>
#include <set>
#include <string>
#include <vector>

#include <stdint.h>

#include <wrl\client.h>
#include "VertexTypes.h"

namespace VSD3DStarter
{
	// .CMO files

	// UINT - Mesh count
	// { [Mesh count]
	//      UINT - Length of name
	//      wchar_t[] - Name of mesh (if length > 0)
	//      UINT - Material count
	//      { [Material count]
	//          UINT - Length of material name
	//          wchar_t[] - Name of material (if length > 0)
	//          Material structure
	//          UINT - Length of pixel shader name
	//          wchar_t[] - Name of pixel shader (if length > 0)
	//          { [8]
	//              UINT - Length of texture name
	//              wchar_t[] - Name of texture (if length > 0)
	//          }
	//      }
	//      BYTE - 1 if there is skeletal animation data present
	//      UINT - SubMesh count
	//      { [SubMesh count]
	//          SubMesh structure
	//      }
	//      UINT - IB Count
	//      { [IB Count]
	//          UINT - Number of USHORTs in IB
	//          USHORT[] - Array of indices
	//      }
	//      UINT - VB Count
	//      { [VB Count]
	//          UINT - Number of verts in VB
	//          Vertex[] - Array of vertices
	//      }
	//      UINT - Skinning VB Count
	//      { [Skinning VB Count]
	//          UINT - Number of verts in Skinning VB
	//          SkinningVertex[] - Array of skinning verts
	//      }
	//      MeshExtents structure
	//      [If skeleton animation data is not present, file ends here]
	//      UINT - Bone count
	//      { [Bone count]
	//          UINT - Length of bone name
	//          wchar_t[] - Bone name (if length > 0)
	//          Bone structure
	//      }
	//      UINT - Animation clip count
	//      { [Animation clip count]
	//          UINT - Length of clip name
	//          wchar_t[] - Clip name (if length > 0)
	//          float - Start time
	//          float - End time
	//          UINT - Keyframe count
	//          { [Keyframe count]
	//              Keyframe structure
	//          }
	//      }
	// }

#pragma pack(push,1)

	struct Material
	{
		DirectX::XMFLOAT4   Ambient;
		DirectX::XMFLOAT4   Diffuse;
		DirectX::XMFLOAT4   Specular;
		float               SpecularPower;
		DirectX::XMFLOAT4   Emissive;
		DirectX::XMFLOAT4X4 UVTransform;
	};

	const uint32_t MAX_TEXTURE = 8;

	struct SubMesh
	{
		UINT MaterialIndex;
		UINT IndexBufferIndex;
		UINT VertexBufferIndex;
		UINT StartIndex;
		UINT PrimCount;
	};

	const uint32_t NUM_BONE_INFLUENCES = 4;

	static_assert(sizeof(DirectX::VertexPositionNormalTangentColorTexture) == 52, "mismatch with CMO vertex type");

	struct SkinningVertex
	{
		UINT boneIndex[NUM_BONE_INFLUENCES];
		float boneWeight[NUM_BONE_INFLUENCES];
	};

	struct MeshExtents
	{
		float CenterX, CenterY, CenterZ;
		float Radius;

		float MinX, MinY, MinZ;
		float MaxX, MaxY, MaxZ;
	};

	struct Bone
	{
		INT ParentIndex;
		DirectX::XMFLOAT4X4 InvBindPos;
		DirectX::XMFLOAT4X4 BindPos;
		DirectX::XMFLOAT4X4 LocalTransform;
	};

	struct Clip
	{
		float StartTime;
		float EndTime;
		UINT  keys;
	};

	struct Keyframe
	{
		UINT BoneIndex;
		float Time;
		DirectX::XMFLOAT4X4 Transform;
	};

#pragma pack(pop)

}; // namespace

namespace DirectX
{
    class IEffect;
    class IEffectFactory;
    class CommonStates;
    class ModelMesh;

    //----------------------------------------------------------------------------------
    // Each mesh part is a submesh with a single effect
    class ModelMeshPart
    {
    public:
        ModelMeshPart();
        virtual ~ModelMeshPart();

        uint32_t                                                indexCount;
        uint32_t                                                startIndex;
        uint32_t                                                vertexOffset;
        uint32_t                                                vertexStride;
        D3D_PRIMITIVE_TOPOLOGY                                  primitiveType;
        DXGI_FORMAT                                             indexFormat;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>               inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                    indexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                    vertexBuffer;
        std::shared_ptr<IEffect>                                effect;
        std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>>  vbDecl;
        bool                                                    isAlpha;

        typedef std::vector<std::unique_ptr<ModelMeshPart>> Collection;

        // Draw mesh part with custom effect
        void __cdecl Draw( _In_ ID3D11DeviceContext* deviceContext, _In_ IEffect* ieffect, _In_ ID3D11InputLayout* iinputLayout,
                           _In_opt_ std::function<void __cdecl()> setCustomState = nullptr, int numInstance = 1 ) const;

        // Create input layout for drawing with a custom effect.
        void __cdecl CreateInputLayout( _In_ ID3D11Device* d3dDevice, _In_ IEffect* ieffect, _Outptr_ ID3D11InputLayout** iinputLayout ) const;

        // Change effect used by part and regenerate input layout (be sure to call Model::Modified as well)
        void __cdecl ModifyEffect( _In_ ID3D11Device* d3dDevice, _In_ std::shared_ptr<IEffect>& ieffect, bool isalpha = false );
    };


    //----------------------------------------------------------------------------------
    // A mesh consists of one or more model mesh parts
    class ModelMesh
    {
    public:
        ModelMesh();
        virtual ~ModelMesh();

        BoundingSphere              boundingSphere;
        BoundingBox                 boundingBox;
        ModelMeshPart::Collection   meshParts;
        std::wstring                name;
        bool                        ccw;
        bool                        pmalpha;

        typedef std::vector<std::shared_ptr<ModelMesh>> Collection;

        // Setup states for drawing mesh
        void __cdecl PrepareForRendering( _In_ ID3D11DeviceContext* deviceContext, const CommonStates& states, bool alpha = false, bool wireframe = false ) const;

        // Draw the mesh
        void XM_CALLCONV Draw( _In_ ID3D11DeviceContext* deviceContext, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection,
                               bool alpha = false, _In_opt_ std::function<void __cdecl()> setCustomState = nullptr, int numInstance = 1 ) const;
    };


    //----------------------------------------------------------------------------------
    // A model consists of one or more meshes
    class Model
    {
    public:
		static const char* NOT_BUILD_SKELETON_EXCEPTION_MESSAGE;	//スケルトンが構築されていないときの例外メッセージ。
		static const char* NOT_LOADED_CMO_EXCEPTION_ESSAGE;			//CMOファイルのロードに失敗した時の例外メッセージ。
		static const char* NOT_ASSIGN_MATERIAL_TO_MESH;				//メッシュにマテリアルが割り当てられていないときの例外メッセージ。

		typedef std::function<void(	const wchar_t* boneName, const VSD3DStarter::Bone* bone, std::vector<int>& localBoneIDtoGlobalBoneIDTbl)> OnFindBoneData;
        virtual ~Model();

        ModelMesh::Collection   meshes;
        std::wstring            name;

        // Draw all the meshes in the model
        void XM_CALLCONV Draw( _In_ ID3D11DeviceContext* deviceContext, const CommonStates& states, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection,
                               bool wireframe = false, _In_opt_ std::function<void __cdecl()> setCustomState = nullptr, int numInstance = 1 ) const;

        // Notify model that effects, parts list, or mesh list has changed
        void __cdecl Modified() { mEffectCache.clear(); }

        // Update all effects used by the model
        void __cdecl UpdateEffects( _In_ std::function<void __cdecl(IEffect*)> setEffect );

        // Loads a model from a Visual Studio Starter Kit .CMO file
        static std::unique_ptr<Model> __cdecl CreateFromCMO( _In_ ID3D11Device* d3dDevice, _In_reads_bytes_(dataSize) const uint8_t* meshData, size_t dataSize,
                                                             _In_ IEffectFactory& fxFactory, bool ccw = true, bool pmalpha = false, 
																  OnFindBoneData onFindBoneData = nullptr);
        static std::unique_ptr<Model> __cdecl CreateFromCMO( _In_ ID3D11Device* d3dDevice, _In_z_ const wchar_t* szFileName,
                                                             _In_ IEffectFactory& fxFactory, bool ccw = true, bool pmalpha = false, 
			                                                      OnFindBoneData onFindBoneData = nullptr);

        // Loads a model from a DirectX SDK .SDKMESH file
        static std::unique_ptr<Model> __cdecl CreateFromSDKMESH( _In_ ID3D11Device* d3dDevice, _In_reads_bytes_(dataSize) const uint8_t* meshData, _In_ size_t dataSize,
                                                                 _In_ IEffectFactory& fxFactory, bool ccw = false, bool pmalpha = false );
        static std::unique_ptr<Model> __cdecl CreateFromSDKMESH( _In_ ID3D11Device* d3dDevice, _In_z_ const wchar_t* szFileName,
                                                                 _In_ IEffectFactory& fxFactory, bool ccw = false, bool pmalpha = false );

        // Loads a model from a .VBO file
        static std::unique_ptr<Model> __cdecl CreateFromVBO( _In_ ID3D11Device* d3dDevice, _In_reads_bytes_(dataSize) const uint8_t* meshData, _In_ size_t dataSize,
                                                             _In_opt_ std::shared_ptr<IEffect> ieffect = nullptr, bool ccw = false, bool pmalpha = false );
        static std::unique_ptr<Model> __cdecl CreateFromVBO( _In_ ID3D11Device* d3dDevice, _In_z_ const wchar_t* szFileName, 
                                                             _In_opt_ std::shared_ptr<IEffect> ieffect = nullptr, bool ccw = false, bool pmalpha = false );

    private:
        std::set<IEffect*>  mEffectCache;
    };
 }