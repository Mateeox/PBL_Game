#pragma once

class MapKey {
	public:
		enum MapType {
			Null,
			Floor,
			Wall,
			Blocked, //this is for floors with at least one door
			Box,
			Slower
		};
		int x;
		int y;
		MapKey();
		MapKey(int x, int y);
};

bool operator<(const MapKey &c1, const MapKey &c2);