#include "HorizontalLayout.h"
#include "Color.h"
#include "evolution.h"
#include "Display.h"

namespace evo
{

    HorizontalLayout::HorizontalLayout(int width, int height) : Object()
    {
        this->myType = ObjectType::HorizontalLayout;
        this->setWidth(width);
        this->setHeight(height);

        this->center = false;
        this->horizontalMargin = 0;

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});

        this->setBackgroundColor(Color::Background);
    }

    HorizontalLayout::~HorizontalLayout()
    {
    }

    void HorizontalLayout::draw()
    {
        evolution::log("draw : " + this->getName());

        if ( ! this->hasTexture() ) this->createTexture();

        this->applyBackgroundColor();

        int startX = this->padding;
        for( auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            SDL_Rect childScreenRect = (*it)->getScreenRect();

            if ( this->center == true)
            {
                int objHeight = (*it)->getHeight();
                int myHeight = this->getHeight();

                childScreenRect.y = (myHeight/2)  - (objHeight / 2);
                childScreenRect.y += this->screenRect.y;

            }
            else
            {
                childScreenRect.y = this->screenRect.y + this->padding;
            }

            childScreenRect.x = startX;
            childScreenRect.x += this->screenRect.x;

            (*it)->setScreenRect(childScreenRect);

            startX += (*it)->getWidth() + this->horizontalMargin;
        }
    }

    /**
     * Défnie la marge horizontal entre les objets
     */
    void HorizontalLayout::setHorizontalMargin(int horizontalMargin)
    {
        this->horizontalMargin = horizontalMargin;
    }

    /**
     * Indique si les objets contenu dans la barre doivent être centré
     */
    void HorizontalLayout::setCenter(bool center)
    {
        this->center = center;
    }

    /**
     * Défini le padding du layout
     */
    void HorizontalLayout::setPadding(int padding)
    {
        this->padding = padding;
    }
}
