#include "header.h"
#include "templates.h"

#include "TerrainiaMobBig.h"
#include "TerrainiaMobObject.h"
#include "TerrainiaMain.h"
#include "JPGImage.h"


TerrainiaMobBig::~TerrainiaMobBig()
{
}

void TerrainiaMobBig::Draw(){
	//If visible is false dont draw
	if (!IsVisible())
		return;

	//Draw an image loaded from a file.
	ImageData im;
	//Load the bigMob image file into an image object - at the normal size
	im.LoadImage("bigMob.png");

	im.RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight);

	StoreLastScreenPositionForUndraw();
}