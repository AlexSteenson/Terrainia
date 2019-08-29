#include "header.h"
#include "templates.h"

#include "TerrainiaPlayerObject.h"
#include "TerrainiaMain.h"
#include "TerrainiaTileManager.h"
#include "JPGImage.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


TerrainiaPlayerObject::TerrainiaPlayerObject(TerrainiaMain* pEngine, int iMapX, int iMapY)
	: DisplayableObject(pEngine)
	, m_pMainEngine(pEngine)
	, m_iMapX(iMapX)
	, m_iMapY(iMapY)
	, m_iDir(0)
{

	// Current and previous coordinates for the object - set them the same initially
	m_iCurrentScreenX = m_iPreviousScreenX = iMapX;
	m_iCurrentScreenY = m_iPreviousScreenY = iMapY;
	// The object coordinate will be the top left of the object
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	// Record the square size as both height and width
	m_iDrawWidth = 50;
	m_iDrawHeight = 100;
	// And make it visible
	SetVisible(true);

}


TerrainiaPlayerObject::~TerrainiaPlayerObject()
{
}


void TerrainiaPlayerObject::Draw()
{
	//If not visible, dont draw
	if (!IsVisible())
		return;

	//Display health string to be displayed
	char buf[128];
	sprintf(buf, "Health = %.1f", health);
	GetEngine()->CopyBackgroundPixels(450, 0, 50, 30);
	if (health > 20){
		GetEngine()->DrawScreenString(450, 10, buf, 0xffffff, NULL);
	}
	else{
		//Change colour to red if health less than 20 and animate text
		//Set flag to shirink the font
		if (fontSize >= 35){
			fontGrowing = false;
		}
		//Set flat to graw font
		else if (fontSize <= 20){
			fontGrowing = true;

		}
		//Grow or shrink font
		if (fontGrowing == true)
			fontSize += 0.3;
		else if (fontGrowing == false)
			fontSize -= 0.3;

		//Get font with size
		Font* font = GetEngine()->GetFont("Cornerstone Regular.ttf", floor(fontSize));
		GetEngine()->DrawScreenString(450, 10, buf, 0xff0000, font);
	}

	GetEngine()->Redraw(false);
	
	//Draw an image loaded from a file.
	ImageData im;
	//Load the player image file into an image object - at the normal size
	im.LoadImage("player.png");

	im.RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight);

	StoreLastScreenPositionForUndraw();

}


