#include "MapGenerator/MapKey.hpp"

MapKey::MapKey(int x, int y)
{
	this->x = x;
	this->y = y;
	this->type = MapType::Floor;
}

MapKey::MapKey(int x, int y, MapType type)
{
	this->x = x;
	this->y = y;
	this->type = type;
}