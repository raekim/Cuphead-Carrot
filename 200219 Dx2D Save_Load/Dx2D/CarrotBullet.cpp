#include "stdafx.h"
#include "CarrotBullet.h"


CarrotBullet::CarrotBullet()
{
	m_hitCircle = new Circle;

	m_pAnimation = new Animation<STATE>;
	Clip* clip;

	// 날아가는 동작
	g_pTextureManager->AddTexture(L"carrot-bullet-shooting", L"carrot-bullet.png");
	clip = new Clip;
	for (int i = 0; i < 4; i++)
	{
		Sprite* sprite = new Sprite(L"carrot-bullet-shooting", 4, 1, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(STATE::SHOOTING, clip);
	m_speed = 180;


	// 부서지는 동작
	this->Init();
}


CarrotBullet::~CarrotBullet()
{
}

void CarrotBullet::Init()
{
	Destory();
	m_active = false;
	m_eState = STATE::SHOOTING;
	m_pAnimation->Play(m_eState);

	m_hitCircle->SetRadius(30);
	m_hitCircle->SetDraw(false);
	m_hitCircle->SetBorder(true);
}

void CarrotBullet::Update(D3DXVECTOR2 playerPos)
{
	if (!m_active) return;

	// 유도탄 이동, 회전
	auto dir = playerPos - m_vPosition;
	m_vRotaion.z = D3DXToRadian(180) - atan2(dir.x, dir.y);
	
	D3DXVec2Normalize(&dir, &dir);
	//m_vRotaion.z = acos(D3DXVec2Dot(&m_vPrevLookAt, &dir))* g_pTimeManager->GetDeltaTime();
	
	m_vPosition += dir * m_speed * g_pTimeManager->GetDeltaTime();
	m_vPrevLookAt = dir;

	// 애니메이션 업데이트
	m_pAnimation->SetPosition(m_vPosition);
	m_pAnimation->SetRotation(m_vRotaion.x, m_vRotaion.y, m_vRotaion.z);
	m_pAnimation->Play(m_eState);
	m_pAnimation->Update();

	m_hitCircle->SetPosition(m_vPosition.x, m_vPosition.y);
	m_hitCircle->Update();
}

void CarrotBullet::Render()
{
	if (m_active)
	{
		SAFE_RENDER(m_pAnimation);
		SAFE_RENDER(m_hitCircle);
	}
}

void CarrotBullet::Destory()
{
	this->m_vPosition = { 0, static_cast<float>(WINSIZEY)+400 };
	m_active = false;
}

void CarrotBullet::Spawn()
{
	m_active = true;
	this->m_vPosition = { static_cast<FLOAT>(rand() % WINSIZEX), static_cast<float>(WINSIZEY) - 5 };
}
