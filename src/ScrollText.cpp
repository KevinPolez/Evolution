#include "ScrollText.h"
#include "Display.h"

namespace evo
{
    ScrollText::ScrollText(int width, int height): ObjectText(), speed(2), ticks(0), textPositionY(height)
    {
        this->myType = ObjectType::ScrollText;

        this->setWidth(width);
        this->setHeight(height);

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});
    }

    ScrollText::~ScrollText()
    {

    }

    /**
     * Affiche la frame suivante de l'animation
     * @override
     */
    void ScrollText::nextAnimationFrame()
    {
        ticks++;
        if ( ticks % speed == 0 )
        {
            this->textPositionY -= 1;

            if ( this->textPositionY <= 0 )
            {
                return;
            }

            if ( this->textPositionY > this->getHeight()) this->textPositionY = this->getHeight();

            if ( ! this->hasTexture() )
            {
                this->createTexture();
            }

            // application de la couleur de background à la texture principale
            this->applyBackgroundColor();

            int height = 0;
            if ( this->getTextHeight() > this->getHeight() )
            {
                height = this->getHeight();
            }
            else
            {
                height = this->getTextHeight();
            }
            if (height < 0) height = 0;

            Display::ApplyTexture(this->getTextTexture(),{0,0,this->getWidth(),height},this->texture,{5,textPositionY,this->getWidth(),height});

            ticks = 0;
        }
    }

    void ScrollText::draw()
    {
        if ( ! this->hasTexture() )
        {
            this->createTexture();
        }

        // a faire uniquement si les caractéristiques du texte ont été modifié
        if ( this->getTextNeedRedraw() )
        {
            this->buildTextTexture();
        }

        // application de la couleur de background à la texture principale
        this->applyBackgroundColor();

        // creation des bordures
        this->applyBorderColor();

        // application du texte à la texture principale
        int height = 0;
        if ( this->getTextHeight() > this->getHeight() )
        {
            height = this->getHeight();
        }
        else
        {
            height = this->getTextHeight();
        }
        if (height < 0) height = 0;

        if ( this->getTextTexture() != NULL )
        {
            Display::ApplyTexture(this->getTextTexture(),{0,0,this->getWidth(),height},this->texture,{5,textPositionY,this->getWidth(),height});
        }
    }
}
