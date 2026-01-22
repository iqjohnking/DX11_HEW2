#pragma once
#include "StageBase.h"

class Stage1 : public StageBase
{
public:
	Stage1() {};
	~Stage1() {};

	void Init() override; // 初期化
	void Uninit() override; // 終了処理
	void Update() override; // 更新

	//会話パート
	void BuildStartPages() override;
	void BuildEndPages() override;
};

