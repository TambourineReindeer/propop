#include "StdAfx.h"
#include "bolla.h"

bolla::bolla(void)
{
}


bolla::~bolla(void)
{
}

int bolla::getX() {
	return x;
}

void bolla::putX(int newX) {
	x = newX;
}

int bolla::getY() {
	return y;
}

void bolla::putY(int newY) {
	y = newY;
}

void bolla::resetType() {
	type = 0;
}

void bolla::nextType() {
	type++;
	if (type >= TYPES_MAX) {
		resetType();
	} 
}

void bolla::prevType() {
	type--;
	if (type < 0) {
		type = TYPES_MAX;
	}
}

int bolla::getType() {
	return type;
}
