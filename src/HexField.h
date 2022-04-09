//
// Created by magnias on 4/3/22.
//

#ifndef CINDERTEST_HEXFIELD_H
#define CINDERTEST_HEXFIELD_H

#include <spdlog/spdlog.h>
#include "cinder/Vector.h"
#include "SymmetryOperation.h"
#include "GridUtils.h"
#include "HexNode.h"
#include "HexMap.h"

class HexField
{
public:
    HexField();

    void calculateNextDepth();

    void calculateGrid();

    HexMap<HexNode> getHexGrid();

private:

    HexMap<HexNode> grid = HexMap<HexNode>();

    SymmetryOperation triangle = GridUtils::createTriangle(glm::ivec3(3, -3, 0));
    SymmetryOperation hexagon = GridUtils::createHexagon(glm::ivec3(1, 1, -2));
    SymmetryOperation hexagon1 = GridUtils::createHexagon(glm::ivec3(4, -2, -2));
    SymmetryOperation lineup = GridUtils::createLine(glm::ivec3(0, 7, -7));
    SymmetryOperation lineup2 = GridUtils::createLine(glm::ivec3(2, 0, -2));
    SymmetryOperation lineright = GridUtils::createLine(glm::ivec3(-7, 0, 7));
    SymmetryOperation triangleUp = GridUtils::createTriangleUp(glm::ivec3(2, -2, 0));
    SymmetryOperation triangleUp2 = GridUtils::createTriangleUp(glm::ivec3(3, -3, 0));
    SymmetryOperation base = GridUtils::createHexagon(glm::ivec3(35, -35, -0));
    SymmetryOperation base2 = GridUtils::createLine(glm::ivec3(-13, 13, 0));
    SymmetryOperation base3 = GridUtils::createTriangleUp(glm::ivec3(2, -2, 0));
    SymmetryOperation base4 = GridUtils::createTriangleUp(glm::ivec3(2, -2, 0));

    // Calculation data
    std::vector<SymmetryOperation *> parentNodes = {&base};
    std::vector<glm::mat4> parentMatrices = {glm::mat4()};
    int max_depth = 18;
    int depth = 0;

    glm::vec3 palette(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
    glm::vec3 colorGradient(int depth, int max_depth, int children);
};


#endif //CINDERTEST_HEXFIELD_H
