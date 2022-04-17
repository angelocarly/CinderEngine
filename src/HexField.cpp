//
// Created by magnias on 4/3/22.
//

#include "HexField.h"

HexField::HexField()
{
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
//    return palette(scale, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(.3f, .2f, .2f));
    return palette(scale, glm::vec3(.5f, .5f, .5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(.8f, .9f, .3f));
}

void HexField::calculateNextDepth()
{

    if (depth == 0) {
        parentNodes = {&(symmetryStructure.front())};
    }
//        if (depth >= max_depth) return;

    // Breath first loop
    spdlog::info("Calculating depth {}, {} nodes", depth, parentNodes.size());

    std::vector<SymmetryOperation *> childNodes;
    std::vector<glm::mat4> childMatrices;
    std::vector<float> childColors;
    int childDepth = depth + 1;

    for (int i = 0; i < parentNodes.size(); i++)
    {
        SymmetryOperation *op = parentNodes.at(i);
        glm::mat4 tr = parentMatrices.at(i);
        float colorValue = parentColors.at(i);

        for (int c = 0; c < op->getChildSize(); c++)
        {
            glm::mat4 childTransform = tr * op->getChildTransform(c);

            glm::vec3 pos = childTransform * glm::vec4(0, 0, 0, 1);
            if (!grid.hasCell(pos) || grid.getCell(pos).depth >= childDepth)
            {

                if (op->getSymmetryOperation(c) >= 0)
                {
                    childNodes.push_back(&symmetryStructure.at(op->getSymmetryOperation(c)));
                    childMatrices.push_back(childTransform);
                    childColors.push_back(colorValue + op->getChildSize());
                }

                grid.setCell(pos, {
                    pos,
                    colorGradient(childDepth, max_depth, op->getChildSize()),
                    childDepth,
                    colorValue
                });
            }

        }

    }

    parentNodes = childNodes;
    parentMatrices = childMatrices;
    parentColors = childColors;
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

void HexField::addStructure(const std::vector<glm::mat4>& childMatrices, const std::vector<int>& relativeChildIndices)
{
    int currentNode = this->symmetryStructure.size();

    std::vector<int> childIndices;
    childIndices.resize(relativeChildIndices.size());
    for( int i=0; i<childIndices.size(); i++) {
        childIndices.at(i) = currentNode + relativeChildIndices.at(i);
    }
    SymmetryOperation s = SymmetryOperation(childMatrices, childIndices);
    this->symmetryStructure.push_back(s);
}

void HexField::addStructure(const std::vector<glm::mat4> &childMatrices, int allChildIndices)
{
    int currentNode = this->symmetryStructure.size();

    std::vector<int> childIndices;
    childIndices.resize(childMatrices.size());
    for( int i=0; i<childIndices.size(); i++) {
        childIndices.at(i) = currentNode + allChildIndices;
    }

    SymmetryOperation s = SymmetryOperation(childMatrices, childIndices);

    this->symmetryStructure.push_back(s);
}

void HexField::addStructure(const std::vector<glm::mat4> &childMatrices)
{
    std::vector<int> childSymmetries;
    childSymmetries.resize(childMatrices.size());
    for (int i=0; i<childSymmetries.size(); i++) childSymmetries.at(i) = -1;

    SymmetryOperation s = SymmetryOperation(childMatrices, childSymmetries);
    this->symmetryStructure.push_back(s);
}

std::vector<SymmetryOperation *> HexField::getSymmetryOperations(const std::vector<int>& indices)
{
    std::vector<SymmetryOperation*> symmetryOperationAddresses;

    for (int i: indices)
    {
        // TODO: clean up
        if (i + 1 > symmetryStructure.size())
        {
            symmetryStructure.resize(i + 1);
        }
        symmetryOperationAddresses.push_back(&(symmetryStructure[i]));
    }

    return symmetryOperationAddresses;
}