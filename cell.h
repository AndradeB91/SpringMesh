#ifndef CELL_H
#define CELL_H

#include "springnode.h"

class Cell
{
public:
    Cell(SpringNode *p00, SpringNode *p10, SpringNode *p11, SpringNode *p01);
    SpringNode *p00;
    SpringNode *p10;
    SpringNode *p11;
    SpringNode *p01;
};

#endif // CELL_H
