#include "StdAfx.h"
#include "BasePath.h"

class Node
{
public:
	Node::Node(){
		place = CPlace(-1,-1);
		parent = CPlace(-1,-1);
		g = -1;
		h = -1;
	};

	Node::Node(const Node& node){
		place = node.place;
		parent = node.parent;
		g = node.g;
		h = node.h;
	}

	Node::Node(CPlace pl){
		place = pl;
		g = 0;
		h = 0;
	}

	bool Node::operator< (const Node node) const
	{
		// ok, implemento l'operatore al contrario im modo da avere 
		// il nodo meno costoso in testa alla queue
		return (g + h) > (node.g + node.h);
	}

	bool Node::operator!= (const Node node)
	{
		return place != node.place;
	}

	CPlace place;
	CPlace parent;
	float  g;
	float  h;
};




class stringhasher : public hash_compare <string>
{
public:
	size_t stringhasher::operator() (const string& s) const
	{
		size_t h = 0;
		string::const_iterator p, p_end;
		for(p = s.begin(), p_end = s.end(); p != p_end; ++p)
		{
			h = 31 * h + (*p);
		}
		return h;
	}

	bool stringhasher::operator() (const string& s1, const string& s2) const
	{
		return s1 < s2;
	}
};
typedef hash_map<string, Node, stringhasher> MY_HASH_MAP;


CBasePath::CBasePath(void)
{
}

CBasePath::~CBasePath(void)
{
}


//OPEN = priority queue containing START
//CLOSED = empty set
//while lowest rank in OPEN is not the GOAL:
//  current = remove lowest rank item from OPEN
//  add current to CLOSED
//  for neighbors of current:
//    cost = g(current) + movementcost(current, neighbor)
//    if neighbor in OPEN and cost less than g(neighbor):
//      remove neighbor from OPEN, because new path is better
//    if neighbor in CLOSED and cost less than g(neighbor): **
//      remove neighbor from CLOSED
//    if neighbor not in OPEN and neighbor not in CLOSED:
//      set g(neighbor) to cost
//      add neighbor to OPEN
//      set priority queue rank to g(neighbor) + h(neighbor)
//      set neighbor's parent to current
//
//reconstruct reverse path from goal to start
//by following parent pointers
bool CBasePath::BuildPath(CWorld *World, CPlace Start, CPlace End)
{
	priority_queue<Node> pqOpen;  // priority queue per trovare il nodo open meno costoso (lowest rank)
	MY_HASH_MAP hmOpen;           // questa hash map mi serve per recuperare rapidamente i nodi in open.
	MY_HASH_MAP hmClose;          // close è certamente una hask map in quanto devo solo verificare se il nodo è già presente.
	Node        current;          // nodo corrente.
	Node        endNode(End);     // nodo destinazione.
	float       fCost;

	hmOpen[BuildHashMapKey(Start)] = Node(Start);
	pqOpen.push(Node(Start));
	while (pqOpen.top() != endNode)
	{
		current = pqOpen.top();
		string currentkey = BuildHashMapKey(current.place);
		pqOpen.pop();
		hmOpen.erase(currentkey);
		hmClose[currentkey] = current;

		vector<CPlace> neighbors = World->GetPlaceNeighbors(current.place);
		if (!neighbors.empty())
		{
			vector<CPlace>::iterator it = neighbors.begin();
			for (it; it<neighbors.end(); it++)
			{
				fCost = current.g + MovementCost(current.place, *it);
				
				string NeighbourKey = BuildHashMapKey(*it);
				bool bInOpen = false, bInClose = false;
				Node NodeInOpen = hmOpen[NeighbourKey];
				if (NodeInOpen.g != -1) // esiste già il nodo corrispondente in open
				{
					if (fCost < NodeInOpen.g)
					{
						hmOpen.erase(NeighbourKey);
					}
					bInOpen = true;
				} else {
					// l'accesso ad una chiave non esistente nell'hashmap, implicitamente
					// crea l'elemento: qui elimino l'elemento indesiderato.
					// Nel caso di Open l'elemento potrebbe essere lasciato dov'è in quanto
					// all'occorrenza verrebbe sovrascritto.
					hmOpen.erase(NeighbourKey);
				}

				Node NodeInClose = hmClose[NeighbourKey];
				if (NodeInClose.g != -1) // esiste già il nodo corrispondente in close
				{
					if (fCost < NodeInClose.g)
					{
						hmClose.erase(NeighbourKey);
					}
					bInClose = true;
				} else {
					// l'accesso ad una chiave non esistente nell'hashmap, implicitamente
					// crea l'elemento: qui elimino l'elemento indesiderato.
					hmClose.erase(NeighbourKey);
				}

				if (!bInOpen && !bInClose)
				{
					Node newNode(*it);
					if (*it != End) {
						newNode.g = fCost;
						newNode.h = Heuristic(*it,Start,End);
					} else
					{
						newNode.g = 0;
						newNode.h = 0;
					}
					newNode.parent = current.place;
					hmOpen[BuildHashMapKey(newNode.place)] = newNode;
					pqOpen.push(newNode);
				}
			}
		}
	}

	Node node = hmClose[BuildHashMapKey(current.place)];
	while (node.place!=Start)
	{
		Path.push_front(node.place);
		node = hmClose[BuildHashMapKey(node.parent)];
	}
	return true;
}

CPlace* CBasePath::GetNextPlace()
{
	if (Path.empty()) return NULL;
	CPlace *p = &Path.front();
	rotate(Path.begin(), Path.begin()+1, Path.end());
	return p;
}

string CBasePath::BuildHashMapKey(CPlace p)
{
	string sKey;
	char szX[10], szY[10];
	sprintf_s(szX,10,"%03d",(int)p.m_fPosX);
	sprintf_s(szY,10,"%03d",(int)p.m_fPosY);
	sKey.append(szX);
	sKey.append(";");
	sKey.append(szY);

	return sKey;
}

float CBasePath::MovementCost(CPlace p1, CPlace p2)
{
	// se la differenza di altitudine è inferiore, il movimento è "meno dispendioso" 
	// però considero la differenza di altitudine meno importante (/100) dell'eu-
	// ristica.
	return abs(p2.GetAltitude()-p1.GetAltitude()) / 100;
	//return 1;
}

float CBasePath::Heuristic(CPlace n, CPlace start, CPlace goal)
{
	// Manhattan distance
	float distance = abs(n.m_fPosX-goal.m_fPosX)+abs(n.m_fPosY-goal.m_fPosY);
	
	// break ties
	float dx1 = n.m_fPosX-goal.m_fPosX;
	float dy1 = n.m_fPosY-goal.m_fPosY;
	float dx2 = start.m_fPosX-goal.m_fPosX;
	float dy2 = start.m_fPosY-goal.m_fPosY;
	float cross = abs(dx1*dy2-dx2*dy1);
	return distance + cross * .001f;
}