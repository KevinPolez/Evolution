#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "State.h"
#include "VerticalLayout.h"
#include "Button.h"
#include "Label.h"
#include "Animation.h"
#include "ProgressBar.h"
#include "Timelapse.h"
#include "DialogBox.h"

namespace evo
{

    class MenuState : public State
    {
        public:
            MenuState();

            //@override
            void gameLogic();
            void personalEvent(SDL_Event event);
            void animation();
        protected:
        private:
            VerticalLayout *verticalLayout;
            Button* btnQuit;
            Button* btnOption;
            Button* btnGame;
            Label* title;
    };
}

#endif // MENUSTATE_H
