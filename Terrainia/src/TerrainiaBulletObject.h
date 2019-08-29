#pragma once
#include "DisplayableObject.h"

class TerrainiaMain;

class TerrainiaBulletObject :
	public DisplayableObject
{
public:
	TerrainiaBulletObject(TerrainiaMain* pEngine, int iMapX, int iMapY, int speed);
	~TerrainiaBulletObject();
	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	TerrainiaMain* m_pMainEngine;

	int m_iMapX;
	int m_iMapY;
	int m_iDir;
	int m_gravDir;
	int lastKeyPressed = 1;
	int m_speed;

public:
	void MouseDown(int iButton, int iX, int iY);
};

