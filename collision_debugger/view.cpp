#include "view.h"
#include "viewformat.h"
#include "raytracer.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include <memory>

#include "Transform.h"
#include "collision/Collision.h"
#include "collision/TriangleShape.h"
#include "collision/SphereShape.h"
#include "RigidBody.h"

static void drawWireCube(const glm::vec3 &min, const glm::vec3 &max)
{
    float vertices[8][3];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            vertices[i][j] = (i & (1 << j) ? max : min)[j];
        }
    }
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    for (int i = 0; i < 8; i++) glVertex3fv(vertices[i]);
    for (int i = 0; i < 8; i++) glVertex3fv(vertices[(i & 4) | ((i & 2) >> 1) | ((i & 1) << 1)]);
    for (int i = 0; i < 8; i++) glVertex3fv(vertices[((i & 4) >> 2) | (i & 2) | ((i & 1) << 2)]);
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

static void drawArrow(const glm::vec3 &start, const glm::vec3 &end, const glm::vec3 &eye)
{
    glm::vec3 axisA = glm::normalize((end - start)) * 0.05f;
    glm::vec3 axisB = glm::normalize(glm::cross((start - eye), (axisA))) * 0.05f;
    glColor4f(0, 0, 0, 0.25);
    glDisable(GL_DEPTH_TEST);
    const float s = 3.5;
    for (int i = 0; i < 2; i++) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex3fv(glm::value_ptr(end));
        glVertex3fv(glm::value_ptr(end - axisB * s - axisA * s));
        glVertex3fv(glm::value_ptr(end - axisB - axisA * s));
        glVertex3fv(glm::value_ptr(start - axisB));
        glVertex3fv(glm::value_ptr(start + axisB));
        glVertex3fv(glm::value_ptr(end + axisB - axisA * s));
        glVertex3fv(glm::value_ptr(end + axisB * s - axisA * s));
        glEnd();
        glEnable(GL_DEPTH_TEST);
        glColor3f(0, 0, 0);
    }
}

static void drawCursor(const glm::vec3 &center, const glm::vec3 &normal)
{
    glm::vec3 axisX = glm::normalize(glm::cross(glm::vec3(1, 1, 1), normal)) * 0.2f;
    glm::vec3 axisY = glm::cross(normal, glm::normalize(axisX)) * 0.2f;
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; i++) {
        // Draw one head of the arrow
        glVertex3fv(glm::value_ptr(center + axisX * 0.5f + axisY * 0.8f));
        glVertex3fv(glm::value_ptr(center + axisX * 0.3f + axisY));
        glVertex3fv(glm::value_ptr(center + axisX + axisY));
        glVertex3fv(glm::value_ptr(center + axisX + axisY * 0.3f));
        glVertex3fv(glm::value_ptr(center + axisX * 0.8f + axisY * 0.5f));
        glVertex3fv(glm::value_ptr(center + axisX * 0.3f));

        // Rotate 90 degrees
        glm::vec3 temp = axisX;
        axisX = -axisY;
        axisY = temp;
    }
    glEnd();
}

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent), dragCenter(NULL),
    draggingCamera(false), radius(0.5f, 1, 0.5f), quad(NULL)
{
    /** SUPPORT CODE START **/
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    /** SUPPORT CODE END **/
}

View::~View()
{
}

