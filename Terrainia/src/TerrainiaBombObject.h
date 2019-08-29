#pragma once
#include "DisplayableObject.h"
#include "TerrainiaMain.h"
#include "MovementPosition.h"

class TerrainiaBombObject :
	public DisplayableObject
{
public:
	TerrainiaBombObject(TerrainiaMain* pEngine, int iMapX, int iMapY);
	~TerrainiaBombObject();

private:
	TerrainiaMain* m_pMainEngine;
	MovementPosition m_oMover;


	int m_iMapX;
	int m_iMapY;
	int m_iDir;
	int m_gravDir;
};

