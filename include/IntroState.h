#ifndef INTROSTATE_H
#define INTROSTATE_H

#include "State.h"
#include "ScrollText.h"

namespace evo
{
    class IntroState : public State
    {
        public:
            IntroState();
            ~IntroState();

            // @override
            void personalEvent(SDL_Event event);
            void gameLogic();
            void animation();
        protected:
        private:
            ScrollText *scrolling;
    };
}
#endif // INTROSTATE_H
