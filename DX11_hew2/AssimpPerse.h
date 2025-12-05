#pragma once
#include	<assimp/Importer.hpp>
#include	<assimp/scene.h>
#include	<assimp/postprocess.h>
#include	<assimp/cimport.h>

namespace AssimpPerse
{
	struct VERTEX {
		std::string meshname;		// メッシュ名
		aiVector3D	pos;			// 位置
		aiVector3D	normal;			// 法線
		aiColor4D	color;			// 頂点カラー	
		aiVector3D	texcoord;		// テクスチャ座標	
		int			materialindex;	// マテリアルインデックス
		std::string mtrlname;		// マテリアル名
	};

	struct SUBSET {
		std::string meshname;		// メッシュ名
		int materialindex;			// マテリアルインデックス
		unsigned int VertexBase;	// 頂点バッファのベース
		unsigned int VertexNum;		// 頂点数
		unsigned int IndexBase;		// インデックスバッファのベース
		unsigned int IndexNum;		// インデックス数
		std::string	 mtrlname;		// マテリアル名
	};

	struct MATERIAL {
		std::string mtrlname;		// マテリアル名
		aiColor4D	Ambient;		// アンビエント
		aiColor4D	Diffuse;		// ディフューズ
		aiColor4D	Specular;		// スペキュラ
		aiColor4D	Emission;		// エミッション
		float		Shiness;		// シャイネス
		std::string texturename;	// テクスチャ名
	};

	void GetModelData(std::string filename, std::string texturedirectory);
	std::vector<SUBSET> GetSubsets();
	std::vector<std::vector<VERTEX>> GetVertices();
	std::vector<std::vector<unsigned int>> GetIndices();
	std::vector<MATERIAL> GetMaterials();
	std::vector<std::unique_ptr<Texture>> GetTextures();
}
