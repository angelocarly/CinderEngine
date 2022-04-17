//
// Created by magnias on 4/4/22.
//

#ifndef CINDERTEST_GRIDUTILS_H
#define CINDERTEST_GRIDUTILS_H

#include "cinder/Vector.h"
#include "SymmetryOperation.h"

class GridUtils
{
public:

    static std::vector<glm::mat4> createTriangle(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot30 = GridUtils::rotateThirdRight();
        return {translationUp, rot30 * translationUp, rot30 * rot30 * translationUp};
    }

    static std::vector<glm::mat4> createTriangle(glm::ivec3 pos, glm::mat4 base)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot30 = GridUtils::rotateThirdRight();
        return {translationUp, rot30 * translationUp, rot30 * rot30 * translationUp};
    }

    // Todo rename
    static std::vector<glm::mat4> createTriangleUp(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot60 = GridUtils::rotateSixthRight();
        return {translationUp, rot60 * translationUp, glm::inverse(rot60) * translationUp};
    }

    static std::vector<glm::mat4> createTriangleUp(glm::ivec3 pos, glm::mat4 base)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot60 = GridUtils::rotateSixthRight();
        return {base * translationUp, base * rot60 * translationUp, base * glm::inverse(rot60) * translationUp};
    }

    static std::vector<glm::mat4> createHexagon(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot30 = GridUtils::rotateThirdRight();
        glm::mat4 rot60 = GridUtils::rotateSixthRight();
        return {translationUp, rot30 * translationUp, rot30 * rot30 * translationUp,
                 rot60 * translationUp, rot60 * rot30 * translationUp, rot60 * rot30 * rot30 * translationUp};
    }

    static std::vector<glm::mat4> createLine(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        return {translationUp, glm::inverse(translationUp)};
    }

    static glm::mat4 createTranslationMatrix(glm::ivec3 position)
    {
        return glm::mat4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                position.x, position.y, position.z, 1);
    }

    static glm::mat4 rotateSixthRight()
    {
        return glm::mat4(
                0, -1, 0, 0,
                0, 0, -1, 0,
                -1, 0, 0, 0,
                0, 0, 0, 1);
    }

    static glm::mat4 rotateThirdRight()
    {
        return glm::mat4(
                0, 1, 0, 0,
                0, 0, 1, 0,
                1, 0, 0, 0,
                0, 0, 0, 1);
    }
};

#endif //CINDERTEST_GRIDUTILS_H
