#include "Button.h"
#include "Display.h"
#include "evolution.h"
#include "Color.h"

#include <iostream>

namespace evo
{

    Button::Button(int width, int height) : Object()
    {
        myType = ObjectType::Button;

        this->setWidth(width);
        this->setHeight(height);

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});

        this->setPassiveColor(Color::Passive);
        this->setActiveColor(Color::Active);
        this->setFontColor(Color::Font);
        this->setBorderColor(Color::Border);

        this->fontSize = 24;
        this->textNeedRedraw = true;
        this->textTexture = NULL;
    }

    Button::~Button()
    {
    }

    void Button::setText(std::string text) {
        this->text = text;
        this->textNeedRedraw = true;
    }

    void Button::setFontColor(SDL_Color color) {
        this->fontColor = color;
        this->textNeedRedraw = true;
    }

    void Button::setFontSize(int fontSize) {
        this->fontSize = fontSize;
        this->textNeedRedraw = true;
    }

    void Button::setActiveColor(SDL_Color color) {
        this->activeColor = color;
    }

    void Button::setPassiveColor(SDL_Color color) {
        this->passiveColor = color;
        this->setBackgroundColor(color);
    }

    /**
     * Creation de la texture texte
     */
    void Button::buildTextTexture()
    {
        SDL_Surface* surface;
        this->font = TTF_OpenFont("resource/font/calibri.ttf", this->fontSize);

        if ( this->textTexture != NULL)
        {
            SDL_DestroyTexture(this->textTexture);
            this->textTexture = NULL;
        }

        surface = TTF_RenderText_Blended(this->font, this->text.c_str(),this->fontColor);
        this->textTexture = Display::CreateTextureFromSurface(surface);
        SDL_FreeSurface(surface);

        TTF_SizeText(this->font, this->text.c_str(), &this->textW, &this->textH);
        TTF_CloseFont(this->font);

        this->textNeedRedraw = false;
    }

    /**
     * @override
     */
    bool Button::mouseEnterEvent()
    {
        evolution::log("mouse enter event " + this->getName());
        this->setBackgroundColor(activeColor);
        this->draw();
        return true;
    }

    /**
     * @override
     */
    bool Button::mouseLeaveEvent()
    {
        evolution::log("mouse leave event bouton " + this->getName());
        this->setBackgroundColor(passiveColor);
        this->draw();
        return true;
    }

    /**
     * @override
     */
    bool Button::mouseLeftClickEvent(SDL_Point p)
    {
        evolution::soundCollection->playSound("click");

        // lancer les méthodes de callback enregistés
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseLeftClick);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * @override
     */
    void Button::draw()
    {
        evolution::log("draw : " + this->getName());

        // si la texture de base n'est pas créée, la créer
        if ( ! this->hasTexture() ) {
            this->createTexture();
        }

        // a faire uniquement si les caractéristiques du texte ont été modifié
        if ( this->textNeedRedraw ) {
            this->buildTextTexture();
        }

        // application de la couleur de fond
        this->applyBackgroundColor();

        // applique les bordures
        this->applyBorderColor();

         // center text
        int x = (this->getWidth() / 2) - (this->textW / 2);
        int y = (this->getHeight() / 2) - (this->textH / 2);

        // applique la texture texte sur la texture de l'objet
        Display::ApplyTexture(this->textTexture,{0,0,this->textW,this->textH},this->texture,{x,y,this->textW,this->textH});
    }
}
