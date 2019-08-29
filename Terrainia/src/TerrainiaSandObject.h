#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class TerrainiaMain;

class TerrainiaSandObject :
	public DisplayableObject
{
public:
	TerrainiaSandObject(TerrainiaMain* pEngine, int iMapX, int iMapY);
	~TerrainiaSandObject();
	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	TerrainiaMain* m_pMainEngine;
	MovementPosition m_oMover;

	int m_iMapX;
	int m_iMapY;
	int m_iDir;
	int m_gravDir;
};

