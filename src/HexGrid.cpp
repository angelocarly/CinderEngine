//
// Created by magnias on 3/22/22.
//

#include "HexGrid.h"

template<typename T>
HexGrid<T>::HexGrid(int radius)
{
    rSize = radius;
    sSize = radius;
    tSize = radius;
    this->totalHexagons = 1 + 6 * radius;
    this->hexagons = new T[totalHexagons];
}

template<typename T>
void HexGrid<T>::setHexagon(glm::ivec3 pos, T hex)
{
    setHexagon(pos.r, pos.s, pos.t, hex);
}

template<typename T>
void HexGrid<T>::setHexagon(int r, int s, int t, T hex)
{
    verifyPosition(r, s, t);

    hexagons[calcIndex(r, s, t)] = hex;
}

template<typename T>
T HexGrid<T>::getHexagon(glm::ivec3 pos)
{
    return getHexagon(pos.r, pos.s, pos.t);
}

template<typename T>
T HexGrid<T>::getHexagon(int r, int s, int t)
{
    verifyPosition(r, s, t);

    return hexagons[calcIndex(r, s, t)];
}

template<typename T>
bool HexGrid<T>::verifyPosition(int r, int s, int t)
{
    if (!isValidPosition(r, s, t)) {
        std::cerr << "Position does not exist";
        throw 1;
    }
}

template<typename T>
bool HexGrid<T>::isValidPosition(int r, int s, int t)
{
   return r + s + t == 0;
}

template<typename T>
int HexGrid<T>::calcIndex(glm::ivec3 pos)
{
    return calcIndex(pos.r, pos.s, pos.t);
}

template<typename T>
 int HexGrid<T>::calcIndex(int r, int s, int t)
{
    return (r + s * 2 + t * 4) % totalHexagons;
}
