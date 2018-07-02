#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <list>

#include "Board.h"
#include "Fps.h"
#include "TextureCollection.h"
#include "Object.h"
#include "Label.h"
#include "Menu.h"
#include "VerticalLayout.h"
#include "Button.h"
#include "Timelapse.h"
#include "ProgressBar.h"
#include "Animation.h"
#include "State.h"
#include "SoundCollection.h"

class evolution
{
    public:
        evolution();
        virtual ~evolution();
        void run();

        static void log(std::string log);
        static bool cmpf(float a, float b, float epsilon);
        static float floorf(float a);
        static float ceilf(float a);
        static float roundf(float a);
        static Fps* fps;
        static evo::SoundCollection* soundCollection;
    protected:
        void initLib();
        void changeState();

    private:
        evo::State* currentState;
        SDL_RendererInfo rendererInfo;
};

#endif // EVOLUTION_H
