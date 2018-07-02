#include "InputText.h"
#include "Color.h"
#include "Display.h"
#include "evolution.h"

#include <iostream>

namespace evo
{
    InputText::InputText(int width, int height): Object(), inputMode(false)
    {
        this->myType = ObjectType::InputText;

        this->setWidth(width);
        this->setHeight(height);

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});

        this->setActiveColor(Color::Active);
        this->setPassiveColor(Color::Passive);
        this->setBorderColor(Color::Border);
        this->setFontColor(Color::Font);

        this->setBackgroundColor(Color::Active);

        this->fontSize = 16;
        this->textNeedRedraw = false;
        this->textTexture = NULL;
        this->text = "";
        this->setFocusable(true);
    }

    InputText::~InputText()
    {

    }


    void InputText::setText(std::string text) {
        this->text = text;
        this->textNeedRedraw = true;
    }

    std::string InputText::getText()
    {
        return this->text;
    }

    void InputText::setFontColor(SDL_Color color) {
        this->fontColor = color;
        this->textNeedRedraw = true;
    }

    void InputText::setFontSize(int fontSize) {
        this->fontSize = fontSize;
        this->textNeedRedraw = true;
    }

    /**
     * Creation de la texture texte
     */
    void InputText::buildTextTexture()
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

    void InputText::draw()
    {
        evolution::log("draw : " + this->getName());

        // si la texture de base n'est pas créée, la créer
        if ( ! this->hasTexture() ) {
            this->createTexture();
        }

        // application de la couleur de fond
        this->applyBackgroundColor();

        // applique les bordures
        this->applyBorderColor();

        if ( this->textNeedRedraw ) {
            this->buildTextTexture();
        }

        // applique la texture texte sur la texture de l'objet
        if ( textTexture != NULL)
        {
            int y = (this->getHeight() / 2) - (this->textH / 2);
            Display::ApplyTexture(this->textTexture,{0,0,this->textW,this->textH},this->texture,{2,y,this->textW,this->textH});
        }
    }

    void InputText::setActiveColor(SDL_Color color) {
        this->activeColor = color;
    }

    void InputText::setPassiveColor(SDL_Color color) {
        this->passiveColor = color;
        this->setBackgroundColor(color);
    }

    /**
     * @override
     */
    bool InputText::gainFocusEvent()
    {
        evolution::log("gain du focus");
        this->inputMode = true;
        this->setBackgroundColor(Color::White);
        this->draw();
        return true;
    }

    /**
     * @override
     */
    bool InputText::looseFocusEvent()
    {
        evolution::log("perte du focus");
        this->inputMode = false;
        this->setBackgroundColor(Color::Active);
        this->draw();
        return true;
    }

    /**
     * @override
     */
    bool InputText::mouseLeftClickEvent(SDL_Point p)
    {
        std::cout << "mouseLeftClick on InputText" << std::endl;
        this->setFocus(true);
        return true;
    }

    /**
     * @override
     */
    bool InputText::textInputEvent(char* c)
    {
        if ( this->inputMode == true )
        {
            std::cout << "textInput event on InputText" << std::endl;
            this->text += c;
            this->textNeedRedraw = true;
            this->draw();
            return true;
        }
        return false;
    }

    /**
     * @override
     */
    bool InputText::textBackspaceEvent()
    {
        if ( this->inputMode == true )
        {
            if ( ! this->text.empty() )
            {
                this->text.pop_back();
                this->textNeedRedraw = true;
                this->draw();
                return true;
            }
        }
        return false;
    }
}
