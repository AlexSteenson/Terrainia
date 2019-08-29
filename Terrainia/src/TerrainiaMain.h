#pragma once
#include "BaseEngine.h"
#include "TerrainiaTileManager.h"


class TerrainiaMain :
	public BaseEngine
{
public:
	TerrainiaMain();
	~TerrainiaMain();
	void SetupBackgroundBuffer();
	int InitialiseObjects();
	void DrawStrings();
	void KeyDown(int iKeyCode);
	// State number
	enum State { stateInit, stateMain };
	State m_state;
	void drawDeathString(int score, int highScore);
	bool GetMouseToggle(){ return mouseToggle; }

private:
	bool loaded = false;
	int *p;
	//TerrainiaTileManager m_oTiles;
	playScreen pls;
	startScreen ss;
	int* createMapArray();
	int mouseToggle = true;
	
};
