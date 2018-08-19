//--------------------------------------------------------------------------------------
// File: ModelLoadCMO.cpp
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

#include "pch.h"
#include "Model.h"

#include "DDSTextureLoader.h"
#include "Effects.h"
#include "VertexTypes.h"

#include "DirectXHelpers.h"
#include "PlatformHelpers.h"
#include "BinaryReader.h"


using namespace DirectX;
using Microsoft::WRL::ComPtr;


//--------------------------------------------------------------------------------------
// .CMO files are built by Visual Studio 2012 and an example renderer is provided
// in the VS Direct3D Starter Kit
// http://code.msdn.microsoft.com/Visual-Studio-3D-Starter-455a15f1
//--------------------------------------------------------------------------------------

namespace {
	struct IBData
	{
		size_t          nIndices;
		const USHORT*   ptr;
	};
	struct VBData
	{
		size_t                                          nVerts;
		const VertexPositionNormalTangentColorTexture*  ptr;
		const VSD3DStarter::SkinningVertex*             skinPtr;
	};

	/*!
	 *@details 異常が発生している接ベクトルを再計算する。
	 * FBX->cmo変換の時にテクスチャのUV空間を使って、法線を計算しているっぽいのだけど
	 * UV座標のuとvのどちらか、あるいは両方に変化がないときに、接ベクトルが0になっている。
	 */
	void ComputeTangent(std::vector<VBData>& vbData, const std::vector<IBData>& ibData)
	{
		//異常な接ベクトルに補正をかける。
		for (auto& vb : vbData) {
			for (int i = 0; i < vb.nVerts; i++) {
				auto& vert = const_cast<VertexPositionNormalTangentColorTexture&>(vb.ptr[i]);
				float len = sqrt(vert.tangent.x * vert.tangent.x + vert.tangent.y * vert.tangent.y + vert.tangent.z * vert.tangent.z);
				if (len < FLT_EPSILON) {
					XMFLOAT4 tangent;
					//接ベクトルが計算されていない。
					if (fabsf(vert.tangent.y) > 0.9998f) {
						//ほぼY軸を向いている。
						tangent.x = 1.0f;
						tangent.y = 0.0f;
						tangent.z = 0.0f;
						tangent.w = 0.0f;
					}
					else {
						//それ以外はY軸との外積、
						tangent.x = -vert.normal.z;
						tangent.y = 0.0f;
						tangent.z = - vert.normal.x;
						len = std::max(0.001f, sqrt(tangent.x * tangent.x + tangent.y * tangent.y + vert.tangent.z * tangent.z));
						tangent.x /= len;
						tangent.z /= len;
					}
					vert.tangent = tangent;
				}
			}
		}
	}

}
static_assert( sizeof(VSD3DStarter::Material) == 132, "CMO Mesh structure size incorrect" );
static_assert( sizeof(VSD3DStarter::SubMesh) == 20, "CMO Mesh structure size incorrect" );
static_assert( sizeof(VSD3DStarter::SkinningVertex)== 32, "CMO Mesh structure size incorrect" );
static_assert( sizeof(VSD3DStarter::MeshExtents)== 40, "CMO Mesh structure size incorrect" );
static_assert( sizeof(VSD3DStarter::Bone) == 196, "CMO Mesh structure size incorrect" );
static_assert( sizeof(VSD3DStarter::Clip) == 12, "CMO Mesh structure size incorrect" );
static_assert( sizeof(VSD3DStarter::Keyframe)== 72, "CMO Mesh structure size incorrect" );

//--------------------------------------------------------------------------------------
struct MaterialRecordCMO
{
    const VSD3DStarter::Material*   pMaterial;
    std::wstring                    name;
    std::wstring                    pixelShader;
    std::wstring                    texture[VSD3DStarter::MAX_TEXTURE];
    std::shared_ptr<IEffect>        effect;
    ComPtr<ID3D11InputLayout>       il;
};

