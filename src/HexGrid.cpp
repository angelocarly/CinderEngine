//
// Created by magnias on 3/22/22.
//

#include "HexGrid.h"

template<typename T>
HexGrid<T>::HexGrid()
{
}

template<typename T>
void HexGrid<T>::setCell(glm::ivec3 pos, T hex)
{
    assertValidPosition(pos);
    hexagons[pos] = hex;
}

template<typename T>
void HexGrid<T>::setCell(int x, int y, int z, T hex)
{
    setCell(glm::ivec3(x, y, z), hex);

}

template<typename T>
T HexGrid<T>::getCell(glm::ivec3 pos)
{
    assertValidPosition(pos);

    return hexagons[pos];
}

template<typename T>
T HexGrid<T>::getCell(int x, int y, int z)
{
    return getCell(glm::ivec3(x, y, z));
}

template<typename T>
bool HexGrid<T>::hasCell(glm::ivec3 pos)
{
    return hexagons.count(pos) != 0;
}

template<typename T>
bool HexGrid<T>::isPositionValid(glm::ivec3 pos)
{
   return pos.x + pos.y + pos.z == 0;
}

template<typename T>
void HexGrid<T>::assertValidPosition(glm::ivec3 pos)
{
    if (!isPositionValid(pos))
    {
        std::cerr << "Invalid position" << std::endl;
        exit(1);
    }
}
template<typename T>
typename std::unordered_map<glm::ivec3 , T>::iterator HexGrid<T>::begin()
{
    return hexagons.begin();
}

template<typename T>
typename std::unordered_map<glm::ivec3 , T>::iterator HexGrid<T>::end()
{
    return hexagons.end();
}
