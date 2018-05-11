#include "springnode.h"
SpringNode::SpringNode(){

}
SpringNode::SpringNode(bool isAnchor)
{
    isAnchor = isAnchor;
}

void SpringNode::setPosition(vec3 position){
    this->position = position;
}

void SpringNode::setVelocity(vec3 velocity){
    this->velocity = velocity;
}

void SpringNode::setForce(vec3 force){
    this->force = force;
}

void SpringNode::setMass(float mass){
    this->mass = mass;
}

void SpringNode::setNeighbors(SpringNode *neighbors){
    this->neighbors << neighbors;
}

void SpringNode::setNeighborDistances(double d){
    this->neighborDistances << d;
}

void SpringNode::setAnchor(bool anchor){
    this->isAnchor = anchor;
}

vec3 SpringNode::getPosition(){
    return this->position;
}

vec3 SpringNode::getVelocity(){
    return this->velocity;
}

vec3 SpringNode::getForce(){
    return this->force;
}

float SpringNode::getMass(){
    return this->mass;
}

QList<SpringNode*> SpringNode::getNeighbors(){
    return this->neighbors;
}

QList<double> SpringNode::getNeighborDistance(){
    return this->neighborDistances;
}

bool SpringNode::isNodeAnchor(){
    return isAnchor;
}
