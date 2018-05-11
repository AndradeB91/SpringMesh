#include "GLWidget.h"

#include <QtGui>
#include <iostream>
#include "trackball.h"
#include <OpenGL/glu.h>

#define GL_POINTS           0x0000
#define GL_LINES            0x0001
#define GL_LINE_LOOP        0x0002
#define GL_LINE_STRIP       0x0003
#define GL_TRIANGLES        0x0004
#define GL_TRIANGLE_STRIP   0x0005
#define GL_TRIANGLE_FAN     0x0006

#define SPRING_TOLERANCE (0.0000000005)

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer  | QGL::SampleBuffers | QGL::StencilBuffer), parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start();

    zoomDepth = 150.0;

    trackball(curquat, 0.0, 0.0, 0.0, 0.0);
    clipClose = 0.001;
    fovy = 60.0;
    show_grid = true;
    perspective = true;

    initializeMesh();

    drawType = GL_TRIANGLES;
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPointSize(4);
    if(drawType == GL_TRIANGLES){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
}

float t=10;
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    drawBackground();

    if(perspective)
        glTranslatef(0.0, 0.0, -zoomDepth);


    GLfloat m[4][4];
    build_rotmatrix(m, curquat);
    glMultMatrixf(&m[0][0]);

    drawAxis(&m[0][0]);

    //drawGrid();

    glColor3f(1,1,0);
    double _energyLoss = 0.09;
    vec3 gravity(0,-9.81*100,0);

    QList<vec3> auxPos;

    for(int i=0; i<_springSystem.getSpringNodeList()->size(); i++){
        if(!itSpringNodes[i].isNodeAnchor()){

            vec3 resultingForce = (float)0.04*gravity;

            for(int f=0; f< itSpringNodes[i].getNeighbors().size(); f++){
                vec3 forceDirection = normalize(itSpringNodes[i].getNeighbors()[f]->getPosition() - itSpringNodes[i].getPosition());
                double deltaX   = glm::distance(itSpringNodes[i].getNeighbors()[f]->getPosition(), itSpringNodes[i].getPosition()) - itSpringNodes[i].getNeighborDistance()[f];
                forceDirection *= _springSystem.getSpringConstant()*deltaX;
                forceDirection -= (float)1200.0*(itSpringNodes[i].getVelocity() - itSpringNodes[i].getNeighbors()[f]->getVelocity());
                resultingForce += forceDirection;
            }
            resultingForce = normalize(resultingForce);
            vec3 newVelocity = itSpringNodes[i].getVelocity() + resultingForce*(float)_energyLoss;
            itSpringNodes[i].setVelocity(newVelocity);
            auxPos << itSpringNodes[i].getPosition() + itSpringNodes[i].getVelocity();
        }
    }
    int k = 0;
    for(int i=0; i<_springSystem.getSpringNodeList()->size(); i++){
        if(!itSpringNodes[i].isNodeAnchor()){
            itSpringNodes[i].setPosition(auxPos[k]);
            k++;
        }
    }

    _springSystem.draw(drawType);

}


void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    loadProjection();

    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        beginx = event->x();
        beginy = event->y();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float width = size().width();
    float height = size().height();

    trackball(lastquat,
      (2.0 * beginx - width) / width,
      (height - 2.0 * beginy) / height,
      (2.0 * event->x() - width) / width,
      (height - 2.0 * event->y()) / height
    );
    beginx = event->x();
    beginy = event->y();
    add_quats(lastquat, curquat, curquat);
    updateGL();

}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    float ds = event->delta()*0.01;
    zoomDepth -= ds;
    zoomDepth = zoomDepth < 0.0 ? 0.0 : zoomDepth;
    updateGL();
}

void GLWidget::drawGrid()
{
    static struct Grid
    {
        float l;
        int res;
        float d;
        float* vertices;

        Grid()
        :   l(100.0),
            res(71),
            d(l/float(res-1)),
            vertices(new float[res*res*2])
        {
            bool forward = true;
            for(int j=0; j<res; j++)
            {
                for(int i=0; i<res; i++)
                {
                    if(forward)
                        vertices[i*2 + j*res*2] = i*d - l*0.5;
                    else
                        vertices[i*2 + j*res*2] = (res-i-1)*d - l*0.5;

                    vertices[i*2 + j*res*2 + 1] = j*d - l*0.5;
                }

                forward = !forward;
            }
        }
    } grid;

    glPushAttrib(GL_LIGHTING_BIT);
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glRotatef(90,1,0,0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glColor3f(0.4, 0.4, 0.4);
    glVertexPointer(2, GL_FLOAT, 0, reinterpret_cast<void*>(grid.vertices));
    glDrawArrays(GL_LINE_STRIP, 0, grid.res*grid.res);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glDrawArrays(GL_LINE_STRIP, 0, grid.res*grid.res);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopAttrib();
    glPopClientAttrib();
    glPopMatrix();
}

void GLWidget::drawAxis(GLfloat *mat)
{
    static struct Axis
    {
        Axis()
        {
            id = glGenLists(1);

            glNewList(id, GL_COMPILE);

                glLineWidth(2);
                glBegin(GL_LINES);
                    glColor3f(1.0, 0.9, 0.9);
                    glVertex3f(0.0, 0.0, 0.0);
                    glColor3f(0.8, 0.0, 0.0);
                    glVertex3f(1.0, 0.0, 0.0);

                    glColor3f(0.9, 1.0, 0.9);
                    glVertex3f(0.0, 0.0, 0.0);
                    glColor3f(0.0, 0.8, 0.0);
                    glVertex3f(0.0, 1.0, 0.0);

                    glColor3f(0.9, 0.9, 1.0);
                    glVertex3f(0.0, 0.0, 0.0);
                    glColor3f(0.0, 0.0, 0.8);
                    glVertex3f(0.0, 0.0, 1.0);
                glEnd();

            glEndList();
        }

        int id;

    } axis;

    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT | GL_LINE_BIT);
    glDisable( GL_LIGHTING );

    glViewport(0, 0, 80.0, 80.0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho (-1.5, 1.5, -1.5, 1.5, -1.199, 1.199);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(mat);

    glCallList(axis.id);

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

void GLWidget::drawBackground()
{
    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
        glColor3f(0.22, 0.22, 0.22);
        glVertex2f(-1.f, -1.f);
        glVertex2f(1.f, -1.f);

        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(1.f, 1.f);
        glVertex2f(-1.f, 1.f);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

void GLWidget::loadProjection()
{
    float w = width();
    float h = height();
    float ratio = w/h;

    if(perspective)
    {
        gluPerspective(fovy, ratio, clipClose, 20000);
    }
}


void GLWidget::initializeMesh(){
    SpringNode LBNode;
    LBNode.setPosition(vec3(-40,50,40));
    SpringNode RBNode;
    RBNode.setPosition(vec3(40,50,40));
    SpringNode RUNode;
    RUNode.setPosition(vec3(40,50,-40));
    SpringNode LUNode;
    LUNode.setPosition(vec3(-40,50,-40));

    _springSystem.getSpringNodeList()->clear();

    _springSystem.build(LBNode,RBNode,RUNode,LUNode,50,50);
    _springSystem.setSpringConstant(5000);

    itSpringNodes = _springSystem.getSpringNodeList()->begin();
}

void GLWidget::restart(){
    initializeMesh();
    //_springSystem.getSpringNodeList()->clear();

}

