#pragma once

enum class BTYPE { SPEED, INTERPOLATION, BEZIER };

class PlayerBullet
{
	enum class STATE
	{
		READY=-1,
		SHOOTING,
		DESTROYED
	};
	struct tagBullet
	{
		BTYPE		type;

		D3DXVECTOR2 start;
		D3DXVECTOR2 via;
		D3DXVECTOR2 dest;
		D3DXVECTOR3	m_vRotaion;
		float elapsedTime;
		float time;
		Animation<STATE>	anim;

		D3DXVECTOR2 pos;
		float speed;
	};
private:
	vector<tagBullet>	m_vecBullet;
	vector<D3DXVECTOR2>	m_vecBulletPos;
	float				m_hitCircleRadius;
	Circle*				m_pCircle;
public:
	PlayerBullet();
	~PlayerBullet();

	void Init();
	void Update();
	void Render();

	void ShootBullet(D3DXVECTOR2 start, D3DXVECTOR2 dest, BTYPE type, float f = 500);
	vector<D3DXVECTOR2>& GetBulletsPos() { return m_vecBulletPos; }
	float GetBulletRadius() { return m_hitCircleRadius; }
	void DestroyBullet(int idx) { m_vecBullet.erase(m_vecBullet.begin() + idx); }
};

