#include "stdafx.h"
#include "Player.h"
#include "Map.h"
#include "CarrotBullet.h"

Player::Player()
{
	m_pAnimation = new Animation<STATE>;

	// 대기 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-idle", L"Cuphead/Idle/Idle-sheet.png");
	Clip* clip = new Clip;
	for (int i = 0; i < 5; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-idle", 5, 1, i);
		clip->AddFrame(sprite, 1/18.f);
	}
	for (int i = 4; i >= 0; i--)
	{
		Sprite* sprite = new Sprite(L"cuphead-idle", 5, 1, i);
		clip->AddFrame(sprite, 1 / 18.f);
	}
	m_pAnimation->AddClip(IDLE, clip);

	// 달리기 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-run-normal", L"Cuphead/Run/Normal/run-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-run-normal", 4, 4, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(RUN, clip);
	
	// 점프 (업,다운) 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-jump", L"Cuphead/Jump/Cuphead/jump-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 8; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-jump", 1, 8, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(JUMP, clip);

	// 공격 (위) 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-shoot-up", L"Cuphead/Shoot/Up/shoot-up-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 3; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-shoot-up", 3, 1, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(SHOOT_UP, clip);

	// 공격 (대각선 위) 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-shoot-diag-up", L"Cuphead/Shoot/Diagonal Up/shoot-diag-up-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 3; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-shoot-diag-up", 3, 1, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(SHOOT_DIAGONAL_UP, clip);
	
	// 달리면서 공격 (대각선 위) 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-run-diag-up", L"Cuphead/run-shoot-diag-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-run-diag-up", 4, 4, i);
		clip->AddFrame(sprite, 1 / 24.0f);
	}
	m_pAnimation->AddClip(SHOOT_RUN_DIAGONAL_UP, clip);

	// 히트 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-hit-air", L"Cuphead/Hit/Air/hit-air-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 6; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-hit-air", 6, 1, i);
		clip->AddFrame(sprite, 1 / 16.0f);
	}
	clip->SetOnce();
	m_pAnimation->AddClip(HIT, clip);

	// 사망 동작 추가
	g_pTextureManager->AddTexture(L"cuphead-ghost", L"Cuphead/Ghost/ghost-sheet.png");
	clip = new Clip;
	for (int i = 0; i < 24; i++)
	{
		Sprite* sprite = new Sprite(L"cuphead-ghost", 10, 3, i);
		clip->AddFrame(sprite, 1 / 16.0f);
	}
	clip->SetScale(0.9f, 0.9f);
	m_pAnimation->AddClip(DIE, clip);
	
	m_hitCircle = new Circle;
	m_pBullet = new PlayerBullet;
	
	g_pTextureManager->AddTexture(L"ProgressBack", L"progressBarBack.png");
	g_pTextureManager->AddTexture(L"ProgressFront", L"progressBarFront.png");
	
	m_pBar = new ProgressBar(L"ProgressBack", L"ProgressFront");

	m_hitDelay = 0.5f;
}

Player::~Player()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pRect);
	SAFE_DELETE(m_pBullet);
}

void Player::Init()
{
	m_isDead = false;
	m_hp = 3;	// 세 번 맞으면 죽는다

	m_eState = IDLE;
	m_pAnimation->Play(m_eState);

	m_vPosition = D3DXVECTOR2(DEFAULT_X, m_pMap->GetGroundY());
	m_vRotaion = D3DXVECTOR3(0, 0, 0);
	//m_pAnimation->SetScale(0.5f, 0.5f);

	m_hitCircle->SetRadius(50);
	m_hitCircle->SetDraw(false);
	m_hitCircle->SetBorder(true);

	m_fSpeed = 440.0f;

	m_isOnGround = true;
	m_isJumping = false;
	m_fJumpPower = 450.0f;

	m_fLeftEnd = WINSIZEX * 0.45f;
	m_fRightEnd = WINSIZEX * 0.55f;

	m_isLeft = false;

	m_nNumJump = 0;

	m_fMoveY = 150.0f;

	SAFE_INIT(m_pBullet);

	if (m_pBar)
	{
		m_pBar->SetSize(m_pAnimation->GetWidth() * 0.5f, 20);
		m_pBar->Init();
	}

	m_bulletDelay = 0.15f;

	m_isHit = false;
}

