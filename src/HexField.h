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

    void addStructure(const std::vector<glm::mat4>& childMatrices, const std::vector<int>& childIndices);
    void addStructure(const std::vector<glm::mat4>& childMatrices, int allChildIndices);
    void addStructure(const std::vector<glm::mat4>& childMatrices);

    void calculateNextDepth();

    void calculateGrid();

    HexMap<HexNode> getHexGrid();

private:

    HexMap<HexNode> grid = HexMap<HexNode>();

    // Calculation data
    std::vector<SymmetryOperation *> parentNodes = {};
    std::vector<glm::mat4> parentMatrices = {glm::mat4()};
    std::vector<float> parentColors = {0.0f};
    int max_depth = 18;
    int depth = 0;
    std::vector<SymmetryOperation> symmetryStructure;

    glm::vec3 palette(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
    glm::vec3 colorGradient(int depth, int max_depth, int children);

    std::vector<SymmetryOperation*> getSymmetryOperations(const std::vector<int>& indices);
};


#endif //CINDERTEST_HEXFIELD_H
