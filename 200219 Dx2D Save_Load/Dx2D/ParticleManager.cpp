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
	//  �̹� �����ϴ� key
	if (m_mapParticle.find(key) != m_mapParticle.end()) return;

	// ���� ������ ��ƼŬ �������� �����ϴ� ���� ����
	vector<Particle*> vec;
	for (int i = 0; i < n; ++i)
	{
		Particle* p = new Particle;
		*p = *particle;
		vec.push_back(p);
	}

	// �ʿ� ����
	m_mapParticle[key] = vec;
}

void ParticleManager::PlaceAndPlayParticle(wstring key, D3DXVECTOR2 pos)
{
	// ���� ȭ���� Ư�� ��ġ���� ��ƼŬ �ִϸ��̼��� ����
	auto particlesVector = m_mapParticle[key];
	for (auto particle : particlesVector)
	{
		if (particle->IsEnabled()) continue;
		particle->SetPosition(pos);
		particle->Play();
	}
}
