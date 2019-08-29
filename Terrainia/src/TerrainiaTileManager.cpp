#include "header.h"

#include "TerrainiaTileManager.h"
#include "JPGImage.h"


TerrainiaTileManager::TerrainiaTileManager()
	: TileManager(50, 50)
{
}


TerrainiaTileManager::~TerrainiaTileManager()
{
}


void TerrainiaTileManager::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	
}

void startScreen::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	//Loads main background image
	ImageData im;
	//Load the image file into an image object - at the normal size
	im.LoadImage("background.png");

	im.RenderImageWithMask(pSurface,
		0, 0, 0, 0,
		im.GetWidth(), im.GetHeight());

}
void playScreen::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	//Load dirt block image
	if (GetValue(iMapX, iMapY) == 1){
		//Draw an image loaded from a file.
		ImageData im;
		//Load the image file into an image object - at the normal size
		im.LoadImage("dirt2.jpg");

		im.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, GetTileWidth(), GetTileHeight());
	}
}

void deathScreen::DrawTileAt(BaseEngine* pEngine, SDL_Surface* pSurface, int iMapX, int iMapY, int iStartPositionScreenX, int iStartPositionScreenY) const
{
	//Black tile
	unsigned int iColour;
	iColour = 0x000000;
	pEngine->DrawRectangle(
		iStartPositionScreenX,
		iStartPositionScreenY,
		iStartPositionScreenX + GetTileWidth() - 1,
		iStartPositionScreenY + GetTileHeight() - 1,
		iColour,
		pSurface);

}

