#include "header.h"
#include "templates.h"

#include "TerrainiaDirtObject.h"
#include "JPGImage.h"
#include "TileManager.h"


TerrainiaDirtObject::TerrainiaDirtObject(BaseEngine* pEngine, int iMapX, int iMapY)
	: DisplayableObject(pEngine)
{

	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = iMapX;
	m_iCurrentScreenY = m_iPreviousScreenY = iMapY;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the square size as both height and width
	m_iDrawWidth = 50;
	m_iDrawHeight = 50;
	// And make it visible
	SetVisible(true);
}


TerrainiaDirtObject::~TerrainiaDirtObject()
{
}


void TerrainiaDirtObject::Draw()
{

	//Draw an image loaded from a file.
	ImageData im;
	//Load the dirt image file into an image object - at the normal size
	im.LoadImage("dirt2.jpg");
	
	im.RenderImageWithMask(m_pEngine->GetBackground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY , m_iDrawWidth, m_iDrawHeight);

	StoreLastScreenPositionForUndraw();
}