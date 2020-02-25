#pragma once

class CarrotBullet;
class PlayerBullet;
class Boss
{
	enum STATE
	{
		READY = -1,
		EMERGNING,	// 맵에 처음 등장
		ATTACK_CARROT,
		ATTACK_BEAM,
		DIE,
		MAX
	};
private:
	D3DXVECTOR2			m_vPosition;
	float				m_hp;
	Animation<STATE>*	m_pAnimation;
	STATE				m_eState;
	Circle*				m_hitCircle;

	float				m_stateDelta;
	float				m_stateDelay;

	bool				m_isHit;		// 맞은 상태
	float				m_hitDelta;		// 맞고나서 지난 시간
	float				m_hitDelay;		// 맞은 상태 최대 지속시간
	float				m_hitEffectDelta;
	float				m_hitEffectDelay;
	bool				m_brightened;

	vector<CarrotBullet*> m_carrotBullets;
public:
	Boss();
	~Boss();

	void Init();
	void Update(PlayerBullet* playerBullets, D3DXVECTOR2 playerPos);
	void Render();
	void RenderBullets();

	vector<CarrotBullet*>& GetCarrotBulletes() { return m_carrotBullets; }
};