void Player::Update(vector<CarrotBullet*>& m_carrotBullets)
{
	m_bulletDelta += g_pTimeManager->GetDeltaTime();

	if (m_isDead)
	{
		// 사망 시 유령이 되어 점점 위로 올라감
		m_vPosition.y += m_fSpeed * 0.4f*g_pTimeManager->GetDeltaTime();
	}
	ProcessHit();

	Control();
	
	UpdateAnimation();

	// Update hit circle
	m_hitCircle->SetPosition(m_vPosition.x, m_vPosition.y);
	m_hitCircle->Update();

	CarrotBulletCollision(m_carrotBullets);
	SAFE_UPDATE(m_pBullet);

	if (m_pBar)
	{
		m_pBar->SetProgress(3, m_hp, true);
		m_pBar->SetPos(m_vPosition.x - m_pAnimation->GetWidth() * 0.25f,
			m_vPosition.y + m_pAnimation->GetHeight()*0.5f);
		m_pBar->Update();
	}
}

void Player::ProcessHit()
{
	if (m_isHit)
	{
		m_hitDelta += g_pTimeManager->GetDeltaTime();
		if (m_hitDelta >= m_hitDelay)
		{
			m_isHit = false;
			m_hitDelta = 0;
		}
	}
}

void Player::Control()
{
	if (m_isHit || m_isDead) return;

	static int currState = 0;
	bool pressedKey = false;
	float speedRate = 1.0f;

	if (g_pKeyManager->isStayKeyDown(VK_LSHIFT))
		speedRate = 2.0f;

	if (g_pKeyManager->isStayKeyDown('W'))
	{
		// 위 쏘기
		currState = 3;
		pressedKey = true;
	}

	if (g_pKeyManager->isStayKeyDown('Q'))
	{
		// 왼쪽 대각선 쏘기
		currState = 4;
		if (!m_isLeft)
		{
			m_isLeft = true;
			m_vRotaion.y = D3DX_PI;
		}
		pressedKey = true;
	}

	if (g_pKeyManager->isStayKeyDown('E'))
	{
		// 오른쪽 대각선 쏘기
		currState = 4;
		if (m_isLeft)
		{
			m_isLeft = false;
			m_vRotaion.y = 0.0f;
		}
		pressedKey = true;
	}

	if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
	{
		// 오른쪽 달리기
		m_isLeft = false;
		m_vRotaion.y = 0.0f;

		if (m_vPosition.x < WINSIZEX)
		{
			m_vPosition.x += m_fSpeed * speedRate * g_pTimeManager->GetDeltaTime();
			if (speedRate == 1.0f)
				m_eMoveDirX = X_DIR::Right;
			else
				m_eMoveDirX = X_DIR::RightRun;
			g_camX += m_fSpeed * speedRate * g_pTimeManager->GetDeltaTime();
		}
		else
		{
			m_eMoveDirX = X_DIR::Walk;
		}

		currState = 1;
		if (g_pKeyManager->isStayKeyDown('E')) currState = 5;

		pressedKey = true;
	}
	else if (g_pKeyManager->isStayKeyDown(VK_LEFT))
	{
		// 왼쪽 달리기
		m_isLeft = true;
		m_vRotaion.y = D3DX_PI;

		if (m_vPosition.x > 0)
		{
			m_vPosition.x -= m_fSpeed * speedRate * g_pTimeManager->GetDeltaTime();
			if (speedRate == 1.0f)
				m_eMoveDirX = X_DIR::Left;
			else
				m_eMoveDirX = X_DIR::LeftRun;
			g_camX -= m_fSpeed * speedRate * g_pTimeManager->GetDeltaTime();
		}
		else
		{
			m_eMoveDirX = X_DIR::Walk;
		}

		currState = 1;
		if (g_pKeyManager->isStayKeyDown('Q')) currState = 5;

		pressedKey = true;
	}

	if (!pressedKey)
	{
		m_eMoveDirX = X_DIR::Walk;
		currState = 0;
	}

	if (g_pKeyManager->isOnceKeyDown(VK_SPACE) && m_isJumping == false)
	{
		m_isOnGround = false;
		m_isJumping = true;
		m_fElapsedGravity = 0.0f;
		m_nNumJump++;
	}

	if (m_isJumping)
	{
		currState = 2; // jump up, down 상관없이 동일한 애니메이션을 재생합니다

		float speed = (m_fJumpPower - m_fElapsedGravity) * g_pTimeManager->GetDeltaTime();
		m_fElapsedGravity += GRAVITY * g_pTimeManager->GetDeltaTime();

		if (m_fJumpPower > m_fElapsedGravity)	// 위로 올라갈 때
		{
			m_vPosition.y += speed;
		}
		else // 아래로 떨어질 때
		{
			m_vPosition.y += speed;
		}

		if (m_vPosition.y <= m_pMap->GetGroundY() + 150 - m_fMoveY)
		{
			m_vPosition.y = m_pMap->GetGroundY();
			m_isJumping = false;
			m_isOnGround = true;
			m_fElapsedGravity = 0.0f;
		}
	}

	m_eState = (STATE)currState;

	ShootBullet();
}

