//
// Created by magnias on 4/6/22.
//

#ifndef CINDERTEST_SYMMETRYOPERATION_H
#define CINDERTEST_SYMMETRYOPERATION_H

#include <spdlog/spdlog.h>

#include <utility>
#include "cinder/Vector.h"

class SymmetryOperation {
public:
    SymmetryOperation(const std::vector<glm::mat4>& childTransforms, const std::vector<int>& symmetryOperations)
            : childTransforms(childTransforms), symmetryOperations(symmetryOperations) {
        if (childTransforms.size() != symmetryOperations.size()) {
            spdlog::error("Failed initializing SymmetryOperation because of mismatching size. childtransform.size: {}, symmetryOperations.size: {}",
                          childTransforms.size(), symmetryOperations.size());
        }
    }

    SymmetryOperation() {

    }

    int getChildSize() {
        return childTransforms.size();
    }

    glm::mat4 getChildTransform(int child) {
        if (child > getChildSize()) {
            spdlog::error("Child not found: {}", child);
            std::exit(1);
        }
        return childTransforms.at(child);
    }

    int getSymmetryOperation(int child) {
        if (child > getChildSize()) {
            spdlog::error("Child not found: {}", child);
            std::exit(1);
        }
        return symmetryOperations.at(child);
    }

    void setChild(int child, int operation) {
        if (child > getChildSize()) {
            spdlog::error("Child not found: {}", child);
            std::exit(1);
        }
        symmetryOperations.at(child) = operation;
    }

    void setAllChilds(int operation) {
        for(int i=0; i < symmetryOperations.size(); i++) {
            symmetryOperations.at(i) = operation;
        }
    }


private:
    std::vector<int> symmetryOperations;
    std::vector<glm::mat4> childTransforms;
};

#endif //CINDERTEST_SYMMETRYOPERATION_H
