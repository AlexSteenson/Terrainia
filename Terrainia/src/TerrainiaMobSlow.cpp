#include "header.h"
#include "templates.h"

#include "TerrainiaMobSlow.h"
#include "TerrainiaMobObject.h"
#include "TerrainiaMain.h"
#include "JPGImage.h"


TerrainiaMobSlow::TerrainiaMobSlow(TerrainiaMain* pEngine, int iMapX, int iMapY, int speed)
	:TerrainiaMobObject(pEngine, iMapX, iMapY)
{
	m_speed = speed; // set the speed
}


TerrainiaMobSlow::~TerrainiaMobSlow()
{
}


void TerrainiaMobSlow::Draw()
{
	//If set to not visible, dont draw
	if (!IsVisible())
		return;

	GetEngine()->Redraw(false);


	//Draw an image loaded from a file.
	ImageData im;
	//Load the slowMob image file into an image object - at the normal size
	im.LoadImage("slowMob.png");

	im.RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight);

	StoreLastScreenPositionForUndraw();
}
