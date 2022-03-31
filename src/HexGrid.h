//
// Created by magnias on 3/22/22.
//

#ifndef CINDERTEST_HEXGRID_H
#define CINDERTEST_HEXGRID_H

#include "cinder/Vector.h"
#include <map>


struct KeyFuncs
{
    size_t operator()(const glm::ivec3& k)const
    {
        return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
    }

    bool operator()(const glm::ivec3& a, const glm::ivec3& b)const
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

template <typename T>
class HexGrid
{
public:

    explicit HexGrid();

    void setCell(glm::ivec3 pos, T hex);
    void setCell(int x, int y, int z, T hex);
    T getCell(glm::ivec3 pos);
    T getCell(int x, int y, int z);
    bool hasCell(glm::ivec3 pos);

private:
    std::unordered_map<glm::ivec3, T, KeyFuncs, KeyFuncs> hexagons;

    void assertValidPosition(glm::ivec3 pos);

    bool isPositionValid(glm::ivec3 pos);
};

template class HexGrid<glm::vec3>;

#endif //CINDERTEST_HEXGRID_H
