#pragma once

int const TYPES_MAX = 8;

enum Types {

};


class Bubble
{
	int x;
	int y;
	int type;

public:
	Bubble(void);
	~Bubble(void);

	int  getX();
	void putX(int newX);
	int  getY();
	void putY(int newY);
	void resetType();
	void nextType();
	void prevType();
	int  getType();
};

