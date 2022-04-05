#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HexGrid.h"
#include "GridUtils.h"
#include "cinder/Rand.h"
#include "BrainNode.h"
#include "HexField.h"
#include <spdlog/spdlog.h>
#include <stack>

using namespace ci;
using namespace ci::app;

class CinderApp : public App {
public:

    void setup() override;

    void mouseDrag(MouseEvent event) override;

    void mouseDown(MouseEvent event) override;

    void keyDown(KeyEvent event) override;

    void draw() override;

private:

    HexGrid<BrainNode> grid = HexGrid<BrainNode>();

    vec2 toWindowPos(vec2 position);

    vec2 windowSize;

    vec2 mousePosition;
    vec2 cameraPosition;

    int hexTiles = 90;
    float diameter = 1.0f / hexTiles;
    float hexRadius = diameter / sqrt(3);

    // hex 'normals'
    glm::mat2 hexToScreenMatrix = glm::mat2(-1, 0,
                                            -1 / 2.0f, sqrt(3.0f) / 2);
    glm::mat2 hexToScreenMatrixPointyTop = glm::mat2(-sqrt(3.0f) / 2, 1.0f / 2,
                                                     0.0f, 1.0f);

    glm::vec2 rDir = hexToScreenMatrixPointyTop * glm::vec2(1, 0) * glm::vec2(hexRadius);
    glm::vec2 sDir = hexToScreenMatrixPointyTop * glm::vec2(0, -1) * glm::vec2(hexRadius);
    glm::vec2 tDir = hexToScreenMatrixPointyTop * glm::vec2(-1, 1) * glm::vec2(hexRadius);

    ivec3 screenPosToHexPos(vec2 position);

    void fillRandomHexagon();

    vec3 colorGradient(float value);
};

glm::vec3 CinderApp::colorGradient(float value) {
//    Position = 0.0     Color = (  0,   7, 100)
//    Position = 0.16    Color = ( 32, 107, 203)
//    Position = 0.42    Color = (237, 255, 255)
//    Position = 0.6425  Color = (255, 170,   0)
//    Position = 0.8575  Color = (  0,   2,   0)
    float col = value * value * 0.8;
    return glm::vec3(col + 0.04, col + 0.02, col + 0.0);
}

void CinderApp::setup() {
    windowSize = vec2(getWindowWidth(), getWindowHeight());

//    for (int i = 0; i < 30; i++) {
//        fillRandomHexagon();
//    }

    SymmetryOperation triangle = GridUtils::createTriangle(glm::ivec3(2, -3, 1));
    SymmetryOperation hexagon = GridUtils::createHexagon(glm::ivec3(2, -3, 1));
    SymmetryOperation hexagon1 = GridUtils::createHexagon(glm::ivec3(4, -4, 0));
    SymmetryOperation lineup = GridUtils::createLine(glm::ivec3(0, 1, -1));
    SymmetryOperation lineright = GridUtils::createLine(glm::ivec3(0, -5, 5));
    SymmetryOperation triangleUp = GridUtils::createTriangleUp(glm::ivec3(3, -3, 0));

    triangleUp.setAllChilds(&lineup);
    lineup.setChild(0, &triangleUp);
    lineup.setChild(1, &lineright);
    triangle.setChild(0, &lineright);
    triangle.setChild(1, &hexagon);
    triangle.setChild(2, &lineright);
    hexagon.setAllChilds(&triangle);
    lineright.setAllChilds(&triangleUp);
    hexagon1.setAllChilds(&lineup);

    std::vector<glm::mat4> transformStack = std::vector<glm::mat4>();
    std::vector<SymmetryOperation *> operationStack;
    std::vector<int> childIndexStack;
    std::vector<bool> validOverWrites;

    // Start the depth search with the first node, starting with first child
    int depth = 0;
    int max_depth = 15;
    int child = 0;
    SymmetryOperation *op = &hexagon;
    glm::mat4 tr = glm::mat4();
    do {

        if (depth < 4) {
            spdlog::info("Calculating child {} on depth {}", child, depth);
        }

        int childCellsPlaced = 0;
        for (int c = 0; c < op->getChildSize(); c++) {
            glm::vec3 pos = tr * op->getChildTransform(c) * glm::vec4(0, 0, 0, 1);
            if (!grid.hasCell(pos) || grid.getCell(pos).depth >= depth + 1) {
                grid.setCell(pos, {pos, colorGradient((float) depth / max_depth), depth + 1});

                childCellsPlaced++;
            }
        }

        // Try to traverse the current child
        if (op->getChildSize() < 3 || childCellsPlaced > 2) {
            if (child < op->getChildSize() && op->getSymmetryOperation(child) != nullptr && depth < max_depth) {
                // Push parent state
                transformStack.push_back(tr);
                operationStack.push_back(op);
                childIndexStack.push_back(child + 1);

                // Set argument for the child
                tr = tr * op->getChildTransform(child);
                op = op->getSymmetryOperation(child);
                child = 0;
                depth++;
                continue;

            } else if (child + 1 < op->getChildSize() && op->getSymmetryOperation(child) == nullptr) {

                // There's more children to traverse
                glm::vec3 pos = tr * op->getChildTransform(child) * glm::vec4(0, 0, 0, 1);
                child++;
                continue;
            }
        }

        // Go to parent
        depth--;

        // pop parent state
        child = childIndexStack.back();
        op = operationStack.back();
        tr = transformStack.back();
        childIndexStack.pop_back();
        operationStack.pop_back();
        transformStack.pop_back();
        continue;

    } while (depth >= 0);

    spdlog::info("Setup completed");

}

