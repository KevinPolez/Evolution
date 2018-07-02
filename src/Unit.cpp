#include "Unit.h"
#include "Display.h"

namespace evo
{
    Unit::Unit(UnitType type) : spriteSheet(nullptr)
    {
        this->spriteSheet = Display::textureCollection->getTexture("units");
    }

    Unit::~Unit()
    {

    }

    SDL_Texture* Unit::getSpriteSheet() const
    {
        return this->spriteSheet;
    }

    SDL_Rect Unit::getSrcRect() const
    {
        return {0,0,64,48};
    }
}
