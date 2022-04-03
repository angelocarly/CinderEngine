//
// Created by magnias on 4/3/22.
//

#ifndef CINDERTEST_HEXFIELD_H
#define CINDERTEST_HEXFIELD_H

#include "cinder/Vector.h"

class HexField {
public:
    std::vector<glm::ivec3> mirrorTwofold(glm::ivec3 pos);
    std::vector<glm::ivec3> mirrorTwofold(glm::ivec3 pos, std::vector<glm::ivec3> nodes);
    std::vector<glm::ivec3> mirrorThreefold(glm::ivec3 pos);
    std::vector<glm::ivec3> mirrorThreefold(glm::ivec3 pos, std::vector<glm::ivec3> nodes);
};


#endif //CINDERTEST_HEXFIELD_H
