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

	int getx();
	void putx(int newx);
	int gety();
	void puty(int newy);
	void resettype();
	void nexttype();
	void prevtype();
	int gettype();
};

