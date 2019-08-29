#pragma once
#include "TerrainiaMobObject.h"


class TerrainiaMobBig :
	public TerrainiaMobObject
{

public:
	TerrainiaMobBig(TerrainiaMain* pEngine, int iMapX, int iMapY, int speed)
		: TerrainiaMobObject(pEngine, iMapX, iMapY)
	{
		m_speed = speed;
	}
	~TerrainiaMobBig();

	void TerrainiaMobBig::Draw();
};