#include "stdafx.h"
#include "Particle.h"


Particle::Particle()
{
}

Particle::Particle(Clip clip)
{
	m_clip = clip;
}


Particle::~Particle()
{
}

void Particle::Init()
{
	m_enabled = false;
}

void Particle::Play()
{
	m_enabled = true;
	m_clip.Play();
}

void Particle::Update()
{
	m_enabled = m_clip.IsPlaying();

	if (!m_enabled) return;

	m_clip.SetPosition({ m_pos.x - g_camX, m_pos.y });
	m_clip.Update();
}

void Particle::Render()
{
	if (m_enabled)
	{
		m_clip.Render();
	}
}
