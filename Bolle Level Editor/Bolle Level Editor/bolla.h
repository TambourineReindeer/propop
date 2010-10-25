#pragma once

int const TYPES_MAX = 8;

class bolla
{
	int x;
	int y;
	int type;

public:
	bolla(void);
	~bolla(void);

	int getX();
	void putX(int newX);
	int getY();
	void putY(int newY);
	void resetType();
	void nextType();
	void prevType();
	int getType();
};

