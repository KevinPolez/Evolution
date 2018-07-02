#ifndef CONFIGGAMESTATE_H
#define CONFIGGAMESTATE_H

#include "State.h"
#include "InputText.h"

namespace evo
{
    class ConfigGameState : public State
    {
        public:
            ConfigGameState();
            ~ConfigGameState();

            // @override
            void gameLogic();
            void personalEvent(SDL_Event event);
            void animation();
        protected:
        private:
            InputText *inputText;
            InputText *sizeText;
    };
}
#endif // CONFIGGAMESTATE_H
