#pragma once

#include "Place.h"
#include <string>

using namespace std;

class CCharacter
{
public:
	CCharacter(void);
	~CCharacter(void);

	float     m_fPosX;
	float     m_fPosY;
	float     m_fMaxAltitude;
	float     m_fMinAltitude;
	CPlace	  **Path;    // Un puntatore a puntatori: gli oggetti puntati sono quelli nel World
	string    GetName();
	

	void  SetName(char* name);
	void  SetName(string name);

private:
	string    m_szName;

};
