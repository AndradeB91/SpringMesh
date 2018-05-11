    #include "springsystem.h"

SpringSystem::SpringSystem(){
}

void SpringSystem::setSpringConstant(double k){
    this->springConstant = k;
}

double SpringSystem::getSpringConstant(){
    return this->springConstant;
}

void SpringSystem::build(SpringNode LBNode, SpringNode RBNode, SpringNode RUNode, SpringNode LUNode, int widthSubdivisions, int heightSubdivisions)
{
    vec3 widthVec = RBNode.getPosition() - LBNode.getPosition();
    vec3 heightVec = LUNode.getPosition() - LBNode.getPosition();

    float widthStep = sqrt(widthVec.x*widthVec.x + widthVec.y*widthVec.y + widthVec.z*widthVec.z)/(float)widthSubdivisions;
    float heightStep = sqrt(heightVec.x*heightVec.x + heightVec.y*heightVec.y + heightVec.z*heightVec.z)/(float)heightSubdivisions;

    for(int i=0; i<= widthSubdivisions; i++){
        vec3 pi = LBNode.getPosition() + i*widthStep*(normalize(RBNode.getPosition() - LBNode.getPosition()));
        vec3 pf = LUNode.getPosition() + i*widthStep*(normalize(RUNode.getPosition() - LUNode.getPosition()));
        for(int j=0; j <= heightSubdivisions; j++){
            SpringNode n;
            n.setMass(10);
            n.setVelocity(vec3(0,0,0));
            n.setPosition(pi + j*heightStep*(normalize(pf - pi)));
            if(j == 0 && i == 0
            || j == 0 && i == widthSubdivisions
            || j == heightSubdivisions && i == widthSubdivisions
            || j == heightSubdivisions && i ==0) n.setAnchor(true);

            //if(j == 0 && i == 0 || j == 0 && i == widthSubdivisions ) n.setAnchor(true);
//            if(j == 0) n.setAnchor(true);

            //if(j == 0 || i == 0 ||i == widthSubdivisions || j == heightSubdivisions) n.setAnchor(true);
            else n.setAnchor(false);
            springNodeList << n;
        }
    }
    makeEdges(widthSubdivisions, heightSubdivisions);
    makeCells(heightSubdivisions);
}

void SpringSystem::makeEdges(int widthSub, int heightSub){

    for(int i=0; i<springNodeList.size(); i=i+heightSub+1){
        for(int j=0; j<heightSub; j++){
            SpringNode *sp0 = &springNodeList[i+j];
            SpringNode *sp1 = &springNodeList[i+j+1];
            Edge e(sp0,sp1);
            edgeList << e;
            double dist = distance(sp0->getPosition(),sp1->getPosition());
            sp0->setNeighbors(sp1);
            sp1->setNeighbors(sp0);
            sp0->setNeighborDistances(dist);
            sp1->setNeighborDistances(dist);
        }
    }

    for(int i=0; i<=heightSub; i++){
        for(int j=1; j<=widthSub; j++){
            SpringNode *sp0 = &springNodeList[i+(heightSub+1)*(j-1)];
            SpringNode *sp1 = &springNodeList[i+(heightSub+1)*j];
            Edge e(sp0,sp1);
            edgeList << e;
            double dist = distance(sp0->getPosition(),sp1->getPosition());
            sp0->setNeighbors(sp1);
            sp1->setNeighbors(sp0);
            sp0->setNeighborDistances(dist);
            sp1->setNeighborDistances(dist);
        }
    }
}

void SpringSystem::makeCells(int heightSub){
    for(int i=0;i<springNodeList.size()-(heightSub+1);i=i+heightSub+1 ){
        for(int j=0; j<heightSub; j++){
            SpringNode *p00 = &springNodeList[i+j];
            SpringNode *p10 = &springNodeList[i+j+heightSub+1];
            SpringNode *p11 = &springNodeList[i+j+heightSub+2];
            SpringNode *p01 = &springNodeList[i+j+1];
            cellList << Cell(p00,p10,p11,p01);

            Edge ed1(p00,p11);
            Edge ed2(p01,p10);
            edgeList << ed1;
            edgeList << ed2;
            p00->setNeighbors(p11);
            p11->setNeighbors(p00);
            p01->setNeighbors(p10);
            p10->setNeighbors(p01);

            double dist1 = distance(p00->getPosition(),p11->getPosition());
            double dist2 = distance(p01->getPosition(),p10->getPosition());

            p00->setNeighborDistances(dist1);
            p11->setNeighborDistances(dist1);
            p01->setNeighborDistances(dist2);
            p10->setNeighborDistances(dist2);
        }
    }
}

void SpringSystem::draw(GLenum drawType){

    if(drawType == GL_TRIANGLES){

        glBegin(GL_TRIANGLES);
        for(int i=0; i<cellList.size(); i++){
            vec3 nx = cellList[i].p10->getPosition() - cellList[i].p00->getPosition();
            vec3 ny = cellList[i].p11->getPosition() - cellList[i].p00->getPosition();
            vec3 n = normalize(cross(nx,ny));
            glNormal3f(n.x,n.y,n.z);
            glVertex3f(cellList[i].p00->getPosition().x,cellList[i].p00->getPosition().y,cellList[i].p00->getPosition().z);
            glVertex3f(cellList[i].p10->getPosition().x,cellList[i].p10->getPosition().y,cellList[i].p10->getPosition().z);
            glVertex3f(cellList[i].p11->getPosition().x,cellList[i].p11->getPosition().y,cellList[i].p11->getPosition().z);

            nx = cellList[i].p11->getPosition() - cellList[i].p00->getPosition();
            ny = cellList[i].p01->getPosition() - cellList[i].p00->getPosition();
            n = normalize(cross(nx,ny));
            glNormal3f(n.x,n.y,n.z);
            glVertex3f(cellList[i].p00->getPosition().x,cellList[i].p00->getPosition().y,cellList[i].p00->getPosition().z);
            glVertex3f(cellList[i].p11->getPosition().x,cellList[i].p11->getPosition().y,cellList[i].p11->getPosition().z);
            glVertex3f(cellList[i].p01->getPosition().x,cellList[i].p01->getPosition().y,cellList[i].p01->getPosition().z);

        }
        glEnd();
    }
    else{
        glBegin(GL_POINTS);
        for(int i=0; i<springNodeList.size(); i++){
            glVertex3f(springNodeList[i].getPosition().x,springNodeList[i].getPosition().y,springNodeList[i].getPosition().z);
        }
        glEnd();

        glBegin(GL_LINES);
        for(int i=0; i<edgeList.size(); i++){
            glVertex3f(edgeList[i].sp0->getPosition().x,edgeList[i].sp0->getPosition().y,edgeList[i].sp0->getPosition().z);
            glVertex3f(edgeList[i].sp1->getPosition().x,edgeList[i].sp1->getPosition().y,edgeList[i].sp1->getPosition().z);

        }
        glEnd();
    }
}

QList<SpringNode> *SpringSystem::getSpringNodeList(){
    return &this->springNodeList;
}

