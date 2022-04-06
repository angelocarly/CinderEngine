//
// Created by magnias on 4/3/22.
//

#ifndef CINDERTEST_HEXFIELD_H
#define CINDERTEST_HEXFIELD_H

#include <spdlog/spdlog.h>
#include "cinder/Vector.h"
#include "SymmetryOperation.h"
#include "GridUtils.h"
#include "BrainNode.h"
#include "HexGrid.h"

class HexField {
public:
    HexField() {
//        lineright.setAllChilds(&hexagon);
//        lineright.setAllChilds(&triangle);
//        hexagon1.setAllChilds(&hexagon);
//        triangle.setChild(0, &triangle);
//        triangle.setChild(1, &lineup);
//    triangle.setChild(2, &lineright);
//    triangleUp2.setAllChilds(&lineright);
        lineup2.setChild(0, &triangle);
        lineup2.setChild(1, &triangle);
        lineup.setChild(0, &triangleUp);
        lineup.setChild(1, &triangleUp);
//    lineup.setChild(0, &hexagon1);
        triangleUp.setChild(0, &lineup);
        triangleUp.setChild(1, &lineup);
        triangleUp.setChild(2, &lineup);
        hexagon.setChild(0, &hexagon1);
        hexagon.setChild(1, &hexagon1);
        hexagon.setChild(2, &hexagon1);
        hexagon.setChild(3, &triangle);
        hexagon.setChild(4, &triangle);
        hexagon.setChild(5, &triangle);
        hexagon1.setChild(0, &lineright);
        hexagon1.setChild(1, &lineright);
        hexagon1.setChild(2, &lineright);
        hexagon.setChild(3, &lineup);
        hexagon.setChild(4, &lineup);
        hexagon.setChild(5, &lineup);
    }

    glm::vec3 colorGradient(int depth, int max_depth, int children) {
//    Position = 0.0     Color = (  0,   7, 100)
//    Position = 0.16    Color = ( 32, 107, 203)
//    Position = 0.42    Color = (237, 255, 255)
//    Position = 0.6425  Color = (255, 170,   0)
//    Position = 0.8575  Color = (  0,   2,   0)
        float scale = (float) depth / max_depth;
        float col = (scale) * 0.8;
        glm::vec3 color = glm::vec3(col + 0.04, col + 0.02, col + 0.0);
//        if (children == 3) color = glm::vec3(scale) * glm::vec3(.1f, .4f, .2f);
//        if (children == 6) color = glm::vec3(scale) * glm::vec3(.3f, .1f, .2f) * 1.5f;
//        if (children == 6) return palette(scale, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.0f, 1.0f, .5f), glm::vec3(.8f, .9f, .3f));
        return palette(scale , glm::vec3(.5f, .2f, .4f), glm::vec3(.4f, .1f, .0f), glm::vec3(1.3f, 0.5f, 0.2f), glm::vec3(.94f, .25f, .6f));
//        return .8f * palette(scale + 1, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(.5f, .20f, .25f));
//        return palette(scale, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(.3f, .2f, .2f));
    }

    void calculateNextDepth() {

//        if (depth >= max_depth) return;

        // Breath first loop
        spdlog::info("Calculating depth {}, {} nodes", depth, parentNodes.size());

        std::vector<SymmetryOperation *> childNodes;
        std::vector<glm::mat4> childMatrices;

        for (int i = 0; i < parentNodes.size(); i++) {
            SymmetryOperation *op = parentNodes.at(i);
            glm::mat4 tr = parentMatrices.at(i);

            for (int c = 0; c < op->getChildSize(); c++) {
                glm::vec3 pos = tr * op->getChildTransform(c) * glm::vec4(0, 0, 0, 1);
                if (!grid.hasCell(pos) || grid.getCell(pos).depth >= depth + 1) {
                    grid.setCell(pos, {pos, colorGradient(depth, max_depth, op->getChildSize()), depth + 1});
                }
                if (grid.getCell(pos).depth + 1 >= depth + 1) {
                    if (op->getSymmetryOperation(c) != nullptr) {
                        childNodes.push_back(op->getSymmetryOperation(c));
                        childMatrices.push_back(tr * op->getChildTransform(c));
                    }
                }
            }

        }

        parentNodes = childNodes;
        parentMatrices = childMatrices;

        depth++;
    }


    void calculateGrid() {
        while (depth < max_depth) calculateNextDepth();
    }

    HexGrid<BrainNode> getHexGrid() {
        return grid;
    }

private:
    glm::vec3 palette( float t, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d )
    {
        return a + b*cos( glm::vec3(6.28318) * (c*t+d) );
    }

    HexGrid<BrainNode> grid = HexGrid<BrainNode>();

    SymmetryOperation triangle = GridUtils::createTriangle(glm::ivec3(3, -3, 0));
    SymmetryOperation hexagon = GridUtils::createHexagon(glm::ivec3(1, 1, -2));
    SymmetryOperation hexagon1 = GridUtils::createHexagon(glm::ivec3(4, -2, -2));
    SymmetryOperation lineup = GridUtils::createLine(glm::ivec3(0, 7, -7));
    SymmetryOperation lineup2 = GridUtils::createLine(glm::ivec3(2, 0, -2));
    SymmetryOperation lineright = GridUtils::createLine(glm::ivec3(-2, 0, 2));
    SymmetryOperation triangleUp = GridUtils::createTriangleUp(glm::ivec3(2, -2, 0));
    SymmetryOperation triangleUp2 = GridUtils::createTriangleUp(glm::ivec3(3, -3, 0));

    // Calculation data
    std::vector<SymmetryOperation *> parentNodes = {&hexagon};
    std::vector<glm::mat4> parentMatrices = {glm::mat4()};
    int max_depth = 18;
    int depth = 0;
};


#endif //CINDERTEST_HEXFIELD_H
