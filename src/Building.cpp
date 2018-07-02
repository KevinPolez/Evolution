#include "Building.h"
#include "Display.h"
#include "TextureCollection.h"

namespace evo
{
    /**
     * Constructeur
     */
    Building::Building(BuildingType type) : my_type(type), resistance(0), spriteSheet(nullptr), spriteW(0), spriteH(0)
    {
        this->ticks = 0;
        this->speed = 7;
        this->currentFrame = 0;

        switch (my_type)
        {
            case BuildingType::Portail :
                this->resistance = 1000;
                this->setSpriteSheet(Display::textureCollection->getTexture("building"));
                this->spriteW = 128;
                this->spriteH = 128;
                this->addAnimation("normal",0,20);
                break;
        }
    }

    /**
     * Destructeur
     */
    Building::~Building()
    {

    }

    /**
     * fourni le type
     */
    BuildingType Building::getType()
    {
        return my_type;
    }

    /**
     * Associe une spriteSheet
     */
    void Building::setSpriteSheet(SDL_Texture *texture)
    {
        this->spriteSheet = texture;
    }

    /**
     * Récupére la spriteSheet
     */
    SDL_Texture* Building::getSpriteSheet()
    {
        return this->spriteSheet;
    }

    /**
     * Fourni le Rect correspondant au sprite de la frame courante
     */
    SDL_Rect Building::getSrcRect()
    {
        return {this->currentFrame * this->spriteW,0,spriteW, spriteH};
    }

    /**
     * Ajoute une animation
     */
    void Building::addAnimation(std::string name, int startX, int frameCount)
    {
        this->frameCount = frameCount;
    }

    /**
     * Calcul la frame courante
     */
    void Building::nextAnimationFrame(int gameSpeed)
    {
        if ( frameCount > 1 )
        {
            ticks++;
            if ( ticks % (int) (speed / gameSpeed) == 0 )
            {
                currentFrame++;
                if( currentFrame >= frameCount)
                {
                    currentFrame = 0;
                }
                ticks = 0;
            }
        }
    }

}
