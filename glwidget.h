#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <OpenGL/glu.h>
#include <OpenGL.h>
#include <QGLWidget>
#include <list>
#include <QListIterator>
#include <QTimer>


#include "springsystem.h"

using namespace std;

class Scene;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

public slots:
    void restart();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

    void initializeMesh();

private:
    void drawGrid();
    void drawAxis(GLfloat *mat);
    void drawBackground();
    void loadProjection();

    QTimer timer;

    bool show_grid;
    bool perspective;

    float zoomDepth;
    float curquat[4];
    float lastquat[4];
    int beginx, beginy;
    float clipClose;
    float fovy;
    GLenum drawType;

    //spring related
    SpringSystem _springSystem;
    QList<SpringNode>::iterator itSpringNodes;
};

#endif
