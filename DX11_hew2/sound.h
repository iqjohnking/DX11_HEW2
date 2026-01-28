#pragma once

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	//SOUND_LABEL_BGM000 = 0,		// サンプルBGM
	//SOUND_LABEL_BGM001,		// サンプルBGM
	//SOUND_LABEL_SE000,		// サンプルSE
	//SOUND_LABEL_SE001,		// サンプルSE


	//Stage0_Start Voice
	//SOUND_LABEL_VOICE_STAGE0_START_000,
	SOUND_LABEL_VOICE_STAGE0_START_001,
	//SOUND_LABEL_VOICE_STAGE0_START_002,
	SOUND_LABEL_VOICE_STAGE0_START_003,
	SOUND_LABEL_VOICE_STAGE0_START_004,
	//SOUND_LABEL_VOICE_STAGE0_START_005,
	//SOUND_LABEL_VOICE_STAGE0_START_006,
	SOUND_LABEL_VOICE_STAGE0_START_007,
	SOUND_LABEL_VOICE_STAGE0_START_008,
	SOUND_LABEL_VOICE_STAGE0_START_009,
	//SOUND_LABEL_VOICE_STAGE0_START_010,
	SOUND_LABEL_VOICE_STAGE0_START_011,
	SOUND_LABEL_VOICE_STAGE0_START_012,
	SOUND_LABEL_VOICE_STAGE0_START_013,
	SOUND_LABEL_VOICE_STAGE0_START_014,
	SOUND_LABEL_VOICE_STAGE0_START_015,
	SOUND_LABEL_VOICE_STAGE0_START_016,
	SOUND_LABEL_VOICE_STAGE0_START_017,
	SOUND_LABEL_VOICE_STAGE0_START_018,
	SOUND_LABEL_VOICE_STAGE0_START_019,
	//Stage1_Start Voice
	SOUND_LABEL_VOICE_STAGE1_START_000,
	SOUND_LABEL_VOICE_STAGE1_START_001,
	SOUND_LABEL_VOICE_STAGE1_START_002,
	SOUND_LABEL_VOICE_STAGE1_START_003,
	SOUND_LABEL_VOICE_STAGE1_START_004,
	//SOUND_LABEL_VOICE_STAGE1_START_005,
	SOUND_LABEL_VOICE_STAGE1_START_006,
	SOUND_LABEL_VOICE_STAGE1_START_007,
	SOUND_LABEL_VOICE_STAGE1_START_008,
	SOUND_LABEL_VOICE_STAGE1_START_009,
	//SOUND_LABEL_VOICE_STAGE1_START_010,
	SOUND_LABEL_VOICE_STAGE1_START_011,
	SOUND_LABEL_VOICE_STAGE1_START_012,
	SOUND_LABEL_VOICE_STAGE1_START_013,
	SOUND_LABEL_VOICE_STAGE1_START_014,
	SOUND_LABEL_VOICE_STAGE1_START_015,
	SOUND_LABEL_VOICE_STAGE1_START_016,
	SOUND_LABEL_VOICE_STAGE1_START_017,
	//SOUND_LABEL_VOICE_STAGE1_START_018,
	SOUND_LABEL_VOICE_STAGE1_START_019,
	SOUND_LABEL_VOICE_STAGE1_START_020,
	SOUND_LABEL_VOICE_STAGE1_START_021,
	SOUND_LABEL_VOICE_STAGE1_START_022,
	//Stage1_End Voice
	SOUND_LABEL_VOICE_STAGE1_END_000,
	SOUND_LABEL_VOICE_STAGE1_END_001,
	SOUND_LABEL_VOICE_STAGE1_END_002,
	SOUND_LABEL_VOICE_STAGE1_END_003,
	SOUND_LABEL_VOICE_STAGE1_END_004,
	SOUND_LABEL_VOICE_STAGE1_END_005,
	//Stage2_Start Voice
	SOUND_LABEL_VOICE_STAGE2_START_000,
	SOUND_LABEL_VOICE_STAGE2_START_001,
	//Stage2_End Voice
	SOUND_LABEL_VOICE_STAGE2_END_000,
	SOUND_LABEL_VOICE_STAGE2_END_001,
	SOUND_LABEL_VOICE_STAGE2_END_002,
	//Stage3_Start Voice
	SOUND_LABEL_VOICE_STAGE3_START_000,
	SOUND_LABEL_VOICE_STAGE3_START_001,
	SOUND_LABEL_VOICE_STAGE3_START_002,
	//Stage3_End Voice
	SOUND_LABEL_VOICE_STAGE3_END_000,
	SOUND_LABEL_VOICE_STAGE3_END_001,
	SOUND_LABEL_VOICE_STAGE3_END_002,
	//Stage4_Start Voice
	SOUND_LABEL_VOICE_STAGE4_START_000,
	SOUND_LABEL_VOICE_STAGE4_START_001,
	//SOUND_LABEL_VOICE_STAGE4_START_002,
	SOUND_LABEL_VOICE_STAGE4_START_003,
	SOUND_LABEL_VOICE_STAGE4_START_004,
	SOUND_LABEL_VOICE_STAGE4_START_005,
	SOUND_LABEL_VOICE_STAGE4_START_006,
	SOUND_LABEL_VOICE_STAGE4_START_007,
	SOUND_LABEL_VOICE_STAGE4_START_008,
	SOUND_LABEL_VOICE_STAGE4_START_009,
	SOUND_LABEL_VOICE_STAGE4_START_011,
	SOUND_LABEL_VOICE_STAGE4_START_012,
	SOUND_LABEL_VOICE_STAGE4_START_013,
	//Stage4_End Voice
	SOUND_LABEL_VOICE_STAGE4_END_000,
	SOUND_LABEL_VOICE_STAGE4_END_001,
	SOUND_LABEL_VOICE_STAGE4_END_002,
	//Stage5_Start Voice
	SOUND_LABEL_VOICE_STAGE5_START_000,
	SOUND_LABEL_VOICE_STAGE5_START_001,
	SOUND_LABEL_VOICE_STAGE5_START_002,
	//Stage5_End Voice
	SOUND_LABEL_VOICE_STAGE5_END_000,
	SOUND_LABEL_VOICE_STAGE5_END_001,
	SOUND_LABEL_VOICE_STAGE5_END_002,
	SOUND_LABEL_VOICE_STAGE5_END_003,
	SOUND_LABEL_VOICE_STAGE5_END_004,
	//Stage6_Start Voice
	SOUND_LABEL_VOICE_STAGE6_START_000,
	//SOUND_LABEL_VOICE_STAGE6_START_001,
	SOUND_LABEL_VOICE_STAGE6_START_002,
	SOUND_LABEL_VOICE_STAGE6_START_003,
	//SOUND_LABEL_VOICE_STAGE6_START_004,
	SOUND_LABEL_VOICE_STAGE6_START_005,
	SOUND_LABEL_VOICE_STAGE6_START_006,
	//Stage6_End Voice
	SOUND_LABEL_VOICE_STAGE6_END_000,
	SOUND_LABEL_VOICE_STAGE6_END_001,
	SOUND_LABEL_VOICE_STAGE6_END_002,
	SOUND_LABEL_VOICE_STAGE6_END_003,
	SOUND_LABEL_VOICE_STAGE6_END_004,
	SOUND_LABEL_VOICE_STAGE6_END_005,
	//SOUND_LABEL_VOICE_STAGE6_END_006,
	SOUND_LABEL_VOICE_STAGE6_END_007,
	SOUND_LABEL_VOICE_STAGE6_END_008,
	SOUND_LABEL_VOICE_STAGE6_END_009,
	SOUND_LABEL_VOICE_STAGE6_END_010,
	SOUND_LABEL_VOICE_STAGE6_END_011,
	SOUND_LABEL_VOICE_STAGE6_END_012,
	SOUND_LABEL_VOICE_STAGE6_END_013,
	//SOUND_LABEL_VOICE_STAGE6_END_014,
	SOUND_LABEL_VOICE_STAGE6_END_015,
	SOUND_LABEL_VOICE_STAGE6_END_016,
	SOUND_LABEL_VOICE_STAGE6_END_017,
	//Stage7_Start Voice
	//SOUND_LABEL_VOICE_STAGE7_START_000,
	SOUND_LABEL_VOICE_STAGE7_START_001,
	SOUND_LABEL_VOICE_STAGE7_START_002,
	SOUND_LABEL_VOICE_STAGE7_START_003,
	SOUND_LABEL_VOICE_STAGE7_START_004,
	SOUND_LABEL_VOICE_STAGE7_START_005,
	//SOUND_LABEL_VOICE_STAGE7_START_006,
	SOUND_LABEL_VOICE_STAGE7_START_007,
	SOUND_LABEL_VOICE_STAGE7_START_008,
	SOUND_LABEL_VOICE_STAGE7_START_009,
	SOUND_LABEL_VOICE_STAGE7_START_010,
	SOUND_LABEL_VOICE_STAGE7_START_011,
	SOUND_LABEL_VOICE_STAGE7_START_012,
	//SOUND_LABEL_VOICE_STAGE7_START_013,
	SOUND_LABEL_VOICE_STAGE7_START_014,
	SOUND_LABEL_VOICE_STAGE7_START_015,
	SOUND_LABEL_VOICE_STAGE7_START_016,
	SOUND_LABEL_VOICE_STAGE7_START_017,
	SOUND_LABEL_VOICE_STAGE7_START_018,
	SOUND_LABEL_VOICE_STAGE7_START_019,
	SOUND_LABEL_VOICE_STAGE7_START_020,
	SOUND_LABEL_VOICE_STAGE7_START_021,
	SOUND_LABEL_VOICE_STAGE7_START_022,
	SOUND_LABEL_VOICE_STAGE7_START_023,
	SOUND_LABEL_VOICE_STAGE7_START_024,
	SOUND_LABEL_VOICE_STAGE7_START_025,
	SOUND_LABEL_VOICE_STAGE7_START_026,
	SOUND_LABEL_VOICE_STAGE7_START_027,
	SOUND_LABEL_VOICE_STAGE7_START_028,
	SOUND_LABEL_VOICE_STAGE7_START_029,
	SOUND_LABEL_VOICE_STAGE7_START_030,
	//Stage7_End Voice
	SOUND_LABEL_VOICE_STAGE7_END_000,
	SOUND_LABEL_VOICE_STAGE7_END_001,
	SOUND_LABEL_VOICE_STAGE7_END_002,
	//SOUND_LABEL_VOICE_STAGE7_END_003,
	//Stage8_Start Voice
	SOUND_LABEL_VOICE_STAGE8_START_000,
	SOUND_LABEL_VOICE_STAGE8_START_001,
	SOUND_LABEL_VOICE_STAGE8_START_002,
	SOUND_LABEL_VOICE_STAGE8_START_003,
	//Stage8_End Voice
	//SOUND_LABEL_VOICE_STAGE8_END_000,
	SOUND_LABEL_VOICE_STAGE8_END_001,
	SOUND_LABEL_VOICE_STAGE8_END_002,
	SOUND_LABEL_VOICE_STAGE8_END_003,
	SOUND_LABEL_VOICE_STAGE8_END_004,
	SOUND_LABEL_VOICE_STAGE8_END_005,
	SOUND_LABEL_VOICE_STAGE8_END_006,
	SOUND_LABEL_VOICE_STAGE8_END_007,
	SOUND_LABEL_VOICE_STAGE8_END_008,
	SOUND_LABEL_VOICE_STAGE8_END_009,
	SOUND_LABEL_VOICE_STAGE8_END_010,
	//Stage9_Start Voice
	SOUND_LABEL_VOICE_STAGE9_START_000,
	SOUND_LABEL_VOICE_STAGE9_START_001,
	//SOUND_LABEL_VOICE_STAGE9_START_002,
	//SOUND_LABEL_VOICE_STAGE9_START_003,
	SOUND_LABEL_VOICE_STAGE9_START_004,
	//SOUND_LABEL_VOICE_STAGE9_START_005,
	SOUND_LABEL_VOICE_STAGE9_START_006,
	//Stage9_End Voice
	SOUND_LABEL_VOICE_STAGE9_END_000,
	SOUND_LABEL_VOICE_STAGE9_END_001,
	SOUND_LABEL_VOICE_STAGE9_END_002,
	SOUND_LABEL_VOICE_STAGE9_END_003,
	SOUND_LABEL_VOICE_STAGE9_END_004,
	//SOUND_LABEL_VOICE_STAGE9_END_005,
	//SOUND_LABEL_VOICE_STAGE9_END_006,
	SOUND_LABEL_VOICE_STAGE9_END_007,
	SOUND_LABEL_VOICE_STAGE9_END_008,
	SOUND_LABEL_VOICE_STAGE9_END_009,
	SOUND_LABEL_VOICE_STAGE9_END_010,
	SOUND_LABEL_VOICE_STAGE9_END_011,
	SOUND_LABEL_VOICE_STAGE9_END_012,
	SOUND_LABEL_VOICE_STAGE9_END_013,
	SOUND_LABEL_VOICE_STAGE9_END_014,
	SOUND_LABEL_VOICE_STAGE9_END_015,
	//SOUND_LABEL_VOICE_STAGE9_END_016,
	SOUND_LABEL_VOICE_STAGE9_END_017,
	SOUND_LABEL_VOICE_STAGE9_END_018,
	SOUND_LABEL_VOICE_STAGE9_END_019,
	//SOUND_LABEL_VOICE_STAGE9_END_020,
	//SOUND_LABEL_VOICE_STAGE9_END_021,
	SOUND_LABEL_VOICE_STAGE9_END_022,
	SOUND_LABEL_VOICE_STAGE9_END_023,
	//SOUND_LABEL_VOICE_STAGE9_END_024,
	//SOUND_LABEL_VOICE_STAGE9_END_025,
	SOUND_LABEL_VOICE_STAGE9_END_026,
	SOUND_LABEL_VOICE_STAGE9_END_027,
	//SOUND_LABEL_VOICE_STAGE9_END_028,
	SOUND_LABEL_VOICE_STAGE9_END_029,
	SOUND_LABEL_VOICE_STAGE9_END_030,
	//SOUND_LABEL_VOICE_STAGE9_END_031,
	SOUND_LABEL_VOICE_STAGE9_END_032,

	//BGM
	//会話パート
	SOUND_LABEL_BGM_CONVERSATION_000,
	SOUND_LABEL_BGM_CONVERSATION_001,
	SOUND_LABEL_BGM_CONVERSATION_002,
	SOUND_LABEL_BGM_CONVERSATION_003,
	SOUND_LABEL_BGM_CONVERSATION_004,
	SOUND_LABEL_BGM_CONVERSATION_005,
	SOUND_LABEL_BGM_CONVERSATION_006,
	SOUND_LABEL_BGM_CONVERSATION_007,
	SOUND_LABEL_BGM_CONVERSATION_008,
	SOUND_LABEL_BGM_CONVERSATION_009,
	SOUND_LABEL_BGM_CONVERSATION_010,

	//Stage
	SOUND_LABEL_BGM_STAGE_000,
	SOUND_LABEL_BGM_STAGE_001,
	SOUND_LABEL_BGM_STAGE_002,
	SOUND_LABEL_BGM_STAGE_003,

	//SE
	SOUND_LABEL_SE_000,
	SOUND_LABEL_SE_001,
	SOUND_LABEL_SE_002,
	SOUND_LABEL_SE_003,
	SOUND_LABEL_SE_004,
	SOUND_LABEL_SE_005,
	SOUND_LABEL_SE_006,
	SOUND_LABEL_SE_007,
	SOUND_LABEL_SE_008,
	SOUND_LABEL_SE_009,



	SOUND_LABEL_MAX,
} SOUND_LABEL;

