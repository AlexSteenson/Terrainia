#pragma once
#include "DisplayableObject.h"
class TerrainiaDirtObject :
	public DisplayableObject
{
public:
	TerrainiaDirtObject(BaseEngine* pEngine, int iMapX, int iMapY);
	~TerrainiaDirtObject();
	void Draw();
};

