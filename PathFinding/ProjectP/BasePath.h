#pragma once

#include <hash_map>
#include <queue>
#include <math.h>
#include "World.h"

using namespace std;
using namespace stdext;

class CBasePath
{
public:
	CBasePath(void);
	~CBasePath(void);

	CPlace* GetNextPlace();
	bool    BuildPath(CWorld *, CPlace , CPlace );

private:
	deque<CPlace> Path;
	string BuildHashMapKey(CPlace p);
	float MovementCost(CPlace, CPlace);
	float Heuristic(CPlace, CPlace, CPlace);
};
