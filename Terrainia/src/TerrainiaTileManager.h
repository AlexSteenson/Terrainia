#pragma once
#include "TileManager.h"

class TerrainiaTileManager :
	public TileManager
{
public:
	TerrainiaTileManager();
	~TerrainiaTileManager();
	void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const;

};

class startScreen :
	public TerrainiaTileManager
{
	void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const;
};

class playScreen :
	public TerrainiaTileManager
{
	void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const;
};

class deathScreen :
	public TerrainiaTileManager
{
	void DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const;
};

