#include "StdAfx.h"
#include "Bubble.h"

Bubble::Bubble(void)
{
}


Bubble::~Bubble(void)
{
}

int Bubble::getX() {
	return this->x;
}

void Bubble::putX(int newX) {
	this->x = newX;
}

int Bubble::getY() {
	return this->y;
}

void Bubble::putY(int newY) {
	this->y = newY;
}

void Bubble::resetType() {
	this->type = 0;
}

void Bubble::nextType() {
	this->type++;
	if (this->type >= TYPES_MAX) {
		this->resetType();
	} 
}

void Bubble::prevType() {
	this->type--;
	if (this->type < 0) {
		this->type = TYPES_MAX;
	}
}

int Bubble::getType() {
	return this->type;
}