void View::initializeGL()
{
    /** SUPPORT CODE START **/
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    quad = gluNewQuadric();

    // Add a directional light
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set up test triangles
    glm::vec3 triA = glm::vec3(0,0,0);
    glm::vec3 triB = glm::vec3(10,0,10);
    glm::vec3 triC = glm::vec3(0, 0, 10);

    triangles.push_back(new Triangle(triA, triC, triB));

    // TODO [Week 2]: Uncomment for collision response
    triB = glm::vec3(-4, 3, 5);
    triangles.push_back(new Triangle(triA, triB, triC));

    triC = glm::vec3(-4, 0, 0);
    triangles.push_back(new Triangle(triA, triC, triB));

    // Acute angle triangle, you don't need to do this
    triA = glm::vec3(10,0,10);
    triB = glm::vec3(0, 0, 10);
    triC = glm::vec3(5, 3, 8);
    triangles.push_back(new Triangle(triA, triB, triC));

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    timer.start(1000 / 60);
    /** SUPPORT CODE END **/

    //0.767049
    centers[ELLIPSOID_START] = glm::vec3(0.310609f, 4.932258f, 0.f);
    centers[ELLIPSOID_END] = glm::vec3(-0.664281f, -0.325403f, 14.731380f);
}

void View::drawScene()
{
    // Draw ellipsoids
    for (int i = 0; i < NUM_ELLIPSOIDS; i++) {
        const glm::vec3 &center = centers[i];
        glColor3f(1, 0.5, 0);
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glScalef(radius.x, radius.y, radius.z);
        switch(i) {
        case ELLIPSOID_START:
            glColor3f(1, 0, 0);
            break;
        case ELLIPSOID_HIT:
        case ELLIPSOID_RESULT:
            glColor3f(1, 0.5, 0);
            break;
        case ELLIPSOID_END:
            glColor3f(0, 1, 0);
            break;
        }
        gluSphere(quad, 1, 32, 16);
        if (i == ELLIPSOID_START || i == ELLIPSOID_END) {
            glColor3f(0, 0, 0);
            drawWireCube(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
        }
        glPopMatrix();
    }

    // Draw plane using polygon offset to avoid drawing on grid lines
    glColor3f(0.75, 0.75, 0.75);
    glNormal3f(0, 1, 0);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.5, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0, +100);
    glVertex3f(+100, 0, +100);
    glVertex3f(+100, 0, -100);
    glEnd();
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw triangles
    for (int i = 0; i < triangles.size(); i++){
        Triangle *t = triangles.at(i);
        glColor3f(0, 1, 0);
        glNormal3fv(glm::value_ptr(t->normal));
        glBegin(GL_TRIANGLES);
        glVertex3fv(glm::value_ptr(t->vertices[0]));
        glVertex3fv(glm::value_ptr(t->vertices[1]));
        glVertex3fv(glm::value_ptr(t->vertices[2]));
        glEnd();
    }
}

void View::drawShadowVolumes()
{
    /** SUPPORT CODE START **/
    glm::vec3 down(0, -100, 0);

    // Shadow volumes for ellipsoids (sides then caps)
    for (int i = 0; i < NUM_ELLIPSOIDS; i++) {
        const glm::vec3 &center = centers[i];
        glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glScalef(radius.x, radius.y, radius.z);
        glRotatef(90, 1, 0, 0);
        gluCylinder(quad, 1, 1, down.length(), 32, 1);
        glRotatef(180, 1, 0, 0);
        gluDisk(quad, 0, 1, 32, 1);
        glTranslatef(0, 0, -down.length());
        glRotatef(180, 1, 0, 0);
        gluDisk(quad, 0, 1, 32, 1);
        glPopMatrix();
    }

    for (int i = 0; i < triangles.size(); i++){
        Triangle *t = triangles.at(i);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex3fv(glm::value_ptr(t->vertices[0] + down));
        glVertex3fv(glm::value_ptr(t->vertices[0]));
        glVertex3fv(glm::value_ptr(t->vertices[1] + down));
        glVertex3fv(glm::value_ptr(t->vertices[1]));
        glVertex3fv(glm::value_ptr(t->vertices[2] + down));
        glVertex3fv(glm::value_ptr(t->vertices[2]));
        glVertex3fv(glm::value_ptr(t->vertices[0] + down));
        glVertex3fv(glm::value_ptr(t->vertices[0]));
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex3fv(glm::value_ptr(t->vertices[0]));
        glVertex3fv(glm::value_ptr(t->vertices[2]));
        glVertex3fv(glm::value_ptr(t->vertices[1]));
        glVertex3fv(glm::value_ptr(t->vertices[0] + down));
        glVertex3fv(glm::value_ptr(t->vertices[1] + down));
        glVertex3fv(glm::value_ptr(t->vertices[2] + down));
        glEnd();
    }
    /** SUPPORT CODE END **/
}

