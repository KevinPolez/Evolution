#ifndef HYGROMAP_H
#define HYGROMAP_H

#include "Map.h"

class HygroMap : public Map
{
    public:
        HygroMap(int width, int height);
        void generate();
    protected :
    private:
};
#endif // HYGROMAP_H

