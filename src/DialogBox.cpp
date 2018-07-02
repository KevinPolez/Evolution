#include "evolution.h"
#include "DialogBox.h"
#include "Display.h"
#include "Color.h"

namespace evo
{

    Box::Box(int width, int height): ObjectText()
    {
        this->myType = ObjectType::Box;

        this->setWidth(width);
        this->setHeight(height);

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});
    }

    Box::~Box()
    {

    }

    /**
     * @override
     */
    bool Box::mouseEnterEvent()
    {
        std::cout <<"mouse enter event bouton" << std::endl;
        return true;
    }

    /**
     * @override
     */
    bool Box::mouseLeaveEvent()
    {
        std::cout << "mouse leave event bouton" << std::endl;
        return true;
    }

    /**
     * @override
     */
    bool Box::mouseLeftClickEvent(SDL_Point p)
    {
        std::cout << "mouse left click event bouton" << std::endl;
        return true; // stop la propagation de l'événement
    }

    /**
     * @override
     * \brief Provoque le déplacement de la fenêtre
     * // TODO : mettre en place une zone de grip
     */
    bool Box::mouseLeftDragEvent(SDL_Point p)
    {
        evolution::log("mouse drag event");
        this->updateScreenRect(p);
        return true;
    }

    /**
     * @override
     */
    void Box::draw()
    {
        evolution::log("draw : " + this->getName());

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

        if ( this->getTextTexture() != NULL )
        {
            Display::ApplyTexture(this->getTextTexture(),{0,0,this->getWidth(),height},this->texture,{5,5,this->getWidth(),height});
        }
    }

}
