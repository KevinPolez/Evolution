#ifndef BUILDING_H
#define BUILDING_H

#include <SDL2/SDL.h>
#include <string>

namespace evo
{
    enum class BuildingType { Portail };

    class Building
    {
        public:
            Building(BuildingType type);
            ~Building();

            BuildingType getType();
            void setSpriteSheet(SDL_Texture* text);
            SDL_Texture* getSpriteSheet();
            SDL_Rect getSrcRect();
            void nextAnimationFrame(int gameSpeed);
        protected:
            void addAnimation(std::string name, int startX, int frameCount);
        private:
            BuildingType my_type;
            int resistance;
            SDL_Texture* spriteSheet;
            int spriteW;
            int spriteH;
            int frameCount;
            int currentFrame;
            int ticks;
            int speed;
    };
}
#endif // BUILDING_H
