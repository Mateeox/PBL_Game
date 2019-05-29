#pragma once
enum MapType {
	Null,
	Floor,
	Wall,
	Blocked, //this is for floors with at least one door
	Box,
	Slower
};

class MapKey {
	public:
		int x;
		int y;
		MapType type;
		MapKey(int x, int y);
		MapKey(int x, int y, MapType type);
};