#pragma once

class CarrotBullet;
class PlayerBullet;
class Boss
{
	enum STATE
	{
		READY = -1,
		EMERGNING,	// �ʿ� ó�� ����
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

	bool				m_isHit;		// ���� ����
	float				m_hitDelta;		// �°��� ���� �ð�
	float				m_hitDelay;		// ���� ���� �ִ� ���ӽð�
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

