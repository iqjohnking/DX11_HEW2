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
	
	//color=============================
    output.col = input.col;
	
	
	
	//texture=============================
    output.tex = input.tex;
	
    return output;
}

