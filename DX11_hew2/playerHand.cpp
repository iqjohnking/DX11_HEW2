#include "playerHand.h"
#include "Field.h"
#include "Game.h"

using namespace DirectX::SimpleMath;

playerHand::playerHand(int side)
{
    if (side == 0)
    {
        SetSide(HandSide::Left);  // 左
    }
    else if (side == 1)
    {
        SetSide(HandSide::Right);  // 左
    }
    else
    {
        return;
    }
}

void playerHand::Init()
{
    // フィールド情報の取得//外でやるべきかも
    auto w_Field = Game::GetInstance()->GetObjects<Field>();
    if (w_Field.size() > 0) {
        this->m_Field = w_Field[0];
    }
    else {
        return; // error
    }

    SetFieldCenter(m_Field->GetCenter());
    SetRadius(m_Field->GetRadius() + 50.f);
    Vector3 initScale = Vector3(100.0f, 50.0f, 0.0f);
    SetScale(initScale);

    if (m_Side == HandSide::Left)
    {
        Vector3 pos;
        m_FiledAngleRad += PI;
        pos.x = m_FiledCenter.x + cosf(m_FiledAngleRad) * m_Radius;
        pos.y = m_FiledCenter.y + sinf(m_FiledAngleRad) * m_Radius;
        pos.z = 0.0f;
        SetPosition(pos);
    }
    else if (m_Side == HandSide::Right)
    {
        Vector3 pos;
        pos.x = m_FiledCenter.x + cosf(m_FiledAngleRad) * m_Radius;
        pos.y = m_FiledCenter.y + sinf(m_FiledAngleRad) * m_Radius;
        pos.z = 0.0f;
        SetPosition(pos);
    }
    else
    {
        return; //error
    }

    // 画像の初期化
    m_Texture2D.Texture2D::Init();
    m_Texture2D.SetTexture("assets/texture/hand.png");
    m_Texture2D.SetPosition(m_Position);
    m_Texture2D.SetRotation(m_Rotation);
    m_Texture2D.SetScale(100.0f, 50.0f, 0.0f);

    SetDrawOrder(2);
}

void playerHand::Update()
{
    Move();

}

void playerHand::Draw(Camera* cam)
{
    m_Texture2D.SetPosition(GetPosition()); // 親クラスの位置を反映
    m_Texture2D.SetScale(GetScale());		// 親クラスの大きさを反映
    m_Texture2D.SetRotation(GetRotation()); // 親クラスの回転を反映
    m_Texture2D.Draw(cam);
}

void playerHand::Uninit()
{
    m_Texture2D.Uninit();
}


void playerHand::Move()
{
    Vector3 pos = GetPosition();

    //==================================================
    // 1) アナログスティックの入力を取る
    //    ※ DeadZone は Input 側で処理済み
    //==================================================
    DirectX::XMFLOAT2 stick = { 0.0f, 0.0f };

    bool controllerConnected = Input::IsControllerConnected();

    if (controllerConnected)
    {
        if (m_Side == HandSide::Left)
            stick = Input::GetLeftAnalogStick();
        else if (m_Side == HandSide::Right)
            stick = Input::GetRightAnalogStick();
    }

    //==================================================
    // 2) 本当にアナログを使っているか判定
    //    （微妙な入力でキーボード操作を潰さないため）
    //==================================================
    const float ANALOG_ACTIVE_THRESHOLD = 0.25f;
    bool hasAnalogInput =
        (fabsf(stick.x) > ANALOG_ACTIVE_THRESHOLD) ||
        (fabsf(stick.y) > ANALOG_ACTIVE_THRESHOLD);

    //==================================================
    // 3) 次の角度候補を計算（まだ確定しない）
    //==================================================
    float candidateAngle = m_FiledAngleRad;
    bool hasCandidate = false;

    //---------- アナログ操作：角度を直接決める ----------
    if (hasAnalogInput)
    {
        float x = stick.x;
        float y = stick.y;
    
        // ほぼ 0 ベクトルのときは、最低限の方向を与える
        if ((x * x + y * y) < 1e-6f)
        {
            x = (m_Side == HandSide::Left) ? -1.0f : 1.0f;
            // y はそのまま（下方向が潰れないように）
        }
    
        candidateAngle = atan2f(y, x);
        hasCandidate = true;
    }
    //---------- キーボード操作：角度を少しずつ回す ----------
    else
    {
        float dAngle = 0.0f;


        if (m_Side == HandSide::Left)
        {
            if (Input::GetKeyPress(VK_W))
                dAngle -= m_AngleSpeed;
            if (Input::GetKeyPress(VK_S))
                dAngle += m_AngleSpeed;
        }
        else
        {
            if (Input::GetKeyPress(VK_UP) || Input::GetKeyPress(VK_I) )
                dAngle += m_AngleSpeed;
            if (Input::GetKeyPress(VK_DOWN) || Input::GetKeyPress(VK_K) )
                dAngle -= m_AngleSpeed;
        }
       // if (m_Side == HandSide::Left)
       // {
       //     if (Input::GetKeyPress(VK_W)|| stick.y >0.5f)
       //         dAngle -= m_AngleSpeed;
       //     if (Input::GetKeyPress(VK_S)|| stick.y <-0.5f)
       //         dAngle += m_AngleSpeed;
       // }
       // else
       // {
       //     if (Input::GetKeyPress(VK_UP) || Input::GetKeyPress(VK_I)|| stick.y > 0.5f)
       //         dAngle += m_AngleSpeed;
       //     if (Input::GetKeyPress(VK_DOWN) || Input::GetKeyPress(VK_K)|| stick.y < -0.5f)
       //         dAngle -= m_AngleSpeed;
       // }

        if (dAngle != 0.0f)
        {
            candidateAngle = m_FiledAngleRad + dAngle;

            // 角度を -PI ～ PI に収める
            if (candidateAngle > PI)        candidateAngle -= TWO_PI;
            else if (candidateAngle < -PI)  candidateAngle += TWO_PI;

            hasCandidate = true;
        }
    }

    //==================================================
    // 4) 角度候補から位置を計算
    //==================================================
    if (hasCandidate)
    {
        Vector3 candidatePos;
        candidatePos.x = m_FiledCenter.x + cosf(candidateAngle) * m_Radius;
        candidatePos.y = m_FiledCenter.y + sinf(candidateAngle) * m_Radius;
        candidatePos.z = 0.0f;

        //==================================================
        // 5) 左手・右手の動ける範囲チェック
        //==================================================
        bool allowMove = true;

        if (m_Side == HandSide::Left)
        {
            // 左手：右半分（-90°～+90°）は禁止
            if (fabsf(candidateAngle) < PI / 2.0f)
                allowMove = false;
        }
        else
        {
            // 右手：-90°～+90° だけ許可
            if (candidateAngle < -PI / 2.0f || candidateAngle > PI / 2.0f)
                allowMove = false;
        }

        //==================================================
        // 6) 問題なければ角度と位置を確定
        //==================================================
        if (allowMove)
        {
            m_FiledAngleRad = candidateAngle;
            SetPosition(candidatePos);
            pos = candidatePos;
        }
    }

    //==================================================
    // 7) もう片方の手の方向を見る（向き合わせ）
    //==================================================
    if (m_AnotherHand)
    {
        Vector3 otherPos = m_AnotherHand->GetPosition();
        Vector3 dir = otherPos - pos;

        float angleToBuddy = atan2f(dir.y, dir.x);
        Vector3 newRot(0.0f, 0.0f, angleToBuddy * (180.0f / PI));
        SetRotation(newRot);
    }
}
