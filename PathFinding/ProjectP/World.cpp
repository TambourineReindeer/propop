#include "StdAfx.h"
#include "World.h"

CWorld::CWorld(void)
{
	m_iNumOfPlaces = 0;
}

CWorld::CWorld(int iNumOfPlaces)
{
	theWorld.resize(iNumOfPlaces);
	m_iNumOfPlaces = iNumOfPlaces;
}


CWorld::~CWorld(void)
{
}


bool CWorld::AddPlace(CPlace place)
{
	try
	{
		theWorld.push_back(place);
		m_iNumOfPlaces++;
		return true;

	} catch ( ... ) {
		return false;
	}
}

CPlace* CWorld::GetPlaceAtPos(float x, float y)
{
	for (int i=0; i<(int)theWorld.size(); i++) {
		if ((theWorld[i].m_fPosX == x) && (theWorld[i].m_fPosY == y))
			return &theWorld[i];
	}

	return NULL;
}

bool CWorld::DeletePlaceAtPos(float x, float y)
{
	for (int i=0; i<(int)theWorld.size(); i++) {
		if ((theWorld[i].m_fPosX == x) && (theWorld[i].m_fPosY == y))
		{
			theWorld.erase(theWorld.begin() + i);
			return true;
		}
	}

	return false;
}

bool CWorld::IsAccessiblePlace(float x, float y, CCharacter chr)
{
	for (int i=0; i<(int)theWorld.size(); i++) {
		if ((theWorld[i].m_fPosX == x) && (theWorld[i].m_fPosY == y))
		{
			CPlace::PlaceType placetype = theWorld[i].GetType();
			if ((placetype == CPlace::PTNone) ||
				(placetype == CPlace::PTWall) ||
				((placetype == CPlace::PTMountain) && (theWorld[i].GetAltitude() > chr.m_fMaxAltitude)) ||
				((placetype == CPlace::PTWater)    && (theWorld[i].GetAltitude() < chr.m_fMinAltitude))) 
			{
				return false;
			} else {
				return true;
			}

		}
	}

	return false;
}

bool CWorld::Save(string filename)
{
	ofstream* pmyFile = new ofstream; // On the heap
	pmyFile->open(filename.c_str(), ios_base::binary | ios_base::trunc);
	pmyFile->write((char*)"WV1", 3); // World Version 1
	pmyFile->write((char*)&m_iNumOfPlaces, sizeof(int));
	for (int i=0; i<(int)theWorld.size(); i++) {
		theWorld[i].Save( pmyFile );
	}
	pmyFile->close();
	delete pmyFile;
	return true;
}

bool CWorld::Load(string filename)
{
	ifstream* pmyFile = new ifstream; // On the heap
	pmyFile->open(filename.c_str(), ios_base::binary);
	char szMagic[4] = "";

	pmyFile->read(szMagic, 3);
	if (strcmp(szMagic, "WV1")==0)
	{
		int iNumOfPlaces;
		pmyFile->read((char*)&iNumOfPlaces, sizeof(int));
		for (int i=0; i<iNumOfPlaces; i++) {
			CPlace pl;
			pl.Load(pmyFile);
			this->AddPlace(pl);
		}
	}
	pmyFile->close();
	return true;
}


//
//  FUNZIONE: BuildPathForCharacter(CCharacter *character, CPlace *to)
//
//  SCOPO:    Restituisce il percorso (un array di CPlace) più breve dal CPlace attuale di
//            <character> a <to> tenendo conto delle caratteristiche di <character>.
//
//  COMMENTI: Implementa l'algoritmo A*
//            Rif.: http://www.policyalmanac.org/games/aStarTutorial.htm
//
//
CPlace* CWorld::BuildPathForCharacter(CCharacter *character, CPlace *to)
{
	return NULL;
}

vector<CPlace> CWorld::GetPlaceNeighbors(CPlace pl)
{
	vector<CPlace> vec;
	CPlace *p = GetPlaceAtPos(pl.m_fPosX - 1, pl.m_fPosY - 1);
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX    , pl.m_fPosY - 1);
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX + 1, pl.m_fPosY - 1);
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX - 1, pl.m_fPosY    );
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX + 1, pl.m_fPosY    );
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX - 1, pl.m_fPosY + 1);
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX    , pl.m_fPosY + 1);
	if (p != NULL) vec.push_back(*p);
	p = GetPlaceAtPos(pl.m_fPosX + 1, pl.m_fPosY + 1);
	if (p != NULL) vec.push_back(*p);

	return vec;
}