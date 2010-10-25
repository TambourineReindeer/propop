#pragma once

int const TYPES_MAX = 8;

class Bolla
{
	int x;
	int y;
	int type;

public:
	Bolla(void);
	~Bolla(void);

	int getX();
	void putX(int newX);
	int getY();
	void putY(int newY);
	void resetType();
	void nextType();
	void prevType();
	int getType();
};

