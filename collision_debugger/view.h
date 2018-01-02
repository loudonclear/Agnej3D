#ifndef VIEW_H
#define VIEW_H

#include <qgl.h>
#include <GL/glu.h>
#include <QTime>
#include <QTimer>
#include "triangle.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime time;
    QTimer timer;

    // Members for player movement
    glm::vec2 angles;
    glm::vec3 cameraCenter;

    // Members for handling dragging
    glm::vec3 *dragCenter;
    glm::vec3 offset;
    glm::vec4 plane;
    bool draggingCamera;
    int mouseX, mouseY;

    // TODO: this is intentionally bad design, replace this with your own ellipse structure
    enum
    {
        ELLIPSOID_START,
        ELLIPSOID_END,
        ELLIPSOID_HIT,
        ELLIPSOID_RESULT,

        // This counts the number of enums above it
        NUM_ELLIPSOIDS
    };
    glm::vec3 centers[NUM_ELLIPSOIDS];
    glm::vec3 radius;
    GLUquadric *quad;

    // TODO: this is intentionally bad design, replace this with your own triangle structure
    //glm::vec3 triA, triB, triC;

    QList<Triangle*> triangles;

    void drawScene();
    void drawShadowVolumes();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    static inline glm::vec3 lerp(const glm::vec3 &a, const glm::vec3 &b, float fraction) { return a + (b - a) * fraction; }

private slots:
    void tick();
};

#endif // VIEW_H

