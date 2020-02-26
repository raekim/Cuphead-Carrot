#pragma once
class Particle
{
	D3DXVECTOR2 m_pos;
	Clip m_clip;	// 파티클 애니메이션 클립
	bool m_enabled;
public:
	Particle(Clip clip);
	Particle();
	~Particle();

	void SetPosition(D3DXVECTOR2 p) { m_pos = p; };
	void Init();
	void Play();
	void Update();
	void Render();

	bool IsEnabled() { return m_enabled; }
};

