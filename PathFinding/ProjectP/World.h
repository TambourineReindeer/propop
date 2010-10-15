#pragma once

#include <vector>
#include <functional>
#include <fstream>
#include "Place.h"
#include "Character.h"

using namespace std;

class CWorld
{
public:
	CWorld(void);
	CWorld(int);
	~CWorld(void);

	CCharacter *Population;

	bool          AddCharacter(CCharacter*);
	bool          AddPlace(CPlace);
	bool          IsAccessiblePlace(float x, float y, CCharacter);
	CPlace*       GetPlaceAtPos(float x, float y);
	vector<CPlace> GetPlaceNeighbors(CPlace);
	CCharacter*   GetCharacterAtPos(float x, float y);
	bool          DeletePlaceAtPos(float x, float y);

	bool          Load(string filename);
	bool          Save(string filename);

	CPlace*       BuildPathForCharacter(CCharacter *character, CPlace *to);

private:
	vector<CPlace>   theWorld;
	int              m_iNumOfPlaces;
};
