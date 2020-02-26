#pragma once
#include "SingletonBase.h"
#include "Particle.h"

#define g_pParticleManager ParticleManager::GetInstance()

class ParticleManager : public SingletonBase<ParticleManager>
{
private:
	map<wstring, vector<Particle*>>	m_mapParticle;

public:
	ParticleManager();
	~ParticleManager();

	void Init();
	void Update();
	void Render();
	void AddParticle(wstring key, Particle* particle, int n = 1);
	void PlaceAndPlayParticle(wstring key, D3DXVECTOR2 pos);
};

