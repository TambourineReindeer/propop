#include "StdAfx.h"
#include "Place.h"

CPlace::CPlace(void)
{
	m_fPosX = 0;
	m_fPosY = 0;
	m_szName = "place";
	m_iType = PTNone;
	m_fAltitude = 0;
}

CPlace::CPlace(float x, float y)
{
	m_fPosX = x;
	m_fPosX = y;
	m_iType = CPlace::PTFloor;
}

CPlace::CPlace(float x, float y, PlaceType type)
{
	m_fPosX = x;
	m_fPosX = y;
	m_iType = type;
}

CPlace::~CPlace(void)
{

}

string CPlace::GetName()
{
	return m_szName;
}

float CPlace::GetAltitude()
{
	return m_fAltitude;
}

bool CPlace::operator!= (const CPlace a)
{
	return ((m_fPosX != a.m_fPosX) || (m_fPosY != a.m_fPosY) ||
			(m_fAltitude != a.m_fAltitude));
}

bool CPlace::operator== (const CPlace a)
{
	return ((m_fPosX == a.m_fPosX) && (m_fPosY == a.m_fPosY) &&
			(m_fAltitude == a.m_fAltitude));
}

CPlace::PlaceType CPlace::GetType()
{
	return m_iType;
}

void CPlace::SetAltitude(float altitude)
{
	m_fAltitude = altitude;
	if (altitude == 0)           m_iType = CPlace::PTFloor;
	if (altitude < 0)            m_iType = CPlace::PTWater;
	if (altitude > 0)            m_iType = CPlace::PTMountain;
	if (altitude == MAXALTITUDE) m_iType = CPlace::PTWall;
}

void CPlace::SetName(char *name)
{
	try
	{
		m_szName = name;
	} 
	catch ( ... )
	{
		m_szName = "";
	}
}

void CPlace::SetName(string name)
{
	try
	{
		m_szName = name;
	} 
	catch ( ... )
	{
		m_szName = "";
	}
}

void CPlace::SetType(CPlace::PlaceType type)
{
	m_iType = type;
	if (type == CPlace::PTFloor)    m_fAltitude = 0;
	if (type == CPlace::PTWall)     m_fAltitude = MAXALTITUDE;
	if (type == CPlace::PTMountain) m_fAltitude =  1.0f;
	if (type == CPlace::PTWater)    m_fAltitude = -1.0f;
}

bool CPlace::Save(ofstream *ofs)
{
	if (ofs != NULL)
	{
		ofs->write((char*)&m_fPosX,sizeof(float));
		ofs->write((char*)&m_fPosY,sizeof(float));
		ofs->write((char*)&m_fAltitude,sizeof(float));
		ofs->write((char*)&m_iType,sizeof(PlaceType));
		int iNameSize = (int)m_szName.size();
		ofs->write((char*)&iNameSize,sizeof(int));
		ofs->write((char*)m_szName.data(),iNameSize);
		return true;
	}
	return true;
}

bool CPlace::Load(ifstream *ifs)
{
	if (ifs != NULL)
	{
		ifs->read((char*)&m_fPosX,sizeof(float));
		ifs->read((char*)&m_fPosY,sizeof(float));
		ifs->read((char*)&m_fAltitude,sizeof(float));
		ifs->read((char*)&m_iType,sizeof(PlaceType));
		int iNameSize;
		ifs->read((char*)&iNameSize,sizeof(int));
		char name[50] = "";
		ifs->read(name,iNameSize);
		m_szName = name;
		return true;
	}
	return true;
}