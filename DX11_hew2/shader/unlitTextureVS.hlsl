#include "common.hlsl"

PS_IN main(in VS_IN input)
{
	PS_IN output;
	
	//positoin=============================
	// ワールド、ビュー、プロジェクション行列を掛け合わせて座標変換を行う
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	output.pos = mul(input.pos, wvp);
	
	//texture=============================
	//output.tex = input.tex;
	// UV座標を移動される
	float4 uv;
	uv.xy = input.tex;
	uv.z = 0.0f;
	uv.w = 1.0f;
	uv = mul(uv, matrixTex);
	output.tex = uv.xy;

	//color=============================
	output.col = input.col;
	
	return output;
}

