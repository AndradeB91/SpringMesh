#ifndef EDGE_H
#define EDGE_H

#include "springnode.h"
#include "glm/glm/glm.hpp"

using namespace glm;

class Edge
{
public:
    Edge(SpringNode *sp0, SpringNode *sp1);
    SpringNode *sp0;
    SpringNode *sp1;

};

#endif // EDGE_H
