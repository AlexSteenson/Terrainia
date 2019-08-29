#include "header.h"
#include "templates.h"

#include "TerrainiaBulletObject.h"
#include "TerrainiaMain.h"


TerrainiaBulletObject::TerrainiaBulletObject(TerrainiaMain* pEngine, int iMapX, int iMapY, int speed)
	: DisplayableObject(pEngine)
	, m_pMainEngine(pEngine)
	, m_iMapX(iMapX)
	, m_iMapY(iMapY)
	, m_iDir(0)
	, m_speed(speed)
{
	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = iMapX;
	m_iCurrentScreenY = m_iPreviousScreenY = iMapY;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the square size as both height and width
	m_iDrawWidth = 13;
	m_iDrawHeight = 4;
	// And make it visible
	SetVisible(false);
}


TerrainiaBulletObject::~TerrainiaBulletObject()
{
}

void TerrainiaBulletObject::Draw(){
	//If its not visible move out of way and dont draw
	if (!IsVisible()){
		m_iCurrentScreenX = 1505;
		m_iDir = 0;
		return;
	}
		

	GetEngine()->DrawScreenRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY - m_iDrawHeight - 1,
		0x000000);

	StoreLastScreenPositionForUndraw();

}
void TerrainiaBulletObject::DoUpdate(int iCurrentTime){

	//Move in players facing direction
	m_iCurrentScreenX += m_iDir;

	//If it hits a wall bouce off
	if (m_iCurrentScreenX <= 0 || m_iCurrentScreenX + m_iDrawWidth >= m_pEngine->GetScreenWidth())
		m_iDir *= -1;

	//Get player object
	DisplayableObject* player;
	DisplayableObject* pObject;
	for (int iObjectId = 0;
		(pObject = m_pMainEngine->GetDisplayableObject(iObjectId)) != NULL;
	iObjectId++)
	{
		if (pObject == this) // This is us, skip it
			continue;
		else if (pObject->GetHeight() == 100){ //Player object
			player = pObject;
		}
	}


	//If player turned left record it
	if ((GetEngine()->IsKeyPressed(SDLK_LEFT) || GetEngine()->IsKeyPressed(SDLK_a))){
		lastKeyPressed = -1;
	}
	//if player turned right, record it
	if ((GetEngine()->IsKeyPressed(SDLK_RIGHT) || GetEngine()->IsKeyPressed(SDLK_d))){
		lastKeyPressed = 1;
	}
	
	//If the player pressed f to fire a bullet set visible at player x and y and move in direction of player
	if (m_pEngine->IsKeyPressed(SDLK_f)){
		//See if mouse toggle is on
		if (m_pMainEngine->GetMouseToggle()){
			//Fire bullet left if cursor is left of the player
			if (GetEngine()->GetCurrentMouseX() < player->GetXCentre()){
				m_iCurrentScreenX = player->GetXCentre() - (player->GetWidth() / 2);
				m_iDir = -m_speed;
			}
			else{ //else fire right
				m_iCurrentScreenX = player->GetXCentre() + (player->GetWidth() / 2);
				m_iDir = m_speed;
			}
		}
		else{ //Keyboard input, use last recorded direction
			m_iCurrentScreenX = player->GetXCentre() + (player->GetWidth() / 2 * lastKeyPressed);
			m_iDir = m_speed * lastKeyPressed;
		}
		//Set bullet Y to players centre Y
		m_iCurrentScreenY = player->GetYCentre();
		SetVisible(true);
	}

	RedrawWholeScreen();
}
