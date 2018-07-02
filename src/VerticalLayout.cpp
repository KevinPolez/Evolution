#include "evolution.h"
#include "Display.h"
#include "VerticalLayout.h"
#include "Color.h"

namespace evo
{

    /**
     * Constructeur
     */
    VerticalLayout::VerticalLayout(int width, int height) : Object()
    {
        this->myType = ObjectType::VerticalLayout;
        this->setWidth(width);
        this->setHeight(height);

        this->center = false;
        this->verticalMargin = 0;

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});

        this->setBackgroundColor(Color::Background);
    }

    /**
     * Dessine la texture de l'objet
     */
    void VerticalLayout::draw()
    {
        evolution::log("draw : " + this->getName());

        if ( ! this->hasTexture() ) this->createTexture();

        this->applyBackgroundColor();

         // pour chacun des objets placés dans la barre il faut calculer leur screenRect
        int startY = 0;
        std::list<Object*>::iterator it;
        for( it = objectList.begin(); it != objectList.end(); ++it)
        {
            SDL_Rect childScreenRect = (*it)->getScreenRect();

            // prise en compte du centrage horizontal
            if ( this->center == true)
            {
                int objWidth = (*it)->getWidth();
                int myWidth = this->getWidth();

                childScreenRect.x = (myWidth/2)  - (objWidth / 2);

            }
            childScreenRect.x += this->screenRect.x;

            // prise en compte de la marge verticale
            startY += this->verticalMargin;
            childScreenRect.y = startY;
            childScreenRect.y += this->screenRect.y;

            (*it)->setScreenRect(childScreenRect);

            startY += (*it)->getHeight() + this->verticalMargin;
        }
    }

    /**
     * Indique si les objets contenu dans la barre doivent être centré
     */
    void VerticalLayout::setCenter(bool center)
    {
        this->center = center;
    }

    /**
     * Défnie la marge varticale entre les objets
     */
    void VerticalLayout::setVerticalMargin(int verticalMargin)
    {
        this->verticalMargin = verticalMargin;
    }

}
