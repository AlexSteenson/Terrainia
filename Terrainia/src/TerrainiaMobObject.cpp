#include "header.h"
#include "templates.h"

#include "TerrainiaMobObject.h"
#include "TerrainiaMobGreen.h"
#include "TerrainiaMain.h"
#include "TerrainiaPlayerObject.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


TerrainiaMobObject::TerrainiaMobObject(TerrainiaMain* pEngine, int iMapX, int iMapY)
	: DisplayableObject(pEngine)
	, m_pMainEngine(pEngine)
	, m_iMapX(iMapX)
	, m_iMapY(iMapY)
	, m_iDir(0)
	, m_speed(2)
{

	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = iMapX;
	m_iCurrentScreenY = m_iPreviousScreenY = iMapY;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the square size as both height and width
	m_iDrawWidth = 50;
	m_iDrawHeight = 99;
	// And make it visible
	SetVisible(true);

}


TerrainiaMobObject::~TerrainiaMobObject()
{
}


void TerrainiaMobObject::Draw()
{
	
	GetEngine()->DrawScreenRectangle(
		m_iCurrentScreenX + m_iDrawWidth - 1, 
		m_iCurrentScreenY - m_iDrawHeight - 1,
		m_iCurrentScreenX, m_iCurrentScreenY,
		0x00ffa8);
	
	StoreLastScreenPositionForUndraw();
}


void TerrainiaMobObject::DoUpdate(int iCurrentTime)
{
	m_gravDir = 5;
	m_CollisionUp = false;
	m_CollisionDown = false;
	m_CollisionLeft = false;
	m_CollisionRight = false;
	MobCollideLeft = false;
	MobCollideRight = false;

	// Iterate through the objects
	// We are looking for one which is too close to us
	DisplayableObject* pObject;
	
	for (int iObjectId = 0; (pObject = m_pMainEngine->GetDisplayableObject(iObjectId)) != NULL; iObjectId++)
	{
		
		int pObjectXCentre = pObject->GetXCentre();
		int pObjectYCentre = pObject->GetYCentre();
		int pObjectWidth = pObject->GetWidth();
		int pObjectHeight = pObject->GetHeight();

		if (pObject == this) // This is us, skip it
			continue;
		else if (pObjectHeight == 100) //player
			player = pObject;
		else if (pObjectHeight == 99){ //another mob
			otherMob = pObject;
		}
		else if (pObjectHeight == 4){ //bullet
			bullet = pObject;
		}
		
		//Left collision
		if (m_iCurrentScreenX <= pObjectXCentre + pObjectWidth / 2 && m_iCurrentScreenX > pObjectXCentre && m_iCurrentScreenY + m_iDrawHeight > pObjectYCentre - pObjectHeight / 2){
			if (pObject == otherMob){
				MobCollideLeft = true;
			}
			m_CollisionLeft = true;
		}
		//Right collision
		if (m_iCurrentScreenX + m_iDrawWidth >= pObjectXCentre - pObjectWidth / 2 && m_iCurrentScreenX + m_iDrawWidth < pObjectXCentre && m_iCurrentScreenY + m_iDrawHeight > pObjectYCentre - pObjectHeight / 2){
			if (pObject == otherMob){
				MobCollideRight = true;
			}
			m_CollisionRight = true;
		}

		//Bullet collision
		if (pObject == bullet){
			if (m_iCurrentScreenX + m_iDrawWidth >= pObjectXCentre - pObjectWidth / 2 && m_iCurrentScreenX + m_iDrawWidth / 2 < pObjectXCentre - pObjectWidth / 2 && m_iCurrentScreenY + m_iDrawHeight > pObjectYCentre - pObjectHeight / 2 && m_iCurrentScreenY < pObjectYCentre + pObjectHeight / 2){
				//Reduce health if hit
				health -= 30;
				bullet->SetVisible(false);
			}
			else if (m_iCurrentScreenX <= pObjectXCentre + pObjectWidth / 2 && m_iCurrentScreenX + m_iDrawWidth / 2 >= pObjectXCentre + pObjectWidth / 2 && m_iCurrentScreenY + m_iDrawHeight> pObjectYCentre - pObjectHeight / 2 && m_iCurrentScreenY < pObjectYCentre + pObjectHeight / 2){
				//Reduce health if hit
				health -= 30;
				bullet->SetVisible(false);
			}
		}

		//Collision detection down
		if (pObjectYCentre - pObjectHeight / 2 <= m_iCurrentScreenY + m_iDrawHeight && pObjectXCentre + pObjectWidth / 2 > m_iCurrentScreenX && pObjectXCentre - pObjectWidth / 2 < m_iCurrentScreenX + m_iDrawWidth){
			m_CollisionDown = true;
		}
	}

	//check where to move
	checkMovement();

	//check if dead
	checkHealth();

	//Stops going off screen
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX >= GetEngine()->GetScreenWidth() - m_iDrawWidth)
		m_iCurrentScreenX = GetEngine()->GetScreenWidth() - m_iDrawWidth;
	if (m_iCurrentScreenY < m_iDrawHeight)
		m_iCurrentScreenY = m_iDrawHeight;
	if (m_iCurrentScreenY >= GetEngine()->GetScreenHeight())
		m_iCurrentScreenY = GetEngine()->GetScreenHeight();

	//If player is not visible, set not visible, for death screen
	if (!player->IsVisible()){
		SetVisible(false);
	}

	// Ensure that the object gets redrawn on the display, if something changed
	RedrawObjects();
}


