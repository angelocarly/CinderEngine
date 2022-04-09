//
// Created by magnias on 4/3/22.
//

#include "HexField.h"

HexField::HexField()
{
    base.setAllChilds(&base2);
    base2.setAllChilds(&base4);
    base3.setAllChilds(&triangle);
    base4.setAllChilds(&base3);
    lineright.setAllChilds(&lineup);
//        lineright.setAllChilds(&triangle);
//        hexagon1.setAllChilds(&hexagon);
//        triangle.setChild(0, &triangle);
//        triangle.setChild(1, &lineup);
//    triangle.setChild(2, &lineright);
    triangle.setAllChilds(&lineright);
    lineup2.setChild(0, &triangle);
    lineup2.setChild(1, &triangle);
    lineup.setChild(0, &base4);
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
    hexagon.setChild(3, &triangle);
    hexagon.setChild(4, &lineup);
    hexagon.setChild(5, &lineup);
}

glm::vec3 HexField::palette(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
    return a + b * cos(glm::vec3(6.28318) * (c * t + d));
}

glm::vec3 HexField::colorGradient(int depth, int max_depth, int children)
{
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
//        return glm::vec3(children / 6.0f);
//        return palette(scale , glm::vec3(.5f, .2f, .4f), glm::vec3(.4f, .1f, .0f), glm::vec3(1.3f, 0.5f, 0.2f), glm::vec3(.94f, .25f, .6f));
//    return .8f * palette(scale + 1, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(.5f, .20f, .25f));
        return palette(scale, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(.3f, .2f, .2f));
}

void HexField::calculateNextDepth()
{

//        if (depth >= max_depth) return;

    // Breath first loop
    spdlog::info("Calculating depth {}, {} nodes", depth, parentNodes.size());

    std::vector<SymmetryOperation *> childNodes;
    std::vector<glm::mat4> childMatrices;
    int childDepth = depth + 1;

    for (int i = 0; i < parentNodes.size(); i++)
    {
        SymmetryOperation *op = parentNodes.at(i);
        glm::mat4 tr = parentMatrices.at(i);

        for (int c = 0; c < op->getChildSize(); c++)
        {
            glm::mat4 childTransform = tr * op->getChildTransform(c);
            SymmetryOperation* childSymmetry = op->getSymmetryOperation(c);

            glm::vec3 pos = childTransform * glm::vec4(0, 0, 0, 1);
            if (!grid.hasCell(pos) || grid.getCell(pos).depth >= childDepth)
            {

                if (childSymmetry != nullptr)
                {
                    childNodes.push_back(childSymmetry);
                    childMatrices.push_back(childTransform);
                }

                grid.setCell(pos, {pos, colorGradient(childDepth, max_depth, op->getChildSize()), childDepth});
            }
        }

    }

    parentNodes = childNodes;
    parentMatrices = childMatrices;
    depth = childDepth;
}


void HexField::calculateGrid()
{
    while (depth < max_depth) calculateNextDepth();
}

HexMap<HexNode> HexField::getHexGrid()
{
    return grid;
}
