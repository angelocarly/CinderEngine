#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HexNode.h"
#include "HexField.h"
#include "cinder/Rand.h"
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

    gl::GlslProgRef mGlsl;
    float colorpos = 0;
    int colorscale = 60;
    glm::vec3 color1 = glm::vec3(1.0f, 1.0f, 0.5f);
    glm::vec3 color2 = glm::vec3(0.8f, 0.9f, 0.3f);
    glm::vec3 color3 = glm::vec3(0.5f);
    glm::vec3 color4 = glm::vec3(0.5f);

    Rand r = Rand();

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

    vec3 colorGradient(int depth, int max_depth, float colorvalue);

    vec3 palette(float t, vec3 a, vec3 b, vec3 c, vec3 d);

    float maxColorValue = 0.0f;
    int maxDepth = 0;
};


void CinderApp::setup()
{
    windowSize = vec2(getWindowWidth(), getWindowHeight());

    hexField.addStructure(
            GridUtils::createTriangle(glm::ivec3(-5, 6, -1)),
            {1, 1, 1}
    );
    hexField.addStructure(
            GridUtils::createTriangleUp(glm::ivec3(-1, 1, -0), GridUtils::rotateThirdRight()),
            {1, 1, 1}
    );
//    hexField.addStructure(
//            GridUtils::createTriangleUp(glm::ivec3(-1, 1, -0), GridUtils::rotateSixthRight()),
//            {1, 2, 2}
//    );
//    hexField.addStructure(
//            GridUtils::createHexagon(glm::ivec3(-5, 5, -0)),
//            {2, -1, 2, -1, -1, 2}
//    );
//    hexField.addStructure(
//            GridUtils::createTriangleUp(glm::ivec3(1, 1, -2)),
//            {0, 2, -3}
//    );
    hexField.addStructure(
            GridUtils::createLine(glm::ivec3(-6, -7, 13)),
            {1, 2}
    );
    hexField.addStructure(
            GridUtils::createTriangleUp(glm::ivec3(-4, 3, 1), GridUtils::rotateThirdRight()),
            {-2, 1, 0}
    );
//    hexField.addStructure(
//             GridUtils::createTriangleUp(glm::ivec3(2, -1, -1)),
//            {1, 1, 1}
//    );
    hexField.addStructure(
            GridUtils::createTriangleUp(glm::ivec3(2, -2, 0)),
            {1, 1, -4}
    );
    hexField.addStructure(
            GridUtils::createTriangleUp(glm::ivec3(1, -1, 0) ),
            {0, 0, 0}
    );

}

void prepareSettings(CinderApp::Settings *settings)
{
    settings->setTitle("cinder");
    settings->setWindowSize(1280, 1280);
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
    if (event.getChar() == 'q')
    {
        hexField.calculateNextDepth();
    }
    if (event.getChar() == 'w')
    {
        colorpos += 0.2f / 100;
    }
    if (event.getChar() == 's')
    {
        colorpos -= 0.2f / 100;
    }
    if (event.getChar() == 'a')
    {
        colorscale += 1;
    }
    if (event.getChar() == 'd')
    {
        colorscale -= 1;
        if (colorscale < 1) colorscale = 1;
    }
    if (event.getChar() == 'r')
    {
        colorpos = randFloat(0.0f, 2.0f);
        color1 = glm::vec3(randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f));
        color2 = glm::vec3(randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f));
    }
    if (event.getChar() == 't')
    {
        color3 = glm::vec3(randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f));
        color4 = glm::vec3(randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f), randFloat(0.0f, 1.0f));
    }
}

glm::vec3 CinderApp::palette(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
{
    return a + b * cos(glm::vec3(6.28318) * (c * t + d));
}

glm::vec3 CinderApp::colorGradient(int depth, int max_depth, float colorvalue)
{
    float scale = 0.9f - depth * depth / 180.0f;
    if (scale < .2f) scale = .2f;
    if (scale > 1.0f) scale = 1.0f;
    float in = colorvalue / colorscale + 1.34f + colorpos;
//    spdlog::info("colorvalue: {}, in: {}", colorvalue, in);
//    return scale * palette(in, glm::vec3(.6f, .6f, .6f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 0.5f),
//                           glm::vec3(.8f, .9f, .3f));
//    return palette(in, glm::vec3(.45f, .6f, .6f), glm::vec3(.55f, 0.4f, .4f), glm::vec3(0.4f, 0.9f, 1.0f), glm::vec3(1.0f, .6f, .1f));
//    return scale * palette(in, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.5f, .2f, .25f));
//    return scale * palette(in, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(0.8f, .9f, .3f)); //nico
    return scale * palette(in, color3, color4, color1, color2); //nico
//    return (1 - scale) * palette(colorvalue, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(.5f, .20f, .25f));
//    return palette(in, glm::vec3(.5f), glm::vec3(.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(.3f, .2f, .2f));
}

void CinderApp::draw()
{

    vec3 bgColor = .4f * colorGradient(maxDepth, 15, maxColorValue);
    gl::clear(Color(bgColor.r, bgColor.g, bgColor.b));

    gl::pushModelMatrix();
    gl::translate(toWindowPos(-cameraPosition));
//    gl::rotate(-colorpos);

    gl::begin(GL_TRIANGLES);
    {

        for (std::pair<glm::ivec3, HexNode> b: hexField.getHexGrid())
        {
            glm::vec3 color = colorGradient(b.second.depth, 20, b.second.colorValue);
            if (b.second.colorValue > maxColorValue)
            {
                maxColorValue = b.second.colorValue;
                maxDepth = b.second.depth;
            }

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
