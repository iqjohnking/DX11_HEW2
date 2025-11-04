cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
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
    bool Enable;        //Žg—p‚·‚é‚©”Û‚©
    bool3 Dummy;        //PADDING
    float4 Direction;   //•ûŒü
    float4 Diffuse;     //ŠgŽU”½ŽË—p‚ÌŒõ
    float4 Ambient;     //ŠÂ‹«Œõ—p‚Ì
};

cbuffer LightBuffer : register(b3)
{
    LIGHT Light;
};
