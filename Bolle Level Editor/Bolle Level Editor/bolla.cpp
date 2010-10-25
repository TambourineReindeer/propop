#include "StdAfx.h"
#include "Bolla.h"

Bolla::Bolla(void)
{
}


Bolla::~Bolla(void)
{
}

int Bolla::getX() {
	return this->x;
}

void Bolla::putX(int newX) {
	this->x = newX;
}

int Bolla::getY() {
	return this->y;
}

void Bolla::putY(int newY) {
	this->y = newY;
}

void Bolla::resetType() {
	this->type = 0;
}

void Bolla::nextType() {
	this->type++;
	if (this->type >= TYPES_MAX) {
		this->resetType();
	} 
}

void Bolla::prevType() {
	this->type--;
	if (this->type < 0) {
		this->type = TYPES_MAX;
	}
}

int Bolla::getType() {
	return this->type;
}
