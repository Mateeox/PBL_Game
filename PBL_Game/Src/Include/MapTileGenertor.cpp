#include "MapTileGenertor.hpp"

std::vector<MapTile *> MapTileGenerator::GetMapInstance(const unsigned width, const unsigned height, ShapeRenderer3D &aShapeRenderer3D, Shader &ShaderProgram)
{

    MapTile ***map = new MapTile **[width];

    for (unsigned x = 0; x < width; x++)
    {
        map[x] = new MapTile *[height];
    }

    for (unsigned x = 0; x < width; x++)
    {
        for (unsigned y = 0; y < height; y++)
        {
            map[x][y] = new MapTile(aShapeRenderer3D, ShaderProgram, std::to_string(x) + "" + std::to_string(y));
        }
    }

    for (unsigned x = 0; x < width; x++)
    {
        for (unsigned y = 0; y < height; y++)
        {
            if (y < height - 1) //DownChild
                map[x][y]->kids.push_back(map[x][y + 1]);

            if (y > 0) //UpChild
                map[x][y]->kids.push_back(map[x][y - 1]);

            if (x > 0) //LeftChild
                map[x][y]->kids.push_back(map[x][x - 1]);

            if (x < width - 1) //RightChild
                map[x][y]->kids.push_back(map[x][x + 1]);
        }
    }

    std::vector<MapTile *> toReturn;

    for (unsigned x = 0; x < width; x++)
    {
        for (unsigned y = 0; y < height; y++)
        {
            toReturn.push_back(map[x][y]);
        }
    }
    return toReturn;
}