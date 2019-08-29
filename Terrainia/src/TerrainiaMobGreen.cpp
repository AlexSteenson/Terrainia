#include "header.h"
#include "templates.h"

#include "TerrainiaMobGreen.h"
#include "TerrainiaMobObject.h"
#include "TerrainiaMain.h"
#include "JPGImage.h"


TerrainiaMobGreen::~TerrainiaMobGreen()
{
}

void TerrainiaMobGreen::Draw(){
	//If visible set to false, dont draw
	if (!IsVisible())
		return;

	GetEngine()->Redraw(false);


	//Draw an image loaded from a file.
	ImageData im;
	//Load the green mob image file into an image object - at the normal size
	im.LoadImage("greenMob.png");

	im.RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight);

	StoreLastScreenPositionForUndraw();
}