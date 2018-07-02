#include "Label.h"
#include "Display.h"
#include "evolution.h"
#include "Color.h"

#include <stdio.h>
#include <iostream>

namespace evo
{
    Label::Label(): Object()
    {
        myType = ObjectType::Label;
        this->fontSize = 24;
        this->font = TTF_OpenFont("resource/font/calibri.ttf",this->fontSize);
        this->setFontColor(Color::Font);
        this->text = " ";
    }

    Label::~Label()
    {
        TTF_CloseFont(font);
    }

    void Label::setText(std::string text) {
        this->text = text;
    }

    void Label::setFontColor(SDL_Color color) {
        this->fontColor = color;
    }

    void Label::setFontSize(int fontSize) {
        this->fontSize = fontSize;
        this->reloadFont();
    }

    void Label::reloadFont() {
        TTF_CloseFont(this->font);
        this->font = TTF_OpenFont("resource/font/calibri.ttf",this->fontSize);
    }

    void Label::setBackgroundTransparency(int transparency) {

    }

    void Label::draw()
    {
        evolution::log("draw : " + this->getName());

        int w, h;
        TTF_SizeText(this->font, text.c_str(), &w, &h);

        SDL_Surface* surface;
        surface = TTF_RenderText_Blended(this->font, text.c_str(),this->fontColor);
        this->setTexture(Display::CreateTextureFromSurface(surface));
        SDL_FreeSurface(surface);

        // Correction de l'affichage en fonction de la nouvelle hauteur et largeur de la surface
        this->setWidth(w);
        this->setHeight(h);
        this->setDisplayRect({0,0,w,h});
        this->screenRect.w = w;
        this->screenRect.h = h;
    }
}
