#include "MapGenerator/MapKey.hpp"

MapKey::MapKey()
{
	this->x = 0;
	this->y = 0;
}

MapKey::MapKey(int x, int y)
{
	this->x = x;
	this->y = y;
}


bool operator<(const MapKey &c1, const MapKey &c2)
{
	return (c1.x < c2.y && c1.y < c2.y) || (c1.x > c2.x && c1.y < c2.y);
}