#include "Seed.h"

#include <chrono>

namespace evo
{
    unsigned Seed::seed = 0;

    void Seed::generateNewSeed()
    {
        Seed::seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
}
