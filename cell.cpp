#include "cell.h"

Cell::Cell(SpringNode *p00, SpringNode *p10, SpringNode *p11, SpringNode *p01)
{
    this->p00 = p00;
    this->p10 = p10;
    this->p11 = p11;
    this->p01 = p01;
}
