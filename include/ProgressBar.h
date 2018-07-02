#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "Object.h"


class ProgressBar : public evo::Object
{
     public:
        ProgressBar(int width, int height);
        ~ProgressBar();
        void setActiveColor(SDL_Color activeColor);
        void setMaxValue(int maxValue);
        void setValue(int value);
        void setStep(int step);

        int increment();

        //override method
        void draw();
    protected:
    private:
        int value;
        int maxValue;
        int step;
        SDL_Color activeColor;
};

#endif // PROGRESSBAR_H