// Helper for creating a D3D input layout.
static void CreateInputLayout(_In_ ID3D11Device* device, IEffect* effect, _Out_ ID3D11InputLayout** pInputLayout, bool skinning )
{
    void const* shaderByteCode;
    size_t byteCodeLength;

    effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    if ( skinning )
    {
        ThrowIfFailed(
            device->CreateInputLayout( VertexPositionNormalTangentColorTextureSkinning::InputElements,
                                       VertexPositionNormalTangentColorTextureSkinning::InputElementCount,
                                       shaderByteCode, byteCodeLength,
                                       pInputLayout)
        );
    }
    else
    {
        ThrowIfFailed(
            device->CreateInputLayout( VertexPositionNormalTangentColorTexture::InputElements,
                                       VertexPositionNormalTangentColorTexture::InputElementCount,
                                       shaderByteCode, byteCodeLength,
                                       pInputLayout)
        );
    }

    _Analysis_assume_(*pInputLayout != 0);

    SetDebugObjectName(*pInputLayout, "ModelCMO");
}

// Shared VB input element description
static INIT_ONCE g_InitOnce = INIT_ONCE_STATIC_INIT;
static std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>> g_vbdecl;
static std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>> g_vbdeclSkinning;

static BOOL CALLBACK InitializeDecl( PINIT_ONCE initOnce, PVOID Parameter, PVOID *lpContext )
{
    UNREFERENCED_PARAMETER( initOnce );
    UNREFERENCED_PARAMETER( Parameter );
    UNREFERENCED_PARAMETER( lpContext );

    g_vbdecl = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>( VertexPositionNormalTangentColorTexture::InputElements,
           VertexPositionNormalTangentColorTexture::InputElements + VertexPositionNormalTangentColorTexture::InputElementCount );

    g_vbdeclSkinning = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>( VertexPositionNormalTangentColorTextureSkinning::InputElements,
           VertexPositionNormalTangentColorTextureSkinning::InputElements + VertexPositionNormalTangentColorTextureSkinning::InputElementCount );
    return TRUE;
}

const char* DirectX::Model::NOT_BUILD_SKELETON_EXCEPTION_MESSAGE = "No Build Skeleton Data";
const char* DirectX::Model::NOT_LOADED_CMO_EXCEPTION_ESSAGE = "CreateFromCMO";		
const char* DirectX::Model::NOT_ASSIGN_MATERIAL_TO_MESH = "Not assign material to mesh";
//======================================================================================
// Model Loader
//======================================================================================