class Sound {
private:
	// パラメータ構造体
	typedef struct
	{
		LPCSTR filename;	// 音声ファイルまでのパスを設定
		bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
		float volume; //音量
	} PARAM;

	PARAM m_param[SOUND_LABEL_MAX] =
	{
		//{"asset/BGM/sample000.wav", true},	// サンプルBGM（ループさせるのでtrue設定）
//		{"asset/BGM/○○○.wav", true},	// サンプルBGM
//		{"asset/SE/○○○.wav", false},  		// サンプルSE（ループしないのでfalse設定）
//		{"asset/SE/○○○.wav", false},		// サンプルSE
// 		//Stage0
		{"assets/sound/voice/Stage0/Stage0_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_003.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_004.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_007.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_008.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_009.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_011.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_012.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_013.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_014.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_015.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_016.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_017.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_018.wav", false,0.5f},
		{"assets/sound/voice/Stage0/Stage0_Start_019.wav", false,0.5f},
		//Stage1
		{"assets/sound/voice/Stage1/Stage1_Start_000.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_002.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_003.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_004.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_006.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_007.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_008.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_009.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_011.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_012.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_013.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_014.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_015.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_016.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_017.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_019.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_020.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_021.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_Start_022.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_End_001.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_End_002.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_End_003.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_End_004.wav", false,0.5f},
		{"assets/sound/voice/Stage1/Stage1_End_005.wav", false,0.5f},
		//Stage2
		{ "assets/sound/voice/Stage2/Stage2_Start_000.wav", false,0.5f },
		{"assets/sound/voice/Stage2/Stage2_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage2/Stage2_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage2/Stage2_End_001.wav", false,0.5f},
		{"assets/sound/voice/Stage2/Stage2_End_002.wav", false,0.5f},
		//Stage3
		{ "assets/sound/voice/Stage3/Stage3_Start_000.wav", false,0.5f },
		{"assets/sound/voice/Stage3/Stage3_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage3/Stage3_Start_002.wav", false,0.5f},
		{"assets/sound/voice/Stage3/Stage3_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage3/Stage3_End_001.wav", false,0.5f},
		{"assets/sound/voice/Stage3/Stage3_End_002.wav", false,0.5f},
		//Stage4
		{ "assets/sound/voice/Stage4/Stage4_Start_000.wav", false,0.5f },
		{"assets/sound/voice/Stage4/Stage4_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_003.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_004.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_005.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_006.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_007.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_008.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_009.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_011.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_012.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_Start_013.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_End_001.wav", false,0.5f},
		{"assets/sound/voice/Stage4/Stage4_End_002.wav", false,0.5f},
		//Stage5
		{ "assets/sound/voice/Stage5/Stage5_Start_000.wav", false,0.5f },
		{"assets/sound/voice/Stage5/Stage5_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage5/Stage5_Start_002.wav", false,0.5f},
		{"assets/sound/voice/Stage5/Stage5_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage5/Stage5_End_001.wav", false,0.5f},
		{"assets/sound/voice/Stage5/Stage5_End_002.wav", false,0.5f},
		{"assets/sound/voice/Stage5/Stage5_End_003.wav", false,0.5f},
		{"assets/sound/voice/Stage5/Stage5_End_004.wav", false,0.5f},
		//Stage6
		{ "assets/sound/voice/Stage6/Stage6_Start_000.wav", false,0.5f },
		{"assets/sound/voice/Stage6/Stage6_Start_002.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_Start_003.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_Start_005.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_Start_006.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_001.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_002.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_003.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_004.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_005.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_007.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_008.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_009.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_010.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_011.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_012.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_013.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_015.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_016.wav", false,0.5f},
		{"assets/sound/voice/Stage6/Stage6_End_017.wav", false,0.5f},
		//Stage7
		{"assets/sound/voice/Stage7/Stage7_Start_001.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_002.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_003.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_004.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_005.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_007.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_008.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_009.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_010.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_011.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_012.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_014.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_015.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_016.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_017.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_018.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_019.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_020.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_021.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_022.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_023.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_024.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_025.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_026.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_027.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_028.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_029.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_Start_030.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_End_000.wav", false,0.5f},
		{"assets/sound/voice/Stage7/Stage7_End_001.wav", false,0.5f},
		{ "assets/sound/voice/Stage7/Stage7_End_002.wav", false,0.5f },
		//Stage8
		{ "assets/sound/voice/Stage8/Stage8_Start_000.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_Start_001.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_Start_002.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_Start_003.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_001.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_002.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_003.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_004.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_005.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_006.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_007.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_008.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_009.wav", false,0.5f },
		{ "assets/sound/voice/Stage8/Stage8_End_010.wav", false,0.5f },
		//Stage9
		{ "assets/sound/voice/Stage9/Stage9_Start_000.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_Start_001.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_Start_004.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_Start_006.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_000.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_001.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_002.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_003.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_004.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_007.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_008.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_009.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_010.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_011.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_012.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_013.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_014.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_015.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_017.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_018.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_019.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_022.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_023.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_026.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_027.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_029.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_030.wav", false,0.5f },
		{ "assets/sound/voice/Stage9/Stage9_End_032.wav", false,0.5f },

		//BGM
		//会話パート
		//BGMなのでtrue
		{ "assets/sound/conversation/BGM001.wav", true,0.1f },
		{ "assets/sound/conversation/BGM002.wav", true,0.1f },
		{ "assets/sound/conversation/BGM004.wav", true,0.1f },
		{ "assets/sound/conversation/BGM005.wav", true,0.1f },
		{ "assets/sound/conversation/BGM007.wav", true,0.1f },
		{ "assets/sound/conversation/BGM008.wav", true,0.1f },
		{ "assets/sound/conversation/BGM009.wav", true,0.1f },
		{ "assets/sound/conversation/BGM011.wav", true,0.1f },
		{ "assets/sound/conversation/BGM013.wav", true,0.1f },
		{ "assets/sound/conversation/BGM014.wav", true,0.1f },
		{ "assets/sound/conversation/BGM000.wav", true,0.1f },

		//StageBGM
		{ "assets/sound/stage/BGM003.wav", true,0.1f },
		{ "assets/sound/stage/BGM006.wav", true,0.1f },
		{ "assets/sound/stage/BGM010.wav", true,0.1f },
		{ "assets/sound/stage/BGM012.wav", true,0.1f },

		//SE

		{ "assets/sound/SE/SE000.wav", false, 1.0f },
		{ "assets/sound/SE/SE001.wav", false, 1.0f },
		{ "assets/sound/SE/SE002.wav", false, 1.0f },
		{ "assets/sound/SE/SE003.wav", false, 1.0f },
		{ "assets/sound/SE/SE004.wav", false, 1.0f },
		{ "assets/sound/SE/SE005.wav", false, 1.0f },
		{ "assets/sound/SE/SE006.wav", false, 1.0f },
		{ "assets/sound/SE/SE007.wav", false, 1.0f },
		{ "assets/sound/SE/SE008.wav", false, 1.0f },
		{ "assets/sound/SE/SE009.wav", false, 1.0f },


	};

	IXAudio2* m_pXAudio2 = NULL;
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;
	IXAudio2SourceVoice* m_pSourceVoice[SOUND_LABEL_MAX];
	WAVEFORMATEXTENSIBLE m_wfx[SOUND_LABEL_MAX]; // WAVフォーマット
	XAUDIO2_BUFFER m_buffer[SOUND_LABEL_MAX];
	BYTE* m_DataBuffer[SOUND_LABEL_MAX];

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:
	// ゲームループ開始前に呼び出すサウンドの初期化処理
	HRESULT Init(void);

	// ゲームループ終了後に呼び出すサウンドの解放処理
	void Uninit(void);

	// 引数で指定したサウンドを再生する
	void Play(SOUND_LABEL label);

	// 引数で指定したサウンドを停止する
	void Stop(SOUND_LABEL label);

	// 引数で指定したサウンドの再生を再開する
	void Resume(SOUND_LABEL label);

};