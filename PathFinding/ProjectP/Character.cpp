#include "StdAfx.h"
#include "Character.h"

CCharacter::CCharacter(void)
{
	m_fPosX = 0;
	m_fPosY = 0;
	m_szName = "character";
}

CCharacter::~CCharacter(void)
{

}

string CCharacter::GetName()
{
	return m_szName;
}

void CCharacter::SetName(char *name)
{
	m_szName = name;
}

void CCharacter::SetName(string name)
{
	m_szName = name;
}