void prepareSettings(CinderApp::Settings *settings) {
    settings->setTitle("cinder");
    settings->setWindowSize(1200, 1200);
    settings->setMultiTouchEnabled(false);
}

void CinderApp::mouseDown(MouseEvent event) {
    glm::vec2 mousePos = glm::vec2(event.getPos()) / windowSize - glm::vec2(0.5f, 0.5f);

    BrainNode node;
    node.pos = screenPosToHexPos(mousePos);
    node.color = glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f));

    grid.setCell(node.pos, node);
}


void CinderApp::mouseDrag(MouseEvent event) {
    glm::vec2 distance = event.getPos() / getWindowSize();
    cameraPosition += distance;
    spdlog::info(event.getPos().x);
    spdlog::info(cameraPosition.x);
}

void CinderApp::keyDown(KeyEvent event) {
    if (event.getChar() == 'a') {
        for (int i = 0; i < 30; i++) {
            fillRandomHexagon();
        }
    }
}

void CinderApp::draw() {
    gl::clear(Color::gray(0.06f));

    gl::translate(cameraPosition);

    gl::begin(GL_TRIANGLES);
    {

        for (std::pair<glm::ivec3, BrainNode> b: grid) {
            glm::vec3 color = b.second.color;

            gl::color(color.r, color.g, color.b);

            glm::vec2 pos = hexToScreenMatrix * glm::vec2(b.first.x, b.first.y) * glm::vec2(diameter);

            gl::vertex(toWindowPos(pos));
            gl::vertex(toWindowPos(pos + rDir));
            gl::vertex(toWindowPos(pos - tDir));

            gl::vertex(toWindowPos(pos));
            gl::vertex(toWindowPos(pos - tDir));
            gl::vertex(toWindowPos(pos + sDir));

            gl::vertex(toWindowPos(pos));
            gl::vertex(toWindowPos(pos + sDir));
            gl::vertex(toWindowPos(pos - rDir));

            gl::vertex(toWindowPos(pos));
            gl::vertex(toWindowPos(pos - rDir));
            gl::vertex(toWindowPos(pos + tDir));

            gl::vertex(toWindowPos(pos));
            gl::vertex(toWindowPos(pos + tDir));
            gl::vertex(toWindowPos(pos - sDir));

            gl::vertex(toWindowPos(pos));
            gl::vertex(toWindowPos(pos - sDir));
            gl::vertex(toWindowPos(pos + rDir));
        }
    }
    gl::end();
}

glm::vec2 CinderApp::toWindowPos(glm::vec2 position) {
    return position * windowSize + windowSize / glm::vec2(2);
}

void CinderApp::fillRandomHexagon() {
    ivec3 pos = glm::ivec3();
    do {
        pos.x = randInt(-hexTiles, hexTiles + 1);
        pos.y = randInt(-hexTiles, hexTiles + 1);
        pos.z = -pos.x - pos.y;
    } while (abs(pos.z) > hexTiles);

    BrainNode node;
    node.pos = pos;
    node.color = glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f));
    node.color = glm::vec3(0, 0, 0);

    grid.setCell(pos, node);
}

glm::ivec3 CinderApp::screenPosToHexPos(glm::vec2 position) {

    glm::vec2 posInHexSpace = glm::inverse(hexToScreenMatrix) * (position / glm::vec2(diameter));

    glm::vec3 newPos;
    newPos.x = posInHexSpace.x;
    newPos.y = posInHexSpace.y;
    newPos.z = -posInHexSpace.x - posInHexSpace.y;

    // How much does the position deviate from a unit coord?
    glm::vec3 roundDelta;
    roundDelta.x = std::abs(round(newPos.x) - newPos.x);
    roundDelta.y = std::abs(round(newPos.y) - newPos.y);
    roundDelta.z = std::abs(round(newPos.z) - newPos.z);

    // Recalculate the axis with the biggest error
    glm::ivec3 nodepos;
    if (roundDelta.z > roundDelta.x && roundDelta.z > roundDelta.y) {
        // Z biggest error
        nodepos.x = round(newPos.x);
        nodepos.y = round(newPos.y);
        nodepos.z = -nodepos.x - nodepos.y;
    } else if (roundDelta.y > roundDelta.x && roundDelta.y > roundDelta.z) {
        // Y biggest error
        nodepos.x = round(newPos.x);
        nodepos.z = round(newPos.z);
        nodepos.y = -nodepos.x - nodepos.z;
    } else {
        // X biggest error
        nodepos.y = round(newPos.y);
        nodepos.z = round(newPos.z);
        nodepos.x = -nodepos.y - nodepos.z;
    }

    return nodepos;
}

// Tell Cinder to actually create and run the application.
CINDER_APP(CinderApp, RendererGl, prepareSettings)
