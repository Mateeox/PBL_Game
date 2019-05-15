#include "MapTileUtils.hpp"
#include <iomanip>
#include <algorithm>
MapTile ***MapTileUtils::GetMapInstance(int width,
                                        int height,
                                        ShapeRenderer3D *aDrawable,
                                        float MapScale,
                                        float FloorTranslation)
{

    MapTile ***map = new MapTile **[width];

    for (unsigned x = 0; x < width; x++)
    {
        map[x] = new MapTile *[height];
    }

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            map[x][y] = new MapTile(aDrawable->GetCopy(), std::to_string(x) + "" + std::to_string(y));
            map[x][y]->x_cord = x;
            map[x][y]->y_cord = y;
            map[x][y]->mSceneNode.Translate(2 + MapScale * x, FloorTranslation, 2 + MapScale * y);
            map[x][y]->mSceneNode.Scale(MapScale, MapScale, MapScale);
            map[x][y]->mSceneNode.Rotate(90.0f, glm::vec3(1, 0, 0));
        }
    }

    for (unsigned x = 0; x < width; x++)
    {
        for (unsigned y = 0; y < height; y++)
        {
            if (y < height - 1 && !map[x][y + 1]->isBlocked()) //DownChild
                map[x][y]->neighbors.push_back(map[x][y + 1]);

            if (y > 0 && !map[x][y - 1]->isBlocked()) //UpChild
                map[x][y]->neighbors.push_back(map[x][y - 1]);

            if (x > 0 && !map[x - 1][y]->isBlocked()) //LeftChild
                map[x][y]->neighbors.push_back(map[x - 1][y]);

            if (x < width - 1 && !map[x + 1][y]->isBlocked()) //RightChild
                map[x][y]->neighbors.push_back(map[x + 1][y]);
        }
    }

    return map;
}

int Manhatan(int A_x, int A_y, int B_x, int B_y)
{
    return (std::abs(A_x - B_x) + std::abs(A_y - B_y));
}

std::vector<MapTile *> MapTileUtils::FindPath(MapTile ***map, MapTile *aStart, MapTile *aEnd, const int m, const int n)
{

    std::vector<MapTile *> mapPath;
    unsigned StepNumber = 0;
    MapTile *currentMapTile = aStart;

    int **CostMap = new int *[m];
    for (int i = 0; i < m; ++i)
        CostMap[i] = new int[n];

    for (int x = 0; x < m; x++)
    {
        for (int y = 0; y < n; y++)
        {
            CostMap[x][y] = Manhatan(aStart->x_cord, aStart->y_cord, map[x][y]->x_cord, map[x][y]->y_cord);
        }
    }

    MapTile *closestMapTile = aStart;
    int depth = 0;
    while (currentMapTile != aEnd && depth < 40)
    {
        if (closestMapTile == aStart)
        {
            closestMapTile = GetClosestMapTile(aStart->neighbors, map, aEnd->x_cord, aEnd->y_cord);
        }
        else
        {
            closestMapTile = GetClosestMapTile(currentMapTile->neighbors, map, aEnd->x_cord, aEnd->y_cord);
        }
        currentMapTile = closestMapTile;
        mapPath.push_back(closestMapTile);
        depth++;
    }

    for (int i = 0; i < m; ++i)
    {
        delete[] CostMap[i];
    }
    delete[] CostMap;

    return mapPath;
}

MapTile *MapTileUtils::GetClosestMapTile(std::vector<MapTile *> children, MapTile ***map, int target_x, int target_y)
{
    std::vector<MapTile *> toSort;
    for (int i = 0; i < children.size(); i++)
    {
        MapTile *tmp = children[i];
        tmp->temporaryCost = Manhatan(tmp->x_cord, tmp->y_cord, map[target_x][target_y]->x_cord, map[target_x][target_y]->y_cord);
        toSort.push_back(tmp);
    }

    std::sort(toSort.begin(), toSort.end(), [](const MapTile *a, const MapTile *b) {
        return a->temporaryCost < b->temporaryCost;
    });

    return toSort[0];
}