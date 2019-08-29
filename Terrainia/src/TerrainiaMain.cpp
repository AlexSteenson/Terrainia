#include "header.h"
#include "TerrainiaMain.h"
#include "TerrainiaDirtObject.h"
#include "TerrainiaPlayerObject.h"
#include "TerrainiaMobObject.h"
#include "TerrainiaBulletObject.h"
#include "TerrainiaTileManager.h"
#include "TerrainiaMobBig.h"
#include "TerrainiaMobGreen.h"
#include "TerrainiaMobSlow.h"
#include "JPGImage.h"

#include <math.h>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


TerrainiaMain::TerrainiaMain()
	: BaseEngine(34)
	, m_state(stateInit)
{
	
}


TerrainiaMain::~TerrainiaMain()
{
}

void TerrainiaMain::SetupBackgroundBuffer()
{
	playScreen pls;
	int height = 0;
	ImageData im;
	ofstream myfile;

	switch (m_state){
	case stateInit:
		//Draw the intial state from tilemanager
		ss.SetSize(1, 116);

		for (int x = 0; x < 1; x++)
			for (int y = 0; y < 1; y++)
				ss.SetValue(x, y, 2);

		ss.SetBaseTilesPositionOnScreen(0, 0);
		ss.DrawAllTiles(this, this->GetBackground(), 0, 0, 1, 1);
		
		break;
	case stateMain:	
		
		// Load the image file into an image object - at the normal size
		im.LoadImage("background.png");

		im.RenderImageWithMask(this->GetBackground(),
			0, 0, 0, 0,
			im.GetWidth(), im.GetHeight());

		pls.SetSize(30, 16);
		
		//Set all tiles to value 0
		for (int x = 0; x < 30; x++)
			for (int y = 0; y < 16; y++)
				pls.SetValue(x, y, 0);
		
		//Set the values of the tiles bellow the top layer of the terrain to 1
		for (int x = 0; x < 30; x++){
			height = p[x];
			for (int y = 0; y < height; y++)
				pls.SetValue(x, 15 - y, 1);
		}

		//Draw the play state
		pls.SetBaseTilesPositionOnScreen(0, 0);

		pls.DrawAllTiles(this, this->GetBackground(), 0, 0, 30, 16);

		//Reset score file
		myfile.open("score.txt");
		myfile << 0;
		myfile.close();
		
		break;
	}
	
}


int TerrainiaMain::InitialiseObjects()
{
	if (m_state != stateInit){
		DrawableObjectsChanged();
		DestroyOldObjects();

		CreateObjectArray(34);

		srand(time(NULL));

		//If the maps not loaded, create a random one
		if (!loaded)
			p = createMapArray();

		int height = 0;
		int count = 0;
		for (int x = 0; x < 30; x++){
			//Create an object for the top level of the terrain
			StoreObjectInArray(count, new TerrainiaDirtObject(this, x * 50, 800 - p[x] * 50));
			count++;

		}

		//Store in array the mobs, playeer and bullet
		StoreObjectInArray(count++, new TerrainiaMobGreen(this, 900, 250, 2));
		StoreObjectInArray(count++, new TerrainiaMobBig(this, 1000, 350, 1));
		StoreObjectInArray(count++, new TerrainiaBulletObject(this, 1510, 200, 7));
		//StoreObjectInArray(count++, new TerrainiaPlayerObject(this, 550, 200));
		StoreObjectInArray(39, NULL);
		//An example of adding an object in without having to add NULL after, can do this anywhere
		AddObjectToArray(new TerrainiaMobSlow(this, 200, 200, 3));
		AddObjectToArray(new TerrainiaPlayerObject(this, 550, 200));
		
	}

	return 0;
}

