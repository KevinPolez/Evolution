#include "HygroMap.h"


HygroMap::HygroMap(int width, int height) : Map(width,height)
{
}

void HygroMap::generate()
{
    this->generateNoize(0,100);
    this->generateCellularAutomata(3);
}

