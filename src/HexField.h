//
// Created by magnias on 4/3/22.
//

#ifndef CINDERTEST_HEXFIELD_H
#define CINDERTEST_HEXFIELD_H

#include <spdlog/spdlog.h>
#include "cinder/Vector.h"

class SymmetryOperation {
public:
    explicit SymmetryOperation(std::vector<glm::mat4> childTransforms)
            : _childTransforms(childTransforms) {
        _symmetryOperations.resize(childTransforms.size());
    }

    int getChildSize() {
        return _childTransforms.size();
    }

    glm::mat4 getChildTransform(int child) {
        if (child > getChildSize()) {
            spdlog::error("Child not found: {}", child);
            std::exit(1);
        }
        return _childTransforms.at(child);
    }

    SymmetryOperation *getSymmetryOperation(int child) {
        if (child > getChildSize()) {
            spdlog::error("Child not found: {}", child);
            std::exit(1);
        }
        return _symmetryOperations.at(child);
    }

    void setChild(int child, SymmetryOperation *operation) {
        if (child > getChildSize()) {
            spdlog::error("Child not found: {}", child);
            std::exit(1);
        }
        _symmetryOperations.at(child) = operation;
    }

    void setAllChilds(SymmetryOperation * operation) {
       for(int i=0; i<_symmetryOperations.size(); i++) {
           _symmetryOperations.at(i) = operation;
       }
    }

private:
    std::vector<SymmetryOperation *> _symmetryOperations;
    std::vector<glm::mat4> _childTransforms;
};

class HexField {
public:
    HexField(SymmetryOperation *baseSymmetry) {

    }

private:
};


#endif //CINDERTEST_HEXFIELD_H
