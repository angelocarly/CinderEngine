//
// Created by magnias on 3/22/22.
//

#ifndef CINDERTEST_HEXGRID_H
#define CINDERTEST_HEXGRID_H

#include "cinder/Vector.h"

template <typename T>

class HexGrid
{
public:

    explicit HexGrid(int radius);

    void setHexagon(glm::ivec3 pos, T hex);
    void setHexagon(int q, int r, int s, T hex);
    T getHexagon(glm::ivec3 pos);
    T getHexagon(int q, int r, int s);


private:
    int tSize, rSize, sSize;
    int totalHexagons;
    T* hexagons;

    bool isValidPosition(int q, int r, int s);

    int calcIndex(glm::ivec3 pos);
    int calcIndex(int r, int s, int t);

    bool verifyPosition(int r, int s, int t);
};

template class HexGrid<int>;
template class HexGrid<glm::vec3>;

#endif //CINDERTEST_HEXGRID_H