void View::paintGL()
{
    // TODO: Replace this with your code that calculates the intersection point
    // from collision detection and (in week 2) the resulting location from
    // collision response

    std::shared_ptr<SphereShape> ellipsoid = std::make_shared<SphereShape>();
    std::shared_ptr<Transform> etrans = std::make_shared<Transform>();
    etrans->setPosition(centers[ELLIPSOID_START]);
    etrans->setScale(radius);
    float lambda = FLT_MAX;

    for (Triangle* tri : triangles) {
        std::shared_ptr<TriangleShape> trishape = std::make_shared<TriangleShape>(tri->vertices[0], tri->vertices[1], tri->vertices[2]);

        float test = Collision::continuousConvexCollision(ellipsoid, etrans, trishape, centers[ELLIPSOID_START], centers[ELLIPSOID_END]);
        if (test >= 0.f && test < lambda) {
            lambda = test;
        }
    }
    if (lambda >= 0.f && lambda <= 1.f) {
        centers[ELLIPSOID_HIT] = centers[ELLIPSOID_START] + lambda*(centers[ELLIPSOID_END] - centers[ELLIPSOID_START]);
    } else {
        centers[ELLIPSOID_HIT] = centers[ELLIPSOID_END];
    }

    //centers[ELLIPSOID_HIT] = (centers[ELLIPSOID_START] + centers[ELLIPSOID_END]) / 2.f;
    centers[ELLIPSOID_RESULT] = centers[ELLIPSOID_END];

    /** SUPPORT CODE START **/

    // Set up the camera
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0, 0, -10);
    glRotatef(-angles.y * 180 / M_PI, 1, 0, 0);
    glRotatef(90 + angles.x * 180 / M_PI, 0, 1, 0);
    glTranslatef(-cameraCenter.x, -cameraCenter.y, -cameraCenter.z);

    // Set up the light to come from straight above
    const float position[4] = { 0, 1, 0, 0 };
    const float ambient[4] = { 0.25, 0.25, 0.25, 0 };
    const float zero[4] = { 0, 0, 0, 0 };
    const float one[4] = { 1, 1, 1, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    // Pass 1 of stenciled shadow volumes: draw scene lit with ambient light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, zero);
    drawScene();

    // Pass 2 of stenciled shadow volumes: set stencil buffer to be non-zero in areas of shadow
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, -1);

    // Disable drawing to the color and depth buffers
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    // Increment stencil buffer for front faces, but only when they fail the depth test
    glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
    glCullFace(GL_FRONT);
    drawShadowVolumes();
    glCullFace(GL_BACK);

    // Decrement stencil buffer for back faces, but only when they fail the depth test
    glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
    drawShadowVolumes();

    // Enable drawing to the color and depth buffers
    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Pass 3 of stenciled shadow volumes: draw scene lit with diffuse light
    glDepthFunc(GL_EQUAL);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, one);
    glStencilFunc(GL_EQUAL, 0, -1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    drawScene();

    glDisable(GL_STENCIL_TEST);
    glDepthFunc(GL_LESS);

    // Lines are partially transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw grid
    glColor4f(0, 0, 0, 0.25);
    glBegin(GL_LINES);
    for (int s = 20, i = -s; i <= s; i++) {
        glVertex3f(i, 0, -s);
        glVertex3f(i, 0, +s);
        glVertex3f(-s, 0, i);
        glVertex3f(+s, 0, i);
    }
    glEnd();

    // Draw arrows
    Raytracer tracer;
    glm::vec3 eye = tracer.getEye();
    drawArrow(centers[ELLIPSOID_START], centers[ELLIPSOID_HIT], eye);
    drawArrow(centers[ELLIPSOID_HIT], centers[ELLIPSOID_RESULT], eye);

    // Draw cursor
    HitTest result;
    glm::vec3 ray = tracer.getRayForPixel(mouseX, mouseY);
    Raytracer::hitTestCube(centers[0] - radius, centers[0] + radius, eye, ray, result);
    Raytracer::hitTestCube(centers[1] - radius, centers[1] + radius, eye, ray, result);
    if (result.t < INFINITY) {
        drawCursor(result.hit, result.normal);
    }

    glDisable(GL_BLEND);

    /** SUPPORT CODE END **/
}

