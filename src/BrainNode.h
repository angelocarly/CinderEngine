//
// Created by magnias on 3/31/22.
//

#ifndef CINDERTEST_BRAINNODE_H
#define CINDERTEST_BRAINNODE_H

#include "cinder/Vector.h"

struct BrainNode
{
    glm::ivec3 pos;
    glm::vec3 color;
    int depth;
    std::vector<glm::ivec3> children;
};

#endif //CINDERTEST_BRAINNODE_H
