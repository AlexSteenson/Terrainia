#pragma once
#include "TerrainiaMobObject.h"

class TerrainiaMobSlow :
	public TerrainiaMobObject
{
public:
	TerrainiaMobSlow(TerrainiaMain* pEngine, int iMapX, int iMapY, int speed);
	~TerrainiaMobSlow();
	void Draw();
};

