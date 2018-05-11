#ifndef SPRINGSYSTEM_H
#define SPRINGSYSTEM_H

#include "springnode.h"
#include "edge.h"
#include "cell.h"
#include <OpenGL/glu.h>

class SpringSystem
{
public:
    SpringSystem();
    void build(SpringNode LBNode, SpringNode RBNode, SpringNode RUNode, SpringNode LUNode, int widthSubdivisions, int heightSubdivisions);
    void setSpringConstant(double k);
    double getSpringConstant();
    void draw(GLenum drawType);
    QList<SpringNode> *getSpringNodeList();
private:
    void makeEdges(int widthSub, int heightSub);
    void makeCells(int heightSub);
    QList<SpringNode> springNodeList;
    QList<Edge> edgeList;
    QList<Cell> cellList;
    double springConstant;


};

#endif // SPRINGSYSTEM_H
