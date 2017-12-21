#ifndef VIEW_H
#define VIEW_H

#include "util/CommonIncludes.h"
#include <QGLWidget>
#include <QTime>
#include <QTimer>

#define FRAMES_TO_AVERAGE 30

/**
 * This is similar to your "CS1971FrontEnd" class. Here you will receive all of the input events
 * to forward to your game.
 *
 * @brief The View class
 */
class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QWidget *window;

    QTime time;
    QTimer timer;

    float fps;
    int frameIndex;
    float frameTimes[FRAMES_TO_AVERAGE];

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void tick();
};

#endif // VIEW_H

