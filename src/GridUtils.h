//
// Created by magnias on 4/4/22.
//

#ifndef CINDERTEST_GRIDUTILS_H
#define CINDERTEST_GRIDUTILS_H

#include "cinder/Vector.h"
#include "HexField.h"

class GridUtils
{
public:

    static SymmetryOperation createTriangle(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot30 = GridUtils::rotateThirdRight();
        return SymmetryOperation(
                {translationUp, rot30 * translationUp, rot30 * rot30 * translationUp});
    }

    // Todo rename
    static SymmetryOperation createTriangleUp(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot60 = GridUtils::rotateSixthRight();
        return SymmetryOperation(
                {translationUp, rot60 * translationUp, glm::inverse(rot60) * translationUp});
    }

    static SymmetryOperation createHexagon(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        glm::mat4 rot30 = GridUtils::rotateThirdRight();
        return SymmetryOperation(
                {translationUp, rot30 * translationUp, rot30 * rot30 * translationUp,
                 glm::inverse(translationUp), rot30 * glm::inverse(translationUp), rot30 * rot30 * glm::inverse(translationUp)}
        );
    }

    static SymmetryOperation createLine(glm::ivec3 pos)
    {
        glm::mat4 translationUp = GridUtils::createTranslationMatrix(pos);
        return SymmetryOperation(
               {translationUp, glm::inverse(translationUp)}
        );
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

    static glm::vec3 transform(glm::mat4 transform, glm::ivec3 position)
    {
        return transform * glm::ivec4(position.x, position.y, position.z, 1.0f);
    }
};

#endif //CINDERTEST_GRIDUTILS_H
