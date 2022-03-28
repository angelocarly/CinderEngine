#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "HexGrid.h"

using namespace ci;
using namespace ci::app;

class CinderApp : public App {
  public:

    void setup() override;

	void mouseDrag( MouseEvent event ) override;

	void keyDown( KeyEvent event ) override;

	void draw() override;

  private:

    HexGrid<glm::vec3> grid = HexGrid<glm::vec3>(3);
};

void CinderApp::setup()
{
}

void prepareSettings(CinderApp::Settings* settings )
{
    settings->setTitle("cinder");
    settings->setWindowSize(800, 800);
    settings->setMultiTouchEnabled( false );
}

void CinderApp::mouseDrag(MouseEvent event )
{
}

void CinderApp::keyDown(KeyEvent event )
{
}

void CinderApp::draw()
{
	gl::clear( Color::gray( 0.1f ) );

    vec2 windowSize = vec2(getWindowWidth(), getWindowHeight());

	gl::begin( GL_POINTS );
    {
        glm::vec3 hexpos = glm::ivec3(0, 0, 0);
        glm::vec3 color = grid.getHexagon(hexpos);

        int radius = 15;
        float size = .15f / radius;
        float height = 2 * size * 3 / 4.0f;
        glm::vec2 rDir = glm::vec2(-height * sqrt(3) / 2, height / 2);
        glm::vec2 sDir = glm::vec2(0, -height);
        glm::vec2 tDir = glm::vec2(height * sqrt(3) / 2, height / 2);

        for (int r = -radius; r <= radius; r++)
        {
            for (int s = -radius; s <= radius; s++)
            {
                for (int t = -radius; t <= radius; t++)
                {
                    if (r + s + t != 0) continue;

                    glm::vec2 pos = rDir * glm::vec2(r) + sDir * glm::vec2(s) + tDir * glm::vec2(t);

                    gl::vertex(pos * windowSize + windowSize / glm::vec2(2));
                }
            }
        }
    }
	gl::end();
}

// Tell Cinder to actually create and run the application.
CINDER_APP(CinderApp, RendererGl, prepareSettings )
