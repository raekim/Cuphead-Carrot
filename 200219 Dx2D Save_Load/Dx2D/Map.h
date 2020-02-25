#pragma once

class Player;

class Map
{
private:
	Sprite*			m_arrLayer[5];
	float			m_arryLayerPosX[5];
	float			m_mapFactor[5];	// 배경 움직임 곱하는 수
	float			m_fGroundY;

	Player*			m_pPlayer;
	float			m_fMapPosX;
	float			m_fMapPosY;

public:
	Map();
	~Map();

	void Init();
	void Update();
	void Render();

	void Render(int idx);
	void SetPlayer(Player* p) { m_pPlayer = p; }

	float GetGroundY() { return m_fGroundY; }
	float GetMapPosX() { return m_fMapPosX; }
	void SetMapPosX(float f) { m_fMapPosX = f; }
	float GetMapPosY() { return m_fMapPosY; }
};

