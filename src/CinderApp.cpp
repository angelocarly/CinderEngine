#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HexGrid.h"
#include "cinder/Rand.h"

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

    HexGrid<glm::vec3> grid = HexGrid<glm::vec3>();

    vec2 toWindowPos(vec2 position);

    vec2 windowSize;
    int hexTiles = 10;
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
        pos.x = randInt(-hexTiles, hexTiles + 1);
        pos.y = randInt(-hexTiles, hexTiles + 1);
        pos.z = -pos.x - pos.y;
        grid.setCell(pos, glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f)));
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
            pos.x = randInt(-hexTiles, hexTiles + 1);
            pos.y = randInt(-hexTiles, hexTiles + 1);
            pos.z = -pos.x - pos.y;
            grid.setCell(pos, glm::vec3(randFloat(1.0f), randFloat(1.0f), randFloat(1.0f)));
        }
    }
}

void CinderApp::draw()
{
    gl::clear(Color::gray(0.1f));


    gl::begin(GL_TRIANGLES);
    {

        // TODO: Replace loop with iteration over hexgrid
        for (int r = -hexTiles; r <= hexTiles; r++)
        {
            for (int s = -hexTiles; s <= hexTiles; s++)
            {
                for (int t = -hexTiles; t <= hexTiles; t++)
                {
                    if (r + s + t != 0) continue;
                    if (!grid.hasCell(glm::ivec3(r, s, t))) continue;

                    glm::vec3 color = grid.getCell(r, s, t);

                    gl::color(color.r, color.g, color.b);

                    glm::vec2 pos = rDir * glm::vec2(r) + sDir * glm::vec2(s) + tDir * glm::vec2(t);

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
