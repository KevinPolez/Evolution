#ifndef WARMMAP_H
#define WARMMAP_H

#include "Map.h"

class WarmMap : public Map
{
    public:
        WarmMap(int width, int height);
        void generate();
    protected :
    private:
};
#endif // WARMMAP_H