void TerrainiaPlayerObject::DoUpdate(int iCurrentTime)
{
	m_gravDir = 5;
	m_CollisionUp = false;
	m_CollisionDown = false;
	m_CollisionLeft = false;
	m_CollisionRight = false;


	// Iterate through the objects
	// We are looking for one which is too close to us
	DisplayableObject* pObject;
	for (int iObjectId = 0;
		(pObject = m_pMainEngine->GetDisplayableObject(iObjectId)
		) != NULL;
	iObjectId++)
	{
		if (pObject == this) // This is us, skip it
			continue;
		else if (pObject->GetHeight() == 99){ //Mob object
			mob = pObject;
		}

		//Object features
		int pObjectXCentre = pObject->GetXCentre();
		int pObjectYCentre = pObject->GetYCentre();
		int pObjectWidth = pObject->GetWidth();
		int pObjectHeight = pObject->GetHeight();

		//Collision left
		if (m_iCurrentScreenX <= pObjectXCentre + pObjectWidth / 2 && m_iCurrentScreenX > pObjectXCentre && m_iCurrentScreenY + m_iDrawHeight > pObjectYCentre - pObjectHeight / 2){
			if (pObject == mob){
				setHealth(0.3);
			}
			m_CollisionLeft = true;
		}
		//Collision right
		if (m_iCurrentScreenX + m_iDrawWidth >= pObjectXCentre - pObjectWidth / 2 && m_iCurrentScreenX + m_iDrawWidth < pObjectXCentre && m_iCurrentScreenY + m_iDrawHeight > pObjectYCentre - pObjectHeight / 2){
			if (pObject == mob){
				setHealth(0.3);
			}
			m_CollisionRight = true;
		}
		
		//Collision detection down
		if (m_iCurrentScreenY + m_iDrawHeight >= pObjectYCentre - pObjectHeight / 2 && pObjectXCentre + pObjectWidth / 2 > m_iCurrentScreenX && pObjectXCentre - pObjectWidth / 2 < m_iCurrentScreenX + m_iDrawWidth){
			if (pObjectHeight != 4)
				m_iCurrentScreenY = pObjectYCentre - pObjectHeight / 2 - m_iDrawHeight;
			m_CollisionDown = true;
			m_gravDir = 0;
			remainingJump = 30;
			maxJumpHeight = m_iCurrentScreenY - 150;
		}
			
	}
	
	//Let gravity take effect
	m_iCurrentScreenY += m_gravDir;
	
	//Handle jumping
	if ((GetEngine()->IsKeyPressed(SDLK_UP) || GetEngine()->IsKeyPressed(SDLK_w)) && !m_CollisionUp){
		m_iCurrentScreenY -= remainingJump;
		remainingJump -= 2;
		m_gravDir = 0;

		if (remainingJump < 0 || m_iCurrentScreenY <= maxJumpHeight){
			remainingJump = 0;
			m_gravDir = 5;
		}
		
		
	}
	
	//Mouse movement
	if (m_pMainEngine->GetMouseToggle()){
		if (GetEngine()->GetCurrentMouseX() < m_iCurrentScreenX && !m_CollisionLeft)
			m_iCurrentScreenX -= 5;
		if (GetEngine()->GetCurrentMouseX() > m_iCurrentScreenX + m_iDrawWidth && !m_CollisionRight)
			m_iCurrentScreenX += 5;
	}
	else{ //Key movement
		if ((GetEngine()->IsKeyPressed(SDLK_LEFT) || GetEngine()->IsKeyPressed(SDLK_a)) && !m_CollisionLeft)
			m_iCurrentScreenX -= 5;
		if ((GetEngine()->IsKeyPressed(SDLK_RIGHT) || GetEngine()->IsKeyPressed(SDLK_d)) && !m_CollisionRight)
			m_iCurrentScreenX += 5;
	}
	
	if ((GetEngine()->IsKeyPressed(SDLK_DOWN) || GetEngine()->IsKeyPressed(SDLK_s)) && !m_CollisionDown)
		m_iCurrentScreenY += 5;

	
	//Stops player going out of the screen	
	if (m_iCurrentScreenX < 0)
		m_iCurrentScreenX = 0;
	if (m_iCurrentScreenX >= GetEngine()->GetScreenWidth() - m_iDrawWidth)
		m_iCurrentScreenX = GetEngine()->GetScreenWidth() - m_iDrawWidth;
	if (m_iCurrentScreenY < m_iDrawHeight)
		m_iCurrentScreenY = m_iDrawHeight;
	if (m_iCurrentScreenY >= GetEngine()->GetScreenHeight())
		m_iCurrentScreenY = GetEngine()->GetScreenHeight();

	
	//Check health
	checkHealth();

	// Ensure that the object gets redrawn on the display, if something changed
	RedrawObjects();
}


void TerrainiaPlayerObject::setHealth(double damage)
{
	//Set health of player and make sure it doesnt go less than 0
	health -= damage;

	if (health < 0)
		health = 0;
}


void TerrainiaPlayerObject::checkHealth()
{
	//If dead change state to death screen and display score strings
	if (health == 0){

		//Change to death screen
		deathScreen ds;

		ds.SetSize(30, 16);

		for (int x = 0; x < 30; x++)
			for (int y = 0; y < 16; y++)
				ds.SetValue(x, y, 2);

		ds.SetBaseTilesPositionOnScreen(0, 0);
		ds.DrawAllTiles(GetEngine(), GetEngine()->GetBackground(), 0, 0, 30, 16);

		//Set player and mobs invisible
		SetVisible(false);
		mob->SetVisible(false);
		
		int highScore = 0;
		int score = 0;
		string temp = "";

		//Read score
		ifstream scoreFile("score.txt");
		if (scoreFile.is_open())
		{
			while (getline(scoreFile, temp))
			{
				score = stoi(temp);
			}
			scoreFile.close();

		}else 
			cout << "Unable to open file";
		
		//Read highscore
		ifstream highScoreFile("highScore.txt");
		if (highScoreFile.is_open())
		{
			while (getline(highScoreFile, temp))
			{
				highScore = stoi(temp);
			}
			highScoreFile.close();

		}
		else
			cout << "Unable to open file";

		//If score better than highscore change highscore
		if (score > highScore){
			
			ofstream myfile;
			myfile.open("highScore.txt");
			myfile << score;
			myfile.close();
		}

		//Display death strings
		m_pMainEngine->drawDeathString(score, highScore);

		RedrawObjects();
	}
}


bool TerrainiaPlayerObject::GetToggle()
{
	//Mouse input
	return mouseToggle;
}
