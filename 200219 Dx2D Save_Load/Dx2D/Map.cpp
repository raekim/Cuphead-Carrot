#include "stdafx.h"
#include "Map.h"
#include "Player.h"


Map::Map()
	: m_pPlayer(NULL)
{
	g_pTextureManager->AddTexture(L"Back00", L"bg/bg00.png");
	g_pTextureManager->AddTexture(L"Back01", L"bg/bg01.png");
	g_pTextureManager->AddTexture(L"Back02", L"bg/bg02.png");
	g_pTextureManager->AddTexture(L"Back03", L"bg/bg03.png");
	g_pTextureManager->AddTexture(L"Back04", L"bg/bg04.png");

	m_arrLayer[0] = new Sprite(L"Back00");
	m_arrLayer[1] = new Sprite(L"Back01");
	m_arrLayer[2] = new Sprite(L"Back02");
	m_arrLayer[3] = new Sprite(L"Back03");
	m_arrLayer[4] = new Sprite(L"Back04");

	for(int i=0; i<5; ++i)
		m_arrLayer[i]->SetSize(1.2f, 1.0f);

	//g_pSoundManager->AddSound("Back", "Angel Attack.mp3", true, true);
}


Map::~Map()
{
	for (auto p : m_arrLayer)
		SAFE_DELETE(p);
}

void Map::Init()
{
	m_fGroundY = 130;
	
	for (int i = 0; i < 3; i++)
	{
		m_arryLayerPosX[i] = 0;
	}

	m_fMapPosX = 0;
	m_fMapPosY = 0;

	g_pSoundManager->Play("Back");

	m_mapFactor[0] = 0.0625f;
	m_mapFactor[1] = 0.03125f;
	m_mapFactor[2] = 0.015625f;
	m_mapFactor[3] = 0.0078125f;
	m_mapFactor[4] = 0.00390625;
}

void Map::Update()
{
	if (g_pKeyManager->isOnceKeyDown(VK_F1))
	{
		if (g_pSoundManager->IsPauseSound("Back"))
			g_pSoundManager->Resume("Back");
		else
			g_pSoundManager->Pause("Back");
	}
	else if (g_pKeyManager->isOnceKeyDown(VK_F2))
	{
		if (g_pSoundManager->IsPlaySound("Back"))
			g_pSoundManager->Stop("Back");
		else
			g_pSoundManager->Play("Back");
	}
	else if (g_pKeyManager->isStayKeyDown(VK_F3))
		g_pSoundManager->VolumeUp("Back");
	else if (g_pKeyManager->isStayKeyDown(VK_F4))
		g_pSoundManager->VolumeDown("Back");

	if (m_pPlayer)
	{
		if (m_pPlayer->GetMoveDirX() != X_DIR::Walk)
		{
			m_fMapPosX += m_pPlayer->GetSpeed() * (int)m_pPlayer->GetMoveDirX() * g_pTimeManager->GetDeltaTime();
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (m_arryLayerPosX[i] < -m_arrLayer[i]->GetWidth())
			m_arryLayerPosX[i] = 0;
		else if (m_arryLayerPosX[i] > 0)
			m_arryLayerPosX[i] = -m_arrLayer[i]->GetWidth();
	}

	m_fMapPosY = m_pPlayer->GetMoveY();
}

void Map::Render()
{

}

void Map::Render(int idx)
{
	m_arrLayer[idx]->SetPosition(WINSIZEX*0.5f - g_camX * m_mapFactor[idx], WINSIZEY*0.5f);
	m_arrLayer[idx]->Update();
	m_arrLayer[idx]->Render();
}