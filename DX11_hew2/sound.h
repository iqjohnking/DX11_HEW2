#pragma once

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	//SOUND_LABEL_BGM000 = 0,		// サンプルBGM
	//SOUND_LABEL_BGM001,		// サンプルBGM
	//SOUND_LABEL_SE000,		// サンプルSE
	//SOUND_LABEL_SE001,		// サンプルSE
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