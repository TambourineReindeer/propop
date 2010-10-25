#include "StdAfx.h"
#include "bolla.h"

bolla::bolla(void)
{
}


bolla::~bolla(void)
{
}

int bolla::getx() {
	return x;
}

void bolla::putx(int newx) {
	x = newx;
}

int bolla::gety() {
	return y;
}

void bolla::puty(int newy) {
	y = newy;
}

void bolla::resettype() {
	type = 0;
}

void bolla::nexttype() {
	type++;
	if (type >= TYPES_MAX) {
		resettype();
	} 
}

void bolla::prevtype() {
	type--;
	if (type < 0) {
		type = TYPES_MAX;
	}
}

int bolla::gettype() {
	return type;
}
