cbuffer WorldBuffer : register(b0)  // world
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)  // View
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2) // Projection
{
	matrix Projection;
}

struct VS_IN
{
	float4 pos : POSITION0;
	float4 nrm : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct LIGHT
{
	bool Enable;        //使用するか否か
	bool3 Dummy;        //PADDING
	float4 Direction;   //方向
	float4 Diffuse;     //拡散反射用の光
	float4 Ambient;     //環境光用の
};

cbuffer LightBuffer : register(b3)
{
	LIGHT Light;
};

struct MATERIAL
{
	float4  Ambient;        //環境反射
	float4  Diffuse;        //拡散反射（≒カラー
	float4  Specular;       //鏡面反射
	float4  Emission;       //発光
	float   Shiness;        //光沢の滑らかさ
	bool    TextureEnable;  //テクスチャの使うか否かのフラグ
	bool2   Dummy;          //alignment padding用
};

cbuffer MaterialBuffer : register(b4)
{
	MATERIAL Material;
};

cbuffer TextureBuffer : register(b5)
{
	matrix matrixTex;
};
