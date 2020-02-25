#include "stdafx.h"
#include "Boss.h"
#include "PlayerBullet.h"
#include "CarrotBullet.h"


Boss::Boss()
{
	m_pAnimation = new Animation<STATE>;
	Clip* clip;

	// 당근 미사일 공격 동작 추가
	g_pTextureManager->AddTexture(L"carrot-attack", L"carrot-attack.png");
	clip = new Clip;
	for (int i = 0; i < 22; i++)
	{
		Sprite* sprite = new Sprite(L"carrot-attack", 10, 3, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(ATTACK_CARROT, clip);

	// 사망 동작 추가
	g_pTextureManager->AddTexture(L"carrot-die", L"carrot-die.png");
	clip = new Clip;
	for (int i = 0; i < 13; i++)
	{
		Sprite* sprite = new Sprite(L"carrot-die", 13, 1, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	for (int i = 12; i >= 0; i--)
	{
		Sprite* sprite = new Sprite(L"carrot-die", 13, 1, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(DIE, clip);

	m_pAnimation->SetScale(1.5f, 1.5f);

	m_hitCircle = new Circle;

	// 오브젝트 풀링
	for (int i = 0; i < 5; ++i)
	{
		m_carrotBullets.push_back(new CarrotBullet);
	}

	m_pBar = new ProgressBar(L"ProgressBack", L"ProgressFront");
}


Boss::~Boss()
{
}

void Boss::Init()
{
	m_vPosition = { WINSIZEX*0.5f, WINSIZEY*0.5f +100};
	m_eState = ATTACK_CARROT;
	m_pAnimation->Play(m_eState);

	// 콜라이더 초기화
	m_hitCircle->SetRadius(100);
	m_hitCircle->SetPosition(m_vPosition.x, m_vPosition.y + 90);
	m_hitCircle->SetDraw(false);
	m_hitCircle->SetBorder(true);

	m_hp = 100;

	m_isHit = false;
	m_hitDelta = 0;
	m_hitDelay = 0.5f;
	m_hitEffectDelay = 0.06f;
	m_hitEffectDelta = 0;

	m_stateDelta = 0;
	m_stateDelay = rand() % 10;
	if (m_stateDelay < 7) m_stateDelay = 7;

	if (m_pBar)
	{
		m_pBar->SetSize(m_pAnimation->GetWidth(), 30);
		m_pBar->Init();
	}
}

void Boss::Update(PlayerBullet* playerBullets, D3DXVECTOR2 playerPos)
{
	m_stateDelta += g_pTimeManager->GetDeltaTime();

	static float carrotDelta = 0;

	// 공격 패턴
	if (m_eState == STATE::ATTACK_CARROT)
	{	// 당근 발사
		carrotDelta += g_pTimeManager->GetDeltaTime();
		if (carrotDelta >= 2)
		{
			for (int i = 0; i < m_carrotBullets.size(); ++i)
			{
				if (!m_carrotBullets[i]->IsActive())
				{
					m_carrotBullets[i]->Spawn();
					break;
				}
			}
			carrotDelta = 0;
		}
	}
	else if (m_eState == STATE::ATTACK_BEAM)
	{

	}

	// 콜라이더 업데이트
	m_hitCircle->SetPosition(m_vPosition.x - g_camX * 0.03f, m_vPosition.y + 100);
	m_hitCircle->Update();

	// 플레이어 총알에 보스가 맞았나 판별
	auto bulletsPos = playerBullets->GetBulletsPos();
	float bulletRadius = playerBullets->GetBulletRadius();

	for (int i = 0; i < bulletsPos.size(); ++i)
	{
		auto bp = bulletsPos[i];
		// 맞았을 때
		if (CircleCollision(bp, bulletRadius, m_hitCircle->GetPosition(), m_hitCircle->GetRadius()))
		{
			m_hp -= 2;

			m_isHit = true;
			m_hitDelta = 0;
			m_pAnimation->SetColor({ 1.5f,1.5f,1.5f,1 });
			m_brightened = true;
			playerBullets->DestroyBullet(i);
			break;
		}
	}

	if (m_isHit && m_hitDelta <= m_hitDelay)
	{
		m_hitDelta += g_pTimeManager->GetDeltaTime();
		m_hitEffectDelta += g_pTimeManager->GetDeltaTime();

		// 맞았을 때 깜빡이는 효과
		if (m_hitEffectDelta > m_hitEffectDelay)
		{
			if (m_brightened)
			{
				m_pAnimation->SetColor({ 1,1,1,1 });
				m_brightened = false;
			}
			else
			{
				m_pAnimation->SetColor({ 1.5f,1.5f,1.5f,1 });
				m_brightened = true;
			}
			m_hitEffectDelta = 0;
		}
	}
	else
	{
		m_isHit = false;
		m_pAnimation->SetColor({ 1,1,1,1 });
		m_hitEffectDelta = 0;
	}

	if(m_hp <= 0)
	{
		// 보스 캐릭터 사망
		m_hp = 0;
		m_pAnimation->SetColor({ 1,1,1,1 });
		m_eState = STATE::DIE;
	}

	for (auto b : m_carrotBullets)
	{
		b->Update(playerPos);
	}

	m_pAnimation->SetPosition(m_vPosition.x - g_camX * 0.03f, m_vPosition.y);
	m_pAnimation->Play(m_eState);
	m_pAnimation->Update();

	if (m_pBar)
	{
		m_pBar->SetProgress(100, m_hp, true);
		m_pBar->SetPos(m_vPosition.x - m_pAnimation->GetWidth()*0.5f,
			m_vPosition.y + m_pAnimation->GetHeight()*0.25f);
		m_pBar->Update();
	}
}

void Boss::Render()
{
	SAFE_RENDER(m_pAnimation);
	SAFE_RENDER(m_hitCircle);
	SAFE_RENDER(m_pBar);
}

void Boss::RenderBullets()
{
	for (auto b : m_carrotBullets)
	{
		if(b->IsActive())
			b->Render();
	}
}
