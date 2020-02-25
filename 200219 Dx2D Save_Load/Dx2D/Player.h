#pragma once
#include "PlayerBullet.h"

class Map;

enum class X_DIR { LeftRun = -2, Left = -1, Walk, Right, RightRun};
class CarrotBullet;
class Player
{
	enum STATE
	{
		READY = -1,
		IDLE,
		RUN,
		JUMP,
		SHOOT_UP,
		SHOOT_DIAGONAL_UP,
		SHOOT_RUN_DIAGONAL_UP,
		HIT,
		DIE,
		MAX
	};

private:
	D3DXVECTOR2			m_vPosition;
	D3DXVECTOR3			m_vRotaion;
	bool				m_isLeft;
	Rect*				m_pRect;
	RECT				m_stRect;
	Circle*				m_hitCircle;

	Animation<STATE>*	m_pAnimation;
	STATE				m_eState;

	float				m_fSpeed;

	bool				m_isOnGround;
	bool				m_isJumping;
	float				m_fJumpPower;
	float				m_fElapsedGravity;

	float				m_fLeftEnd;
	float				m_fRightEnd;
	X_DIR				m_eMoveDirX;

	Map*				m_pMap;

	int					m_nNumJump;

	float				m_fMoveY;

	PlayerBullet*		m_pBullet;
	ProgressBar*		m_pBar;

	float				m_bulletDelta;	// ÃÑ¾ËÀ» ½ð ÈÄ Áö³­ ½Ã°£
	float				m_bulletDelay;  // ÃÑ¾Ë µô·¹ÀÌ ±âÁØ

	float				m_hp;

	float				m_hitDelta;
	float				m_hitDelay;
	bool				m_isHit;
	bool				m_isDead;

private:
	void CarrotBulletCollision(std::vector<CarrotBullet *> & m_carrotBullets);
	void PlayerGetsHit();
	void Control();
	void UpdateAnimation();
	void ShootBullet();
	void ProcessHit();
public:
	Player();
	~Player();

	void Init();
	void Update(vector<CarrotBullet*>& m_carrotBullets);
	void Render();

	float GetSpeed() { return m_fSpeed; }
	X_DIR GetMoveDirX() { return m_eMoveDirX; }

	RECT& GetRect() { return m_stRect; }

	void SetMap(Map* p) { m_pMap = p; }

	D3DXMATRIX& GetWMatrix() { return m_pRect->GetWMatrix(); }

	D3DXVECTOR2 GetPosition() { return m_vPosition; }
	float GetWidth() { return m_pAnimation[m_eState].GetWidth(); }
	float GetHeight() { return m_pAnimation[m_eState].GetHeight(); }
	D3DXVECTOR3 GetRotation() { return m_vRotaion; }

	PlayerBullet* GetBullets() { return m_pBullet; }

	float GetMoveY() { return m_fMoveY; }
};