/* Draw text labels */
void TerrainiaMain::DrawStrings()
{
	
	ifstream highScoreFile("highScore.txt");
	string temp = "";
	char buf[128];
	
	// NEW SWITCH
	switch (m_state)
	{
	case stateInit:
		
		//Draw the strings for the initial state
		CopyBackgroundPixels(0/*X*/, 280/*Y*/, GetScreenWidth(), 40/*Height*/);

		DrawScreenString(550, 200, "Press SPACE for a new level", 0xffffff, NULL);
		DrawScreenString(550, 250, "or L to load one", 0xffffff, NULL);

		//Opand and draw the highscore string
		if (highScoreFile.is_open())
		{
			while (getline(highScoreFile, temp))
			{
				sprintf(buf, "High Score = %d", stoi(temp));
				DrawScreenString(550, 400, buf, 0xffffff, NULL);
			}
			highScoreFile.close();

		}
		else
			cout << "Unable to open file";

		break;
	case stateMain:

		//Draw main game strings 
		CopyBackgroundPixels(0/*X*/, 0/*Y*/, GetScreenWidth(), 30/*Height*/);
		DrawBackgroundString(250, 10, "Running", 0xffffff, NULL);
		CopyBackgroundPixels(1200, 0, 100, 30);
		if (mouseToggle == 1) //Draw what input control is active
			DrawScreenString(1200, 10, "Mouse control", 0xffffff, NULL);
		else
			DrawScreenString(1200, 10, "Key control", 0xffffff, NULL);

		break;
	}
}


/*
Handle any key presses here.
*/
void TerrainiaMain::KeyDown(int iKeyCode)
{
	
	// NEW SWITCH on current state
	switch (iKeyCode)
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		SetExitWithCode(0);
		break;
	case SDLK_t: //Change player input
		mouseToggle = !mouseToggle;
		break;
	case SDLK_SPACE: // SPACE Pauses
		if (m_state == stateInit){
			// Go to state main
			m_state = stateMain; //Stack stuff
			InitialiseObjects();

			SetupBackgroundBuffer();
			Redraw(true);
		} 
		break; // End of case SPACE
	case SDLK_l:
		if (m_state == stateInit){
			ifstream map("map.txt");
			string temp = "";
			int i = 0;
			int mapp[30];

			//Read map from file into an array
			if (map.is_open())
			{
				while (getline(map, temp))
				{
					mapp[i] = stoi(temp);
					i++;
				}
				map.close();
				loaded = true;
			}
			else
				cout << "Unable to open file";

			//Set p to the map array
			p = mapp;
			// Go to state main
			m_state = stateMain;
			//Initialise objects to new map specs
			InitialiseObjects();
			// Force redraw of background
			SetupBackgroundBuffer();
		
		}
	}
}


int* TerrainiaMain::createMapArray()
{
	int const LINELENGTH = 30;
	static int points[LINELENGTH];

	//Random rnumber for the hight of the terrain at block i
	for (int i = 0; i < LINELENGTH; i++){
		int random = rand() % 6;
		//If random is less than 3 store the value as another random number between 0 and 2 + 2
		//else keep the value
		(random < 3) ? points[i] = rand() % 2 + 2 : points[i] = random;

	}

	for (int i = 0; i < LINELENGTH; i++){
		//if the previous tile is more than 2 higher then add 2 to current tile
		if (points[i] + 2 < points[i - 1] && i > 0){
			points[i] += 2;
		}
		//if the next tile is more than 2 higher then add 2 to current tile
		if (points[i] + 2 < points[i + 1] && i < LINELENGTH){
			points[i] += 2;
		}
		//If the previous tile and the next tile are 2 or more higher then add 1
		if (points[i - 1] + 2 <= points[i] && points[i + 1] + 2 <= points[i]){
			points[i - 1] += 1;
			points[i + 1] += 1;
		}

		
	}

	return points;
}


void TerrainiaMain::drawDeathString(int score, int highScore)
{
	//Draw death srings
	char buf[128];
	CopyBackgroundPixels(0, 0, GetScreenWidth(), GetScreenHeight());
	DrawBackgroundString(650, 250, "You died!", 0xffffff, NULL);

	//Draw score
	sprintf(buf, "Score = %d", score);
	DrawBackgroundString(650, 300, buf, 0xffffff, NULL);

	//Draw highscore
	sprintf(buf, "High Score = %d", highScore);
	DrawBackgroundString(650, 350, buf, 0xffffff, NULL);

	Redraw(false);
}
