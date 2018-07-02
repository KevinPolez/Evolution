#ifndef OPTIONSTATE_H
#define OPTIONSTATE_H

#include "State.h"
#include "VerticalLayout.h"
#include "Button.h"
#include "Label.h"

namespace evo
{
    class OptionState : public State
    {
        public:
            OptionState();
            ~OptionState();

            //@override
            void gameLogic();
            void personalEvent(SDL_Event event);
            void animation();

        protected:
        private:
            VerticalLayout *verticalLayout;
            Button* btnRetour;
            Button* btnFullscreen;
            Label* title;

    };
}
#endif // OPTIONSTATE_H
