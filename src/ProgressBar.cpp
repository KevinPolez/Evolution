#include "ProgressBar.h"
#include "evolution.h"
#include "Display.h"

ProgressBar::ProgressBar(int width, int height) : Object()
{
    myType = evo::ObjectType::ProgressBar;
    this->value = 0;
    this->maxValue = 100;
    this->step = 1;

    this->setWidth(width);
    this->setHeight(height);

    this->setDisplayRect({0,0,width,height});
    this->setScreenRect({0,0,width,height});
}

ProgressBar::~ProgressBar()
{
    //dtor
}


void ProgressBar::setValue(int value)
{
    this->value = value;
}

void ProgressBar::setMaxValue(int maxValue)
{
    this->maxValue = maxValue;
}

void ProgressBar::setActiveColor(SDL_Color activeColor)
{
    this->activeColor = activeColor;
}

void ProgressBar::setStep(int step)
{
    this->step = step;
}

/**
 * @overwrite
 */
void ProgressBar::draw()
{
    // si la texture de base n'est pas créée, la créer
    if ( ! this->hasTexture() ) {
        evolution::log("creation de la texture");
        this->createTexture();
    }

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = (this->value * this->getWidth()) / this->maxValue;
    src.h = this->getHeight();

    Display::ApplyColorRect(this->texture, src, this->activeColor);
}

int ProgressBar::increment()
{
    if ( this->value >= this->maxValue) return -1;
    this->value += this->step;
    this->draw();
    return this->value;
}
