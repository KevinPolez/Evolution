#include "Animation.h"
#include "Display.h"
#include "evolution.h"

namespace evo
{

    Animation::Animation(int width, int height) :
        Object(),
        animationFrameCount(0),
        currentFrame(0),
        speed(1),
        fps(60),
        line(5),
        ticks(0)
    {
        this->myType = evo::ObjectType::Animation;

        this->setWidth(width);
        this->setHeight(height);

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});
    }

    Animation::~Animation()
    {

    }

    void Animation::setTextureSprite(SDL_Texture *texture)
    {
        this->textureSprite = texture;
    }

    void Animation::setAnimationFrameCount(int animationFrameCount)
    {
        this->animationFrameCount = animationFrameCount;
    }

    void Animation::setSpeed(int speed)
    {
        this->speed = speed;
    }

    void Animation::setFpsLimit(int fps)
    {
        this->fps = fps;
    }

    /**
     * Affiche la première frame de l'animation
     * @override
     */
    void Animation::draw()
    {
        // si la texture de base n'est pas créée, la créer
        if ( ! this->hasTexture() ) {
            this->createTexture();
            Display::SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
        }

        SDL_Rect src;
        src.x = 0;
        src.y = 64 * (9-1);
        src.w = 64;
        src.h = 64;

        this->applyBackgroundColor();

        Display::ApplyTexture(this->textureSprite, src, this->texture, this->getDisplayRect());
    }

    /**
     * Affiche la frame suivante de l'animation
     * @override
     */
    void Animation::nextAnimationFrame()
    {
        ticks++;
        if ( ticks % speed == 0 )
        {
            currentFrame++;
            if( currentFrame > animationFrameCount)
            {
                currentFrame = 1;
            }

            SDL_Rect src;
            src.x = 64 * (currentFrame -1);
            src.y = 64 * (line -1);
            src.w = 64;
            src.h = 64;
            Display::ClearTexture(this->texture);
            this->applyBackgroundColor();
            Display::ApplyTexture(this->textureSprite, src, this->texture, this->getDisplayRect());
            ticks = 0;
        }
    }
}
