#pragma once

#define MAXALTITUDE 99999

#include <string>
#include <exception>
#include <fstream>

using namespace std;

class CPlace
{
public:
	/* Probabilmente Floor, Water e Mountain sono lo stesso tipo di Place 
	   ma con altitudine diversa... però forse così è più comodo(!?)      */
	enum PlaceType {
		PTNone     = 0,
		PTFloor    = 1,
		PTWall     = 2,
		PTWater    = 3,
		PTMountain = 4
	};

	CPlace(void);
	CPlace(float x, float y);
	CPlace(float x, float y, PlaceType type);
	~CPlace(void);
	bool operator!= (const CPlace);
	bool operator== (const CPlace);

	float     m_fPosX;
	float     m_fPosY;

	string    GetName();
	PlaceType GetType();
	float     GetAltitude();

	void  SetName(char* name);
	void  SetName(string);
	void  SetAltitude(float);
	void  SetType(PlaceType);

	bool  Save(ofstream *ofs);
	bool  Load(ifstream *ifs);

private:
	PlaceType m_iType;
	float     m_fAltitude;
	string    m_szName;
};
