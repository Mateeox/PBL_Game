#include "MapTileUtils.hpp"

std::vector<MapTile *> MapTileUtils::GetMapInstance(const unsigned width,
                                                    const unsigned height,
                                                    Drawable *aDrawable)
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
            map[x][y] = new MapTile(aDrawable, std::to_string(x) + "" + std::to_string(y));
            map[x][y]->mSceneNode.Translate(2+4.5 * x, -1.8,2+ 4.5 * y);
              map[x][y]->mSceneNode.Scale(4.5, 4.5, 4.5);
             map[x][y]->mSceneNode.Rotate(90.0f, glm::vec3(1, 0, 0));
            
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