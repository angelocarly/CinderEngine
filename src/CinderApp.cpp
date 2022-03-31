#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HexGrid.h"
#include "cinder/Rand.h"
#include "BrainNode.h"

using namespace ci;
using namespace ci::app;

class CinderApp : public App
{
public:

    void setup() override;

    void mouseDrag(MouseEvent event) override;

    void mouseDown( MouseEvent event ) override;

    void keyDown(KeyEvent event) override;

    void draw() override;

private:

    HexGrid<BrainNode> grid = HexGrid<BrainNode>();

    vec2 toWindowPos(vec2 position);

    vec2 windowSize;
    int hexTiles = 200;
    float hexRadius = .15f / hexTiles;
    float height = 2 * hexRadius * 3 / 4.0f;

    // hex 'normals'
    glm::vec2 rDir = glm::vec2(-height * sqrt(3) / 2, height / 2);
    glm::vec2 sDir = glm::vec2(0, -height);
    glm::vec2 tDir = glm::vec2(height * sqrt(3) / 2, height / 2);
};

void CinderApp::setup()
{
    windowSize = vec2(getWindowWidth(), getWindowHeight());
    for (int i = 0; i < 30; i++)
    {
        ivec3 pos = glm::ivec3();
        do {
            pos.x = randInt(-hexTiles, hexTiles + 1);
            pos.y = randInt(-hexTiles, hexTiles + 1);
            pos.z = -pos.x - pos.y;
        } while (abs(pos.z) > hexTiles);

        BrainNode node;
        node.pos = pos;
        node.color = glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f));

        grid.setCell(pos, node);
    }
}

void prepareSettings(CinderApp::Settings *settings)
{
    settings->setTitle("cinder");
    settings->setWindowSize(800, 800);
    settings->setMultiTouchEnabled(false);
}

void CinderApp::mouseDown(MouseEvent event)
{
    glm::vec2 mousePos = glm::vec2(event.getPos()) / windowSize;
    glm::vec2 delta = mousePos * rDir;
}


void CinderApp::mouseDrag(MouseEvent event)
{
}

void CinderApp::keyDown(KeyEvent event)
{
    if (event.getChar() == 'a')
    {
        for (int i = 0; i < 30; i++)
        {
            ivec3 pos = glm::ivec3();
            do {
                pos.x = randInt(-hexTiles, hexTiles + 1);
                pos.y = randInt(-hexTiles, hexTiles + 1);
                pos.z = -pos.x - pos.y;
            } while (abs(pos.z) > hexTiles);

            BrainNode node;
            node.pos = pos;
            node.color = glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f));

            grid.setCell(pos, node);
        }
    }
}

void CinderApp::draw()
{
    gl::clear(Color::gray(0.1f));


    gl::begin(GL_TRIANGLES);
    {

        for (std::pair<glm::ivec3, BrainNode> b : grid)
        {
                    glm::vec3 color = b.second.color;

                    gl::color(color.r, color.g, color.b);

                    glm::vec2 pos = rDir * glm::vec2(b.first.x) + sDir * glm::vec2(b.first.y) + tDir * glm::vec2(b.first.z);

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

glm::vec2 CinderApp::toWindowPos(glm::vec2 position)
{
    return position * windowSize + windowSize / glm::vec2(2);
}

// Tell Cinder to actually create and run the application.
CINDER_APP(CinderApp, RendererGl, prepareSettings)
