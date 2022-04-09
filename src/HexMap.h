//
// Created by magnias on 3/22/22.
//

#ifndef CINDERTEST_HEXMAP_H
#define CINDERTEST_HEXMAP_H

#include "cinder/Vector.h"
#include "HexNode.h"
#include <map>


struct ivec3HashFuncs
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
class HexMap
{
public:

    explicit HexMap();

    void setCell(glm::ivec3 pos, T hex);
    void setCell(int x, int y, int z, T hex);
    T getCell(glm::ivec3 pos);
    T getCell(int x, int y, int z);
    bool hasCell(glm::ivec3 pos);

    typename std::unordered_map<glm::ivec3, T>::iterator end();

    typename std::unordered_map<glm::ivec3, T>::iterator begin();

private:
    std::unordered_map<glm::ivec3, T, ivec3HashFuncs, ivec3HashFuncs> hexagons;

    void assertValidPosition(glm::ivec3 pos);

    bool isPositionValid(glm::ivec3 pos);
};

template class HexMap<HexNode>;

#endif //CINDERTEST_HEXMAP_H
