
#ifndef COLOR_H
#define COLOR_H

#include <SDL2/SDL.h>

namespace evo
{
    class Color {
        public :
            static const SDL_Color Blue;
            static const SDL_Color DarkBlue;
            static const SDL_Color AliceBlue;
            static const SDL_Color Lavender;
            static const SDL_Color PowderBlue;
            static const SDL_Color LightBlue;

            static const SDL_Color Gainsboro;
            static const SDL_Color LightGrey;
            static const SDL_Color Silver;
            static const SDL_Color DarkGrey;
            static const SDL_Color Grey;
            static const SDL_Color DimGrey;
            static const SDL_Color LightSlateGrey;
            static const SDL_Color SlateGrey;
            static const SDL_Color DarkSlateGrey;

            static const SDL_Color Border;
            static const SDL_Color Background;
            static const SDL_Color Font;
            static const SDL_Color Active;
            static const SDL_Color Passive;

            static const SDL_Color Black;

            static const SDL_Color Red;
            static const SDL_Color White;
            static const SDL_Color Green;
    };
}
#endif // COLOR_H
