#ifndef SPRINGNODE_H
#define SPRINGNODE_H

#include "glm/glm/glm.hpp"
#include <QList>

using namespace glm;

class SpringNode
{
public:
    SpringNode();
    SpringNode(bool isAnchor);
    void setPosition(vec3 position);
    void setVelocity(vec3 velocity);
    void setForce(vec3 force);
    void setMass(float mass);
    void setNeighbors(SpringNode* neighbors);
    void setNeighborDistances(double d);
    void setAnchor(bool anchor);

    vec3 getPosition();
    vec3 getVelocity();
    vec3 getForce();
    float getMass();
    QList<SpringNode*> getNeighbors();
    QList<double> getNeighborDistance();

    bool isNodeAnchor();

private:
    vec3 position;
    vec3 velocity;
    vec3 force;

    float mass;

    QList<SpringNode*> neighbors;
    QList<double> neighborDistances;

    bool isAnchor;
};

#endif // SPRINGNODE_H
