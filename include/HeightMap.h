#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "Map.h"

class HeightMap : public Map
{
    public:
        HeightMap(int width, int height);
        void generate();
    protected:
    private:
};
#endif // HEIGHTMAP_H
