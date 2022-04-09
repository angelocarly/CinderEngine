//
// Created by magnias on 3/31/22.
//

#ifndef CINDERTEST_HEXNODE_H
#define CINDERTEST_HEXNODE_H

#include "cinder/Vector.h"

struct HexNode
{
    glm::ivec3 pos;
    glm::vec3 color;
    int depth;
};

#endif //CINDERTEST_HEXNODE_H