void TerrainiaMobObject::turnLeft(int speed)
{
	//Move mob left
	if (!m_CollisionLeft)
		m_iCurrentScreenX -= speed;

}

void TerrainiaMobObject::turnRight(int speed)
{
	//Move mob right
	if (!m_CollisionRight)
		m_iCurrentScreenX += speed;

}
void TerrainiaMobObject::turnUp(int speed)
{
	//Make mob jump
	if (!m_CollisionUp)
		m_iCurrentScreenY -= speed;

}


void TerrainiaMobObject::checkMovement()
{
	//Take effect of gravity
	if (!m_CollisionDown){
		m_iCurrentScreenY += m_gravDir;
	}

	if (player != NULL){ //If it sound the player
		if (player->GetXCentre() + player->GetWidth() / 2 < m_iCurrentScreenX){ //Player is left
			if (!MobCollideLeft){ //If not colliding with a mob move
				if (m_CollisionLeft){ //Jump if colliding left
					turnUp(m_speed * 7);
				}
				else{ //Move left
					turnLeft(m_speed);
				}
			}
		}
		else if (player->GetXCentre() - player->GetWidth() / 2 > m_iCurrentScreenX + m_iDrawWidth){ //Player is right
			if (!MobCollideRight){ //If not colliding with mob, move
				if (m_CollisionRight){ //Jump if colliding right
					turnUp(m_speed * 7);
				}
				else{ //Move right
					turnRight(m_speed);
				}
			}
		}
	}
}

void TerrainiaMobObject::checkHealth()
{
	//If dead, reset X and Y and health 
	if (health <= 0){

		//Respawn on other side of map
		m_iCurrentScreenY = 400;
		//m_iCurrentScreenX = 1500 - otherMob->GetXCentre();
		m_iCurrentScreenX = rand() % 1500;
		//Reset health
		health = 100;
		int score = 0;
		string temp = "";

		//Read current score from file
		ifstream scoreFile("score.txt");
		if (scoreFile.is_open())
		{
			while (getline(scoreFile, temp))
			{
				score = stoi(temp);
			}
			scoreFile.close();
		}
		else
			cout << "Unable to open file";

		//Increment score and rewrite it
		score++;
		ofstream myfile;
		myfile.open("score.txt");
		myfile << score;
		myfile.close();

		//If the score is a multiple of 7 add another green mob
		if (score % 7 == 0){
			m_pMainEngine->AddObjectToArray(new TerrainiaMobGreen(m_pMainEngine, 200, 200, 3));
		}
	}
}


int TerrainiaMobObject::GetHealth()
{
	return health;
}
