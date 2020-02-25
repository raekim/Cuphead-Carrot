#pragma once
class CarrotBullet
{
	enum class STATE
	{
		READY = -1,
		SHOOTING,
		DESTROYED
	};

	float				m_speed;
	D3DXVECTOR2			m_vPosition;
	D3DXVECTOR2			m_vPrevLookAt;
	D3DXVECTOR3			m_vRotaion;
	STATE				m_eState;
	Animation<STATE>*	m_pAnimation;
	Circle*				m_hitCircle;

	bool m_active;
public:
	CarrotBullet();
	~CarrotBullet();

	void Init();
	void Update(D3DXVECTOR2 playerPos);
	void Render();
	void Destory();
	void Spawn();

	bool IsActive() { return m_active; }
	Circle* GetCircle() { return m_hitCircle; }
	D3DXVECTOR2 GetPos() { return m_vPosition; }
};

