#pragma once
#include "TerrainiaMobObject.h"

class TerrainiaMobGreen :
	public TerrainiaMobObject
{

public:
	TerrainiaMobGreen(TerrainiaMain* pEngine, int iMapX, int iMapY, int speed)
		: TerrainiaMobObject(pEngine, iMapX, iMapY)
	{
		m_speed = speed;
	}
	~TerrainiaMobGreen();

	void TerrainiaMobGreen::Draw();
};
