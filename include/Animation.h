#ifndef ANIMATION_H
#define ANIMATION_H

#include "Object.h"

namespace evo
{
    class Animation : public evo::Object
    {
        public:
            Animation(int width, int height);
            ~Animation();

            void setTextureSprite(SDL_Texture *texture);
            void setAnimationFrameCount(int animationFrameCount);
            void setSpeed(int speed);
            void setFpsLimit(int fps);

            // override
            void nextAnimationFrame();
            void draw();
        protected:
        private:
            SDL_Texture* textureSprite;
            int animationFrameCount;
            int currentFrame;
            int speed;
            int fps;
            int line;
            int ticks;

    };
}
#endif
