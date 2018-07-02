#ifndef UNIT_H
#define UNIT_H

#include <SDL2/SDL.h>

namespace evo
{
    enum class UnitType { Test };

    class Unit
    {
        public:
            Unit(UnitType type);
            ~Unit();

            SDL_Texture* getSpriteSheet() const;
            SDL_Rect getSrcRect() const;
        protected:
        private:
            SDL_Texture *spriteSheet;

    };
}
#endif // UNIT_H
