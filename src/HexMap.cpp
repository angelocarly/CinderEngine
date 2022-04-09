//
// Created by magnias on 3/22/22.
//

#include "HexMap.h"

template<typename T>
HexMap<T>::HexMap()
{
}

template<typename T>
void HexMap<T>::setCell(glm::ivec3 pos, T hex)
{
    assertValidPosition(pos);
    hexagons[pos] = hex;
}

template<typename T>
void HexMap<T>::setCell(int x, int y, int z, T hex)
{
    setCell(glm::ivec3(x, y, z), hex);

}

template<typename T>
T HexMap<T>::getCell(glm::ivec3 pos)
{
    assertValidPosition(pos);

    return hexagons[pos];
}

template<typename T>
T HexMap<T>::getCell(int x, int y, int z)
{
    return getCell(glm::ivec3(x, y, z));
}

template<typename T>
bool HexMap<T>::hasCell(glm::ivec3 pos)
{
    return hexagons.count(pos) != 0;
}

template<typename T>
bool HexMap<T>::isPositionValid(glm::ivec3 pos)
{
   return pos.x + pos.y + pos.z == 0;
}

template<typename T>
void HexMap<T>::assertValidPosition(glm::ivec3 pos)
{
    if (!isPositionValid(pos))
    {
        std::cerr << "Invalid position" << std::endl;
        exit(1);
    }
}
template<typename T>
typename std::unordered_map<glm::ivec3 , T>::iterator HexMap<T>::begin()
{
    return hexagons.begin();
}

template<typename T>
typename std::unordered_map<glm::ivec3 , T>::iterator HexMap<T>::end()
{
    return hexagons.end();
}
