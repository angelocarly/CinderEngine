#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HexNode.h"
#include "HexField.h"
#include <spdlog/spdlog.h>
#include <stack>

using namespace ci;
using namespace ci::app;

class CinderApp : public App
{
public:

    void setup() override;

    void mouseDrag(MouseEvent event) override;

    void mouseMove(MouseEvent event) override;

    void mouseDown(MouseEvent event) override;

    void mouseWheel(MouseEvent event) override;

    void keyDown(KeyEvent event) override;

    void draw() override;

private:

    HexField hexField;

    vec2 toWindowPos(vec2 position);

    vec2 windowSize;

    vec2 mousePosition;
    vec2 cameraPosition = glm::vec2(0.5f, 0.5f);

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

    ivec3 screenPosToHexCoord(vec2 position);

    void setHexSizes(int hexesOnScreen)
    {
        this->hexTiles = hexesOnScreen;
        this->diameter = 1.0f / hexesOnScreen;
        this->hexRadius = diameter / sqrt(3);

        rDir = hexToScreenMatrixPointyTop * glm::vec2(1, 0) * glm::vec2(hexRadius);
        sDir = hexToScreenMatrixPointyTop * glm::vec2(0, -1) * glm::vec2(hexRadius);
        tDir = hexToScreenMatrixPointyTop * glm::vec2(-1, 1) * glm::vec2(hexRadius);
    }
};


void CinderApp::setup()
{
    windowSize = vec2(getWindowWidth(), getWindowHeight());

    hexField.addStructure(
            0,
            GridUtils::createHexagon(glm::ivec3(2, -2, 0)),
            1
    );
    hexField.addStructure(
            1,
            GridUtils::createLine(glm::ivec3(3, -3, 0)),
            0
    );
}

void prepareSettings(CinderApp::Settings *settings)
{
    settings->setTitle("cinder");
    settings->setWindowSize(900, 900);
    settings->setMultiTouchEnabled(false);
}

void CinderApp::mouseMove(MouseEvent event)
{
    mousePosition = event.getPos();
}

void CinderApp::mouseDown(MouseEvent event)
{
    glm::vec2 mousePos = glm::vec2(event.getPos()) / windowSize - glm::vec2(1.f);

    HexNode node;
    node.pos = screenPosToHexCoord(mousePos + glm::vec2(cameraPosition.x, cameraPosition.y));
//    node.color = glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f));
    node.color = glm::vec3(0.9f);

    hexField.getHexGrid().setCell(node.pos, node);
}


void CinderApp::mouseDrag(MouseEvent event)
{
    glm::vec2 distance = (mousePosition - glm::vec2(event.getPos())) / windowSize;
    cameraPosition += distance;
    mousePosition = event.getPos();
}

void CinderApp::mouseWheel(MouseEvent event)
{
    setHexSizes(hexTiles - event.getWheelIncrement());
}

void CinderApp::keyDown(KeyEvent event)
{
    if (event.getChar() == 'a')
    {
        hexField.calculateNextDepth();
    }
}

void CinderApp::draw()
{
    gl::clear(Color(.02f, 0.02f, 0.05f) * 2.7f);

    gl::pushModelMatrix();
    gl::translate(toWindowPos(-cameraPosition));

    gl::begin(GL_TRIANGLES);
    {

        for (std::pair<glm::ivec3, HexNode> b: hexField.getHexGrid())
        {
            glm::vec3 color = b.second.color;
            glm::ivec3 hexpos = b.second.pos;

            gl::color(color.r, color.g, color.b);

            glm::vec2 pos = hexToScreenMatrix * hexpos * glm::vec2(diameter);

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
    gl::popModelMatrix();
}

glm::vec2 CinderApp::toWindowPos(glm::vec2 position)
{
    return position * windowSize + windowSize / glm::vec2(2);
}

glm::ivec3 CinderApp::screenPosToHexCoord(glm::vec2 position)
{

    glm::vec2 posInHexSpace = glm::inverse(hexToScreenMatrix) * (position / glm::vec2(diameter));

    glm::vec3 newPos;
    newPos.x = -posInHexSpace.x - posInHexSpace.y;
    newPos.y = posInHexSpace.x;
    newPos.z = posInHexSpace.y;

    // How much does the position deviate from a unit coord?
    glm::vec3 roundDelta;
    roundDelta.x = std::abs(round(newPos.x) - newPos.x);
    roundDelta.y = std::abs(round(newPos.y) - newPos.y);
    roundDelta.z = std::abs(round(newPos.z) - newPos.z);

    // Recalculate the axis with the biggest error
    glm::ivec3 nodepos;
    if (roundDelta.z > roundDelta.x && roundDelta.z > roundDelta.y)
    {
        // Z biggest error
        nodepos.x = round(newPos.x);
        nodepos.y = round(newPos.y);
        nodepos.z = -nodepos.x - nodepos.y;
    } else if (roundDelta.y > roundDelta.x && roundDelta.y > roundDelta.z)
    {
        // Y biggest error
        nodepos.x = round(newPos.x);
        nodepos.z = round(newPos.z);
        nodepos.y = -nodepos.x - nodepos.z;
    } else
    {
        // X biggest error
        nodepos.y = round(newPos.y);
        nodepos.z = round(newPos.z);
        nodepos.x = -nodepos.y - nodepos.z;
    }

    return nodepos;
}

// Tell Cinder to actually create and run the application.
CINDER_APP(CinderApp, RendererGl, prepareSettings)
