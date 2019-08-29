#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class TerrainiaMain;

class TerrainiaMobObject :
	public DisplayableObject
{
public:
	TerrainiaMobObject(TerrainiaMain* pEngine, int iMapX, int iMapY);
	~TerrainiaMobObject();
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
	bool collidingPlayer = false;
	bool MobCollideLeft = false;
	bool MobCollideRight = false;
	
	DisplayableObject* player = NULL;
	DisplayableObject* bullet = NULL;
	DisplayableObject* otherMob = NULL;
	int health = 100;

public:
	void turnLeft(int speed);
	void turnRight(int speed);
	void turnUp(int speed);
	void checkMovement();
	void checkHealth();
	int GetHealth();

protected:
	int m_speed;
};

