#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class TerrainiaMain;

class TerrainiaPlayerObject :
	public DisplayableObject
{
public:
	TerrainiaPlayerObject(TerrainiaMain* pEngine, int iMapX, int iMapY);
	~TerrainiaPlayerObject();
	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	TerrainiaMain* m_pMainEngine;
	MovementPosition m_oMover;

	int m_iMapX;
	int m_iMapY;
	int m_iDir;
	int m_gravDir;
	bool m_CollisionUp = false;
	bool m_CollisionDown = false;
	bool m_CollisionLeft = false;
	bool m_CollisionRight = false;
	DisplayableObject *mob = NULL;
	int remainingJump = 20;
	int maxJumpHeight = m_iCurrentScreenY + 150;
	double fontSize = 20.0;
	bool fontGrowing;
	bool mouseToggle = true;
	double health = 100.0;

public:
	void setHealth(double damage);
	void checkHealth();
	bool GetToggle();
};

