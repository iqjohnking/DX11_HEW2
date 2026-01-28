#include "Hashira.h"
#include "Game.h"
//#include "silkWall.h"

using namespace std;
using namespace DirectX::SimpleMath;

void Hashira::Init()
{
	//初期化処理
	m_Texture2D.Texture2D::Init();

	m_Texture2D.SetTexture("assets/texture/hashira.png");
	//SetPosition(100.0f, 100.0f, 0.0f);	// 初期位置は外部で設定する想定
	m_Texture2D.SetRotation(m_Rotation);	// 初期値0,0,0//外で変える
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0); //初期値25*2  , 25*2  ,0//外で変える
	m_Collider.center = GetPosition();		// 初期値0,0,0//外で変える
	m_Collider.radius = GetRadius();		// 初期値25,25,0//外で変える

	//animation 設定
	//m_Texture2D.SetSpriteSheet(3, 3);
	//m_Texture2D.AddAnimClip("SPAWNING", 0, 0, 10);
	//
	//m_Texture2D.PlayAnim("SPAWNING");

	SetDrawOrder(2);
}

void Hashira::Update()
{

}

void Hashira::Draw(Camera* cam)
{
	m_Texture2D.SetPosition(m_Position);
	m_Texture2D.SetScale(m_Radius * 2, m_Radius * 2, 0);
	m_Texture2D.Draw(cam);
}

void Hashira::Uninit()
{
	m_Texture2D.Uninit();
}