_Use_decl_annotations_
std::unique_ptr<Model> DirectX::Model::CreateFromCMO( 
	ID3D11Device* d3dDevice, 
	const uint8_t* meshData, 
	size_t dataSize, 
	IEffectFactory& fxFactory, 
	bool ccw, 
	bool pmalpha ,
	OnFindBoneData onFindBoneData
)
{
    if ( !InitOnceExecuteOnce( &g_InitOnce, InitializeDecl, nullptr, nullptr ) )
        throw std::exception("One-time initialization failed");
    
    if ( !d3dDevice || !meshData )
        throw std::exception("Device and meshData cannot be null");

    auto fxFactoryDGSL = dynamic_cast<DGSLEffectFactory*>( &fxFactory );

    // Meshes
    auto nMesh = reinterpret_cast<const UINT*>( meshData );
    size_t usedSize = sizeof(UINT);
    if ( dataSize < usedSize )
        throw std::exception("End of file");

    if ( !*nMesh )
        throw std::exception("No meshes found");

    std::unique_ptr<Model> model(new Model());
	std::vector<int> localBoneIDtoGlobalBoneIDTbl; //メッシュにウェイトが設定されているボーンだけのボーン配列のIDから、すべてのボーン配列のIDに変換するテーブル。
	localBoneIDtoGlobalBoneIDTbl.reserve(512);
    for( UINT meshIndex = 0; meshIndex < *nMesh; ++meshIndex )
    {
		localBoneIDtoGlobalBoneIDTbl.clear();
        // Mesh name
        auto nName = reinterpret_cast<const UINT*>( meshData + usedSize );
        usedSize += sizeof(UINT);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        auto meshName = reinterpret_cast<const wchar_t*>( meshData + usedSize );

        usedSize += sizeof(wchar_t)*(*nName);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        auto mesh = std::make_shared<ModelMesh>();
        mesh->name.assign( meshName, *nName );
        mesh->ccw = ccw;
        mesh->pmalpha = pmalpha;

        // Materials
        auto nMats = reinterpret_cast<const UINT*>( meshData + usedSize );
        usedSize += sizeof(UINT);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        std::vector<MaterialRecordCMO> materials;
        materials.reserve( *nMats );
        for( UINT j = 0; j < *nMats; ++j )
        {
            MaterialRecordCMO m;

            // Material name
            nName = reinterpret_cast<const UINT*>( meshData + usedSize );
            usedSize += sizeof(UINT);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            auto matName = reinterpret_cast<const wchar_t*>( meshData + usedSize );

            usedSize += sizeof(wchar_t)*(*nName);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            m.name.assign( matName, *nName );

            // Material settings
            auto matSetting = reinterpret_cast<const VSD3DStarter::Material*>( meshData + usedSize );
            usedSize += sizeof(VSD3DStarter::Material);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            m.pMaterial = matSetting;

            // Pixel shader name
            nName = reinterpret_cast<const UINT*>( meshData + usedSize );
            usedSize += sizeof(UINT);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            auto psName = reinterpret_cast<const wchar_t*>( meshData + usedSize );

            usedSize += sizeof(wchar_t)*(*nName);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            m.pixelShader.assign( psName, *nName );

            for( UINT t = 0; t < VSD3DStarter::MAX_TEXTURE; ++t )
            {
                nName = reinterpret_cast<const UINT*>( meshData + usedSize );
                usedSize += sizeof(UINT);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                auto txtName = reinterpret_cast<const wchar_t*>( meshData + usedSize );

                usedSize += sizeof(wchar_t)*(*nName);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                m.texture[t].assign( txtName, *nName );
            }

            materials.emplace_back( m );
        }

        assert( materials.size() == *nMats );

        // Skeletal data?
        auto bSkeleton = reinterpret_cast<const BYTE*>( meshData + usedSize );
        usedSize += sizeof(BYTE);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        // Submeshes
        auto nSubmesh = reinterpret_cast<const UINT*>( meshData + usedSize );
        usedSize += sizeof(UINT);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        if ( !*nSubmesh )
            throw std::exception("No submeshes found\n");

        auto subMesh = reinterpret_cast<const VSD3DStarter::SubMesh*>( meshData + usedSize );
        usedSize += sizeof(VSD3DStarter::SubMesh) * (*nSubmesh);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        // Index buffers
        auto nIBs = reinterpret_cast<const UINT*>( meshData + usedSize );
        usedSize += sizeof(UINT);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        if ( !*nIBs )
            throw std::exception("No index buffers found\n");

        std::vector<IBData> ibData;
        ibData.reserve( *nIBs );

        std::vector<ComPtr<ID3D11Buffer>> ibs;
        ibs.resize( *nIBs );

        for( UINT j = 0; j < *nIBs; ++j )
        {
            auto nIndexes = reinterpret_cast<const UINT*>( meshData + usedSize );
            usedSize += sizeof(UINT);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            if ( !*nIndexes )
                throw std::exception("Empty index buffer found\n");

            size_t ibBytes = sizeof(USHORT) * (*(nIndexes));

            auto indexes = reinterpret_cast<const USHORT*>( meshData + usedSize );
            usedSize += ibBytes;
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            IBData ib;
            ib.nIndices = *nIndexes;
            ib.ptr = indexes;
            ibData.emplace_back( ib );

            D3D11_BUFFER_DESC desc = {};
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = static_cast<UINT>( ibBytes );
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = indexes;

            ThrowIfFailed(
                d3dDevice->CreateBuffer( &desc, &initData, &ibs[j] )
                );

            SetDebugObjectName( ibs[j].Get(), "ModelCMO" ); 
        }

        assert( ibData.size() == *nIBs );
        assert( ibs.size() == *nIBs );

        // Vertex buffers
        auto nVBs = reinterpret_cast<const UINT*>( meshData + usedSize );
        usedSize += sizeof(UINT);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        if ( !*nVBs )
            throw std::exception("No vertex buffers found\n");

        
        std::vector<VBData> vbData;
        vbData.reserve( *nVBs );
        for( UINT j = 0; j < *nVBs; ++j )
        {
            auto nVerts = reinterpret_cast<const UINT*>( meshData + usedSize );
            usedSize += sizeof(UINT);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            if ( !*nVerts )
                throw std::exception("Empty vertex buffer found\n");

            size_t vbBytes = sizeof(VertexPositionNormalTangentColorTexture) * (*(nVerts));

            auto verts = reinterpret_cast<const VertexPositionNormalTangentColorTexture*>( meshData + usedSize );
            usedSize += vbBytes;
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            VBData vb;
            vb.nVerts = *nVerts;
            vb.ptr = verts;
            vb.skinPtr = nullptr;
            vbData.emplace_back( vb );
        }
		// 接ベクトルを計算する。
		ComputeTangent(vbData, ibData);
        assert( vbData.size() == *nVBs );

        // Skinning vertex buffers
        auto nSkinVBs = reinterpret_cast<const UINT*>( meshData + usedSize );
        usedSize += sizeof(UINT);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        if ( *nSkinVBs )
        {
            if ( *nSkinVBs != *nVBs )
                throw std::exception("Number of VBs not equal to number of skin VBs");

            for( UINT j = 0; j < *nSkinVBs; ++j )
            {
                auto nVerts = reinterpret_cast<const UINT*>( meshData + usedSize );
                usedSize += sizeof(UINT);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                if ( !*nVerts )
                    throw std::exception("Empty skinning vertex buffer found\n");

                if ( vbData[ j ].nVerts != *nVerts )
                    throw std::exception("Mismatched number of verts for skin VBs");
    
                size_t vbBytes = sizeof(VSD3DStarter::SkinningVertex) * (*(nVerts));

                auto verts = reinterpret_cast<const VSD3DStarter::SkinningVertex*>( meshData + usedSize );
                usedSize += vbBytes;
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                vbData[j].skinPtr = verts;
            }
        }

        // Extents
        auto extents = reinterpret_cast<const VSD3DStarter::MeshExtents*>( meshData + usedSize );
        usedSize += sizeof(VSD3DStarter::MeshExtents);
        if ( dataSize < usedSize )
            throw std::exception("End of file");

        mesh->boundingSphere.Center.x = extents->CenterX;
        mesh->boundingSphere.Center.y = extents->CenterY;
        mesh->boundingSphere.Center.z = extents->CenterZ;
        mesh->boundingSphere.Radius = extents->Radius;

        XMVECTOR min = XMVectorSet( extents->MinX, extents->MinY, extents->MinZ, 0.f );
        XMVECTOR max = XMVectorSet( extents->MaxX, extents->MaxY, extents->MaxZ, 0.f );
        BoundingBox::CreateFromPoints( mesh->boundingBox, min, max );

#if 1
        // Animation data
        if ( *bSkeleton )
        {
            // Bones
            auto nBones = reinterpret_cast<const UINT*>( meshData + usedSize );
            usedSize += sizeof(UINT);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            if ( !*nBones )
                throw std::exception("Animation bone data is missing\n");

            for( UINT j = 0; j < *nBones; ++j )
            {
                // Bone name
                nName = reinterpret_cast<const UINT*>( meshData + usedSize );
                usedSize += sizeof(UINT);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                auto boneName = reinterpret_cast<const wchar_t*>( meshData + usedSize );

                usedSize += sizeof(wchar_t)*(*nName);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");
                
                // TODO - What to do with bone name?
                boneName;

                // Bone settings
                auto bones = reinterpret_cast<const VSD3DStarter::Bone*>( meshData + usedSize );
                usedSize += sizeof(VSD3DStarter::Bone);
                if ( dataSize < usedSize )  
                    throw std::exception("End of file");

                // TODO - What to do with bone data?
                bones;

				if (onFindBoneData != nullptr) {
					//ボーンを見つけた時のコールバック関数が指定されている。
					onFindBoneData(boneName, bones, localBoneIDtoGlobalBoneIDTbl);
				}
            }

            // Animation Clips
            auto nClips = reinterpret_cast<const UINT*>( meshData + usedSize );
            usedSize += sizeof(UINT);
            if ( dataSize < usedSize )
                throw std::exception("End of file");

            for( UINT j = 0; j < *nClips; ++j )
            {
                // Clip name
                nName = reinterpret_cast<const UINT*>( meshData + usedSize );
                usedSize += sizeof(UINT);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                auto clipName = reinterpret_cast<const wchar_t*>( meshData + usedSize );

                usedSize += sizeof(wchar_t)*(*nName);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");
                
                // TODO - What to do with clip name?
                clipName;

                auto clip = reinterpret_cast<const VSD3DStarter::Clip*>( meshData + usedSize );
                usedSize += sizeof(VSD3DStarter::Clip);
                if ( dataSize < usedSize )
                    throw std::exception("End of file");

                if ( !clip->keys )
                    throw std::exception("Keyframes missing in clip");

                auto keys = reinterpret_cast<const VSD3DStarter::Keyframe*>( meshData + usedSize );
                usedSize += sizeof(VSD3DStarter::Keyframe) * clip->keys;
                if ( dataSize < usedSize )  
                    throw std::exception("End of file");

                // TODO - What to do with keys and clip->StartTime, clip->EndTime?
                keys;
            }

        }
#else
        UNREFERENCED_PARAMETER(bSkeleton);
#endif

        bool enableSkinning = ( *nSkinVBs ) != 0;

        // Build vertex buffers
        std::vector<ComPtr<ID3D11Buffer>> vbs;
        vbs.resize( *nVBs );

        const size_t stride = enableSkinning ? sizeof(VertexPositionNormalTangentColorTextureSkinning)
                                             : sizeof(VertexPositionNormalTangentColorTexture);

        for( UINT j = 0; j < *nVBs; ++j )
        {
            size_t nVerts = vbData[ j ].nVerts;

            size_t bytes = stride * nVerts;

            D3D11_BUFFER_DESC desc = {};
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = static_cast<UINT>( bytes );
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            if ( fxFactoryDGSL && !enableSkinning )
            {
                // Can use CMO vertex data directly
                D3D11_SUBRESOURCE_DATA initData = {};
                initData.pSysMem = vbData[j].ptr;

                ThrowIfFailed(
                    d3dDevice->CreateBuffer( &desc, &initData, &vbs[j] )
                    );
            }
            else
            {
                std::unique_ptr<uint8_t[]> temp( new uint8_t[ bytes + ( sizeof(UINT) * nVerts ) ] );

                auto visited = reinterpret_cast<UINT*>( temp.get() + bytes );
                memset( visited, 0xff, sizeof(UINT) * nVerts );

                assert( vbData[j].ptr != 0 );

                if ( enableSkinning )
                {
                    // Combine CMO multi-stream data into a single stream
                    auto skinptr = vbData[j].skinPtr;
                    assert( skinptr != 0 );

                    uint8_t* ptr = temp.get();

                    auto sptr = vbData[j].ptr;

                    for( size_t v = 0; v < nVerts; ++v )
                    {
                        *reinterpret_cast<VertexPositionNormalTangentColorTexture*>( ptr ) = *sptr;
                        ++sptr;

                        auto skinv = reinterpret_cast<VertexPositionNormalTangentColorTextureSkinning*>( ptr );
						XMUINT4 index = *reinterpret_cast<const XMUINT4*>(skinptr[v].boneIndex);
						if (localBoneIDtoGlobalBoneIDTbl.size() <= index.x
							|| localBoneIDtoGlobalBoneIDTbl.size() <= index.y
							|| localBoneIDtoGlobalBoneIDTbl.size() <= index.z
							|| localBoneIDtoGlobalBoneIDTbl.size() <= index.w 
						) 
						{

							throw std::exception(NOT_BUILD_SKELETON_EXCEPTION_MESSAGE);
						}
						index.x = localBoneIDtoGlobalBoneIDTbl[index.x];
						index.y = localBoneIDtoGlobalBoneIDTbl[index.y];
						index.z = localBoneIDtoGlobalBoneIDTbl[index.z];
						index.w = localBoneIDtoGlobalBoneIDTbl[index.w];
						
                        skinv->SetBlendIndices( index );
                        skinv->SetBlendWeights( *reinterpret_cast<const XMFLOAT4*>( skinptr[v].boneWeight ) );

                        ptr += stride;
                    }
                }
                else
                {
                    memcpy( temp.get(), vbData[j].ptr, bytes );
                }

                if ( !fxFactoryDGSL )
                {
                    // Need to fix up VB tex coords for UV transform which is not supported by basic effects
                    for( UINT k = 0; k < *nSubmesh; ++k )
                    {
                        auto& sm = subMesh[ k ];

                        if ( sm.VertexBufferIndex != j )
                            continue;
						if ((sm.MaterialIndex >= *nMats)) {
							//マテリアルがポリゴンに割り当てられていない。
							throw std::exception(NOT_ASSIGN_MATERIAL_TO_MESH);
						}
						if (sm.IndexBufferIndex >= *nIBs) {
							throw std::exception("Invalid submesh found\n");
						}
                        XMMATRIX uvTransform = XMLoadFloat4x4( &materials[ sm.MaterialIndex ].pMaterial->UVTransform );

                        auto ib = ibData[ sm.IndexBufferIndex ].ptr;

                        size_t count = ibData[ sm.IndexBufferIndex ].nIndices;

                        for( size_t q = 0; q < count; ++q )
                        {
                            size_t v = ib[ q ];

                            if ( v >= nVerts )
                                throw std::exception("Invalid index found\n");

                            auto verts = reinterpret_cast<VertexPositionNormalTangentColorTexture*>( temp.get() + ( v * stride ) );
                            if ( visited[v] == UINT(-1) )
                            {
                                visited[v] = sm.MaterialIndex;

                                XMVECTOR t = XMLoadFloat2( &verts->textureCoordinate );

                                t = XMVectorSelect( g_XMIdentityR3, t, g_XMSelect1110 );

                                t = XMVector4Transform( t, uvTransform );

                                XMStoreFloat2( &verts->textureCoordinate, t );
                            }
                            else if ( visited[v] != sm.MaterialIndex )
                            {
#ifdef _DEBUG
                                XMMATRIX uv2 = XMLoadFloat4x4( &materials[ visited[v] ].pMaterial->UVTransform );

                                if ( XMVector4NotEqual( uvTransform.r[0], uv2.r[0] )
                                     || XMVector4NotEqual( uvTransform.r[1], uv2.r[1] )
                                     || XMVector4NotEqual( uvTransform.r[2], uv2.r[2] )
                                     || XMVector4NotEqual( uvTransform.r[3], uv2.r[3] ) )
                                {
                                    DebugTrace( "WARNING: %ls - mismatched UV transforms for the same vertex; texture coordinates may not be correct\n", mesh->name.c_str() );
                                }
#endif
                            }
                        }
                    }
                }

                // Create vertex buffer from temporary buffer
                D3D11_SUBRESOURCE_DATA initData = {};
                initData.pSysMem = temp.get();

                ThrowIfFailed(
                    d3dDevice->CreateBuffer( &desc, &initData, &vbs[j] )
                    );
            }

            SetDebugObjectName( vbs[j].Get(), "ModelCMO" ); 
        }

        assert( vbs.size() == *nVBs );
        
        // Create Effects
        for( UINT j = 0; j < *nMats; ++j )
        {
            auto& m = materials[ j ];

            if ( fxFactoryDGSL )
            {
                DGSLEffectFactory::DGSLEffectInfo info;
                info.name = m.name.c_str();
                info.specularPower = m.pMaterial->SpecularPower;
                info.perVertexColor = true;
                info.enableSkinning = enableSkinning;
                info.alpha = m.pMaterial->Diffuse.w;
                info.ambientColor = XMFLOAT3( m.pMaterial->Ambient.x, m.pMaterial->Ambient.y, m.pMaterial->Ambient.z );
                info.diffuseColor = XMFLOAT3( m.pMaterial->Diffuse.x, m.pMaterial->Diffuse.y, m.pMaterial->Diffuse.z );
                info.specularColor = XMFLOAT3( m.pMaterial->Specular.x, m.pMaterial->Specular.y, m.pMaterial->Specular.z );
                info.emissiveColor = XMFLOAT3( m.pMaterial->Emissive.x, m.pMaterial->Emissive.y, m.pMaterial->Emissive.z );
                info.diffuseTexture = m.texture[0].empty() ? nullptr : m.texture[0].c_str();
                info.specularTexture = m.texture[1].empty() ? nullptr : m.texture[1].c_str();
                info.normalTexture = m.texture[2].empty() ? nullptr : m.texture[2].c_str();
                info.pixelShader = m.pixelShader.c_str();
                
                const int offset = DGSLEffectFactory::DGSLEffectInfo::BaseTextureOffset;
                for( int i = 0; i < (DGSLEffect::MaxTextures - offset); ++i )
                {
                    info.textures[i] = m.texture[ i + offset ].empty() ? nullptr : m.texture[ i + offset ].c_str();
                }

                m.effect = fxFactoryDGSL->CreateDGSLEffect( info, nullptr );

                auto dgslEffect = static_cast<DGSLEffect*>( m.effect.get() );
                dgslEffect->SetUVTransform( XMLoadFloat4x4( &m.pMaterial->UVTransform ) );
            }
            else
            {
                EffectFactory::EffectInfo info;
                info.name = m.name.c_str();
                info.specularPower = m.pMaterial->SpecularPower;
                info.perVertexColor = true;
                info.enableSkinning = enableSkinning;
                info.alpha = m.pMaterial->Diffuse.w;
                info.ambientColor = XMFLOAT3( m.pMaterial->Ambient.x, m.pMaterial->Ambient.y, m.pMaterial->Ambient.z );
                info.diffuseColor = XMFLOAT3( m.pMaterial->Diffuse.x, m.pMaterial->Diffuse.y, m.pMaterial->Diffuse.z );
                info.specularColor = XMFLOAT3( m.pMaterial->Specular.x, m.pMaterial->Specular.y, m.pMaterial->Specular.z );
                info.emissiveColor = XMFLOAT3( m.pMaterial->Emissive.x, m.pMaterial->Emissive.y, m.pMaterial->Emissive.z );
                info.diffuseTexture = m.texture[0].c_str();

                m.effect = fxFactory.CreateEffect( info, nullptr );
            }

			CreateInputLayout( d3dDevice, m.effect.get(), &m.il, enableSkinning );
        }

        // Build mesh parts
        for( UINT j = 0; j < *nSubmesh; ++j )
        {
            auto& sm = subMesh[j];

            if ( (sm.IndexBufferIndex >= *nIBs)
                 || (sm.VertexBufferIndex >= *nVBs)
                 || (sm.MaterialIndex >= *nMats) )
                 throw std::exception("Invalid submesh found\n");

            auto& mat = materials[ sm.MaterialIndex ];

            auto part = new ModelMeshPart();

            if ( mat.pMaterial->Diffuse.w < 1 )
                part->isAlpha = true;

            part->indexCount = sm.PrimCount * 3;
            part->startIndex = sm.StartIndex;
            part->vertexStride = static_cast<UINT>( stride );
            part->inputLayout = mat.il;
            part->indexBuffer = ibs[ sm.IndexBufferIndex ];
            part->vertexBuffer = vbs[ sm.VertexBufferIndex ];
            part->effect = mat.effect;
            part->vbDecl = enableSkinning ? g_vbdeclSkinning : g_vbdecl;

            mesh->meshParts.emplace_back( part );
        }

        model->meshes.emplace_back( mesh );
    }

    return model;
}


//--------------------------------------------------------------------------------------
_Use_decl_annotations_
std::unique_ptr<Model> DirectX::Model::CreateFromCMO( 
	ID3D11Device* d3dDevice, 
	const wchar_t* szFileName, 
	IEffectFactory& fxFactory, 
	bool ccw, 
	bool pmalpha,
	OnFindBoneData onFindBoneData
)
{
    size_t dataSize = 0;
    std::unique_ptr<uint8_t[]> data;
    HRESULT hr = BinaryReader::ReadEntireFile( szFileName, data, &dataSize );
    if ( FAILED(hr) )
    {
        DebugTrace( "CreateFromCMO failed (%08X) loading '%ls'\n", hr, szFileName );
        throw std::exception(NOT_LOADED_CMO_EXCEPTION_ESSAGE);
    }

    auto model = CreateFromCMO( 
		d3dDevice, data.get(), dataSize, 
		fxFactory, ccw, pmalpha, 
		onFindBoneData);

    model->name = szFileName;

    return model;
}