void View::resizeGL(int w, int h)
{
    /** SUPPORT CODE START **/

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w / (float)h, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);

    /** SUPPORT CODE END **/
}

void View::mousePressEvent(QMouseEvent *event)
{
    /** SUPPORT CODE START **/

    // Remember the mouse position so we can calculate deltas
    mouseX = event->x();
    mouseY = event->y();

    if (event->button() == Qt::LeftButton) {
        // Raytrace to both AABBs
        HitTest result;
        Raytracer tracer;
        glm::vec3 eye = tracer.getEye();
        glm::vec3 ray = tracer.getRayForPixel(mouseX, mouseY);
        if (Raytracer::hitTestCube(centers[ELLIPSOID_START] - radius, centers[ELLIPSOID_START] + radius, eye, ray, result)) {
            dragCenter = &centers[ELLIPSOID_START];
        } else if (Raytracer::hitTestCube(centers[ELLIPSOID_END] - radius, centers[ELLIPSOID_END] + radius, eye, ray, result)) {
            dragCenter = &centers[ELLIPSOID_END];
        } else {
            dragCenter = NULL;
        }

        // Set up dragging
        if (dragCenter) {
            plane = glm::vec4(result.normal, glm::dot(-1.f * result.normal, result.hit));
            offset = *dragCenter - result.hit;
        } else {
            draggingCamera = true;
        }
    }

    /** SUPPORT CODE END **/
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    /** SUPPORT CODE START **/

    // Get the change in mouse position from last time
    int deltaX = event->x() - mouseX;
    int deltaY = event->y() - mouseY;
    mouseX = event->x();
    mouseY = event->y();

    // Either move an ellipse or rotate the camera
    if (dragCenter) {
        Raytracer tracer;
        glm::vec3 eye = tracer.getEye();
        glm::vec3 ray = tracer.getRayForPixel(mouseX, mouseY);
        float t = glm::dot(-1.f * glm::vec4(eye, 1), (plane)) / glm::dot(glm::vec4(ray, 0), (plane));
        *dragCenter = eye + ray * t + offset;
    } else if (draggingCamera) {
        angles.x += deltaX * 0.01;
        angles.y -= deltaY * 0.01;
        angles.y = fmaxf(-M_PI / 2, fminf(M_PI / 2, angles.y));
    }

    /** SUPPORT CODE END **/
}

void View::mouseReleaseEvent(QMouseEvent *)
{
    /** SUPPORT CODE START **/

    dragCenter = NULL;
    draggingCamera = false;

    /** SUPPORT CODE END **/
}

void View::keyPressEvent(QKeyEvent *event)
{
    /** SUPPORT CODE START **/

    if (event->key() == Qt::Key_Escape) QApplication::quit();

    /** SUPPORT CODE END **/
}

void View::keyReleaseEvent(QKeyEvent *)
{
}



void View::tick()
{
    /** SUPPORT CODE START **/

    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001;

    // Move the camera towards the average center
    if (!dragCenter) {
        glm::vec3 average = (centers[ELLIPSOID_START] + centers[ELLIPSOID_END]) / 2.f;
        cameraCenter = View::lerp(cameraCenter, average, 1 - powf(0.01, seconds));
    }

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();

    /** SUPPORT CODE END **/
}
