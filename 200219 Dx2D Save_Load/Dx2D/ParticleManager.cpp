#include "stdafx.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
}


ParticleManager::~ParticleManager()
{
}

void ParticleManager::Init()
{
	for (auto it : m_mapParticle)
	{
		auto particlesVector = it.second;
		for (auto particle : particlesVector)
		{
			particle->Init();
		}
	}
}

void ParticleManager::Update()
{
	for (auto it : m_mapParticle)
	{
		auto particlesVector = it.second;
		for (auto particle : particlesVector)
		{
			particle->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto it : m_mapParticle)
	{
		auto particlesVector = it.second;
		for (auto particle : particlesVector)
		{
			particle->Render();
		}
	}
}

void ParticleManager::AddParticle(wstring key, Particle * particle, int n)
{
	//  이미 존재하는 key
	if (m_mapParticle.find(key) != m_mapParticle.end()) return;

	// 같은 종류의 파티클 여러개를 저장하는 벡터 생성
	vector<Particle*> vec;
	for (int i = 0; i < n; ++i)
	{
		Particle* p = new Particle;
		*p = *particle;
		vec.push_back(p);
	}

	// 맵에 저장
	m_mapParticle[key] = vec;
}

void ParticleManager::PlaceAndPlayParticle(wstring key, D3DXVECTOR2 pos)
{
	// 게임 화면의 특정 위치에서 파티클 애니메이션을 실행
	auto particlesVector = m_mapParticle[key];
	for (auto particle : particlesVector)
	{
		if (particle->IsEnabled()) continue;
		particle->SetPosition(pos);
		particle->Play();
	}
}