void Player::UpdateAnimation()
{
	m_pAnimation->SetPosition(m_vPosition.x, m_vPosition.y);
	m_pAnimation->SetRotation(m_vRotaion.x, m_vRotaion.y, m_vRotaion.z);

	// 애니메이션 이미지 offset 해결
	if (m_eState == STATE::SHOOT_UP)
	{
		m_pAnimation->SetPosition(m_vPosition.x - (7 * (int)m_eMoveDirX), m_vPosition.y + 18);
	}
	else if (m_eState == STATE::SHOOT_DIAGONAL_UP)
	{
		m_pAnimation->SetPosition(m_vPosition.x + (7 * (int)m_eMoveDirX), m_vPosition.y + 9);
	}
	else if (m_eState == STATE::HIT)
	{
		m_pAnimation->SetPosition(m_vPosition.x, m_vPosition.y + 13);
	}

	// ******* Play 먼저 하고 Update!!! *******중요!!
	// Play를 먼저 하는 이유 : 새로운 애니메이션을 플레이 하게 된 경우 때문.
	// Update를 먼저 하면 현재 플레이 해야 할 애니메이션이 아닌, 이전에 플레이 하고 있었던 애니메이션을 쌩뚱맞게 업데이트 하게 된다.
	m_pAnimation->Play(m_eState);
	m_pAnimation->Update();
}

void Player::ShootBullet()
{
	// 총알 발사
	if (m_bulletDelta >= m_bulletDelay)
	{
		static float bulletSpeed = 1200.0f;
		switch (m_eState)
		{
		case STATE::SHOOT_UP:
		{
			D3DXVECTOR2 start;
			if (m_isLeft)
			{
				start = m_vPosition + D3DXVECTOR2(-30, 70);
				m_pBullet->ShootBullet(start, { start.x, start.y + 10 }, BTYPE::SPEED, bulletSpeed);
			}
			else
			{
				start = m_vPosition + D3DXVECTOR2(30, 70);
				m_pBullet->ShootBullet(start, { start.x, start.y + 10 }, BTYPE::SPEED, bulletSpeed);
			}
			m_bulletDelta = 0;
		}
		break;
		case STATE::SHOOT_DIAGONAL_UP:
		case STATE::SHOOT_RUN_DIAGONAL_UP:
		{
			D3DXVECTOR2 start;
			if (m_isLeft)
			{
				start = m_vPosition + D3DXVECTOR2(-50, 30);
				m_pBullet->ShootBullet(start, { start.x - 10, start.y + 10 }, BTYPE::SPEED, bulletSpeed);
			}
			else
			{
				start = m_vPosition + D3DXVECTOR2(50, 30);
				m_pBullet->ShootBullet(start, { start.x + 10, start.y + 10 }, BTYPE::SPEED, bulletSpeed);
			}
			m_bulletDelta = 0;
		}
		break;
		default:
			break;
		}
	}
}

void Player::CarrotBulletCollision(std::vector<CarrotBullet *> & m_carrotBullets)
{
	auto myBulletsPos = m_pBullet->GetBulletsPos();
	auto myBulletRadius = m_pBullet->GetBulletRadius();
	// 플레이어가 쏜 총알이 당근을 맞혔는가?
	for (int i = 0; i < myBulletsPos.size(); ++i)
	{
		auto b = myBulletsPos[i];
		for (auto c : m_carrotBullets)
		{
			if (CircleCollision(b, myBulletRadius, c->GetPos(), c->GetCircle()->GetRadius()))
			{
				c->Destory();
				i = myBulletsPos.size();
				break;
			}
		}
	}

	// 플레이어가 당근에 맞았는가?
	for (auto c : m_carrotBullets)
	{
		if (CircleCollision(m_vPosition, m_hitCircle->GetRadius(), c->GetPos(), c->GetCircle()->GetRadius()))
		{
			c->Destory();
			if (!m_isHit && !m_isDead)
			{
				PlayerGetsHit();
			}
		}
	}
}

void Player::PlayerGetsHit()
{
	// 맞았다
	m_eState = STATE::HIT;
	m_isHit = true;
	m_hitDelta = 0;
	m_hp--;

	// 죽음 처리
	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isDead = true;
		m_eState = STATE::DIE;
	}
}

void Player::Render()
{
	SAFE_RENDER(m_pAnimation);
	SAFE_RENDER(m_hitCircle);
	SAFE_RENDER(m_pBar);

	//g_pTextManager->IntRender(m_nNumJump, 10, WINSIZEY - 120, 50, NumberType::Blue);

	SAFE_RENDER(m_pBullet);
}
