//
// Created by magnias on 4/3/22.
//

#include "HexField.h"

std::vector<glm::ivec3> HexField::mirrorTwofold(glm::ivec3 pos) {
    std::vector<glm::ivec3> reflections;
    reflections.push_back(pos);
    reflections.push_back(glm::ivec3(pos.z, pos.x, pos.y));
    reflections.push_back(glm::ivec3(pos.y, pos.z, pos.x));
    return reflections;
}

std::vector<glm::ivec3> HexField::mirrorTwofold(glm::ivec3 pos, std::vector<glm::ivec3> nodes) {
    std::vector<glm::ivec3> reflections;
    for(glm::ivec3 node : nodes) {
        std::vector<glm::ivec3> twofoldmirror = mirrorTwofold(pos + node);
        reflections.insert(reflections.end(), twofoldmirror.begin(), twofoldmirror.end());
    }
    return reflections;
}

std::vector<glm::ivec3> HexField::mirrorThreefold(glm::ivec3 pos) {
    std::vector<glm::ivec3> reflections;
    reflections.push_back(pos);
    reflections.push_back(-pos);
    return reflections;
}

std::vector<glm::ivec3> HexField::mirrorThreefold(glm::ivec3 pos, std::vector<glm::ivec3> nodes) {
    std::vector<glm::ivec3> reflections;
    for(glm::ivec3 node : nodes) {
        std::vector<glm::ivec3> twofoldmirror = mirrorTwofold(pos + node);
        reflections.insert(reflections.end(), twofoldmirror.begin(), twofoldmirror.end());
    }
    return reflections;
}
