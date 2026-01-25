#include "input.h"

Input* Input::m_Instance = {};



void Input::Create()
{
	if (m_Instance)return;
	m_Instance = new Input;

	m_Instance->VibrationTime = 0;
}

void Input::Update()
{
	//1フレーム前の入力を記録しておく
	for (int i = 0; i < 256; i++) { m_Instance->keyState_old[i] = m_Instance->keyState[i]; }
	m_Instance->controllerState_old = m_Instance->controllerState;

	//キー入力を更新
	BOOL hr = GetKeyboardState(m_Instance->keyState);

	//コントローラー入力を更新(XInput)
	//XInputGetState(0, &(m_Instance->controllerState));
	DWORD result = XInputGetState(0, &m_Instance->controllerState);

	if (result == ERROR_SUCCESS)
	{
		// コントローラー接続中
		m_Instance->isControllerConnected = true;
	}
	else
	{
		// 未接続：ゴミ値を使わないようにクリア
		m_Instance->isControllerConnected = false;
		ZeroMemory(&m_Instance->controllerState, sizeof(XINPUT_STATE));
	}

	//振動継続時間をカウント
	if (m_Instance->VibrationTime > 0) {
		m_Instance->VibrationTime--;
		if (m_Instance->VibrationTime == 0) { //振動継続時間が経った時に振動を止める
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;
			XInputSetState(0, &vibration);
		}
	}
}

void Input::Release()
{
	//振動を終了させる
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &vibration);

	//解放
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

//キー入力
bool Input::GetKeyPress(int key) //プレス
{
	return m_Instance->keyState[key] & 0x80;
}
bool Input::GetKeyTrigger(int key) //トリガー
{
	return (m_Instance->keyState[key] & 0x80) && !(m_Instance->keyState_old[key] & 0x80);
}
bool Input::GetKeyRelease(int key) //リリース
{
	return !(m_Instance->keyState[key] & 0x80) && (m_Instance->keyState_old[key] & 0x80);
}

//左アナログスティック
DirectX::XMFLOAT2 Input::GetLeftAnalogStick(void)
{
	SHORT x = m_Instance->controllerState.Gamepad.sThumbLX;
	SHORT y = m_Instance->controllerState.Gamepad.sThumbLY;

	DirectX::XMFLOAT2 res;
	res.x = x / 32767.0f; //-1～1
	res.y = y / 32767.0f; //-1～1
	return res;

	////SHORT x = m_Instance->controllerState.Gamepad.sThumbLX; // -32768～32767
	//SHORT y = m_Instance->controllerState.Gamepad.sThumbLY; // -32768～32767
	//
	//DirectX::XMFLOAT2 res;
	//res.x = ApplyDeadZone(x, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//res.y = ApplyDeadZone(y, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	//return res;
}
//右アナログスティック
DirectX::XMFLOAT2 Input::GetRightAnalogStick(void)
{
	SHORT x = m_Instance->controllerState.Gamepad.sThumbRX; // -32768～32767
	SHORT y = m_Instance->controllerState.Gamepad.sThumbRY; // -32768～32767

	DirectX::XMFLOAT2 res;
	res.x = x / 32767.0f; //-1～1
	res.y = y / 32767.0f; //-1～1
	return res;

	//SHORT x = m_Instance->controllerState.Gamepad.sThumbRX;
	//SHORT y = m_Instance->controllerState.Gamepad.sThumbRY;
	//
	//DirectX::XMFLOAT2 res;
	//res.x = ApplyDeadZone(x, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//res.y = ApplyDeadZone(y, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	//return res;
}

//左トリガー
float Input::GetLeftTrigger(void)
{
	BYTE t = m_Instance->controllerState.Gamepad.bLeftTrigger; // 0～255
	return t / 255.0f;
}
//右トリガー
float Input::GetRightTrigger(void)
{
	BYTE t = m_Instance->controllerState.Gamepad.bRightTrigger; // 0～255
	return t / 255.0f;
}

//ボタン入力
bool Input::GetButtonPress(WORD btn) //プレス
{
	return (m_Instance->controllerState.Gamepad.wButtons & btn) != 0;
}
bool Input::GetButtonTrigger(WORD btn) //トリガー
{
	return (m_Instance->controllerState.Gamepad.wButtons & btn) != 0 && (m_Instance->controllerState_old.Gamepad.wButtons & btn) == 0;
}
bool Input::GetButtonRelease(WORD btn) //リリース
{
	return (m_Instance->controllerState.Gamepad.wButtons & btn) == 0 && (m_Instance->controllerState_old.Gamepad.wButtons & btn) != 0;
}

//振動
void Input::SetVibration(int frame, float powor)
{
	// XINPUT_VIBRATION構造体のインスタンスを作成
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	// モーターの強度を設定（0～65535）
	vibration.wLeftMotorSpeed = (WORD)(powor * 65535.0f);
	vibration.wRightMotorSpeed = (WORD)(powor * 65535.0f);
	XInputSetState(0, &vibration);

	//振動継続時間を代入
	m_Instance->VibrationTime = frame;
}

float Input::ApplyDeadZone(short v, short deadZone)
{
	if (abs(v) < deadZone) return 0.0f;
	return (float)v / 32767.0f;
}

DirectX::XMFLOAT2 Input::GetMousePosition()
{
	POINT p;
	GetCursorPos(&p); // スクリーン上のマウス位置を取得

	// ウィンドウ内の座標に変換
	ScreenToClient(GetActiveWindow(), &p);

	// 現在のウィンドウの幅と高さを取得（解像度変更に対応）
	RECT rect;
	GetClientRect(GetActiveWindow(), &rect);
	float windowW = (float)(rect.right - rect.left);
	float windowH = (float)(rect.bottom - rect.top);

	// ゲーム内の座標系（中心 0,0 / 上がプラス）に変換
	DirectX::XMFLOAT2 pos;
	pos.x = (float)p.x - (windowW / 2.0f);
	pos.y = -((float)p.y - (windowH / 2.0f));

	return pos;
}

bool Input::GetMouseButtonTrigger(int button)
{
	int vk = (button == 0) ? VK_LBUTTON : VK_RBUTTON;

	// GetAsyncKeyStateの最上位ビットが1なら現在押されている
	// ※キー入力と同様にoldStateで比較するのが理想ですが、
	// 手軽にクリックを判定するならこれで十分動作します
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}