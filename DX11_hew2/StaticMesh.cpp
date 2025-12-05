#include	"StaticMesh.h"
#include	"AssimpPerse.h"

void StaticMesh::Load(std::string filename, std::string texturedirectory)
{
	std::vector<AssimpPerse::SUBSET> subsets{};					// サブセット情報
	std::vector<std::vector<AssimpPerse::VERTEX>> vertices{};	// 頂点データ（メッシュ単位）
	std::vector<std::vector<unsigned int>> indices{};			// インデックスデータ（メッシュ単位）
	std::vector<AssimpPerse::MATERIAL> materials{};				// マテリアル
	std::vector<std::unique_ptr<Texture>> embededtextures{};	// 内蔵テクスチャ群

	// assimpを使用してモデルデータを取得
	AssimpPerse::GetModelData(filename, texturedirectory);

	subsets = AssimpPerse::GetSubsets();		// サブセット情報取得
	vertices = AssimpPerse::GetVertices();		// 頂点データ（メッシュ単位）
	indices = AssimpPerse::GetIndices();		// インデックスデータ（メッシュ単位）
	materials = AssimpPerse::GetMaterials();	// マテリアル情報取得

	m_textures = AssimpPerse::GetTextures();	// テクスチャ情報取得	

	// 頂点データ作成
	for (const auto& mv : vertices)
	{
		for (auto& v : mv)
		{
			VERTEX_3D vertex{};
			vertex.position = DirectX::SimpleMath::Vector3(v.pos.x, v.pos.y, v.pos.z);
			vertex.normal = DirectX::SimpleMath::Vector3(v.normal.x, v.normal.y, v.normal.z);
			vertex.uv = DirectX::SimpleMath::Vector2(v.texcoord.x, v.texcoord.y);
			vertex.color = DirectX::SimpleMath::Color(v.color.r, v.color.g, v.color.b, v.color.a);

			m_vertices.emplace_back(vertex);
		}
	}

	// インデックスデータ作成
	for (const auto& mi : indices)
	{
		for (auto& index : mi)
		{
			m_indices.emplace_back(index);
		}
	}

	// サブセットデータ作成
	for (const auto& sub : subsets)
	{
		SUBSET subset{};
		subset.VertexBase = sub.VertexBase; // 頂点の開始位置
		subset.VertexNum = sub.VertexNum; // サブセット内の頂点数
		subset.IndexBase = sub.IndexBase;  // インデックスの開始位置
		subset.IndexNum = sub.IndexNum; // サブセット内のインデックス数
		subset.MtrlName = sub.mtrlname; // マテリアル名
		subset.MaterialIdx = sub.materialindex; // マテリアル配列のインデックス
		m_subsets.emplace_back(subset);
	}

	// マテリアルデータ作成
	for (const auto& m : materials)
	{
		MATERIAL material{};
		material.Ambient = DirectX::SimpleMath::Color(
			m.Ambient.r, m.Ambient.g, m.Ambient.b, m.Ambient.a);
		material.Diffuse = DirectX::SimpleMath::Color(
			m.Diffuse.r, m.Diffuse.g, m.Diffuse.b, m.Diffuse.a);

		material.Specular = DirectX::SimpleMath::Color(
			m.Specular.r, m.Specular.g, m.Specular.b, m.Specular.a);
		material.Emission = DirectX::SimpleMath::Color(
			m.Emission.r, m.Emission.g, m.Emission.b, m.Emission.a);
		material.Shiness = m.Shiness;

		if (m.texturename.empty())
		{
			material.TextureEnable = FALSE;
			m_texturenames.emplace_back("");
		}
		else
		{
			material.TextureEnable = TRUE;
			m_texturenames.emplace_back(m.texturename);
		}

		m_materials.emplace_back(material);
	}
}