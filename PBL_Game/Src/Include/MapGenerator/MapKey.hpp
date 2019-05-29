#pragma once

struct MapKey {
	int x;
	int y;

	inline bool operator<(const MapKey& a) const {
		return (x < a.x && y < a.y) || (x < a.x && y > a.y);
	}
};

enum MapType {
	Null,
	Floor,
	Wall,
	Blocked, //this is for floors with at least one door
	Box,
	Slower
};