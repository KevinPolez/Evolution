#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "Board.h"
#include "Label.h"
#include "Menu.h"
#include "VerticalLayout.h"
#include "Button.h"
#include "ProgressBar.h"
#include "Timelapse.h"
#include "TextureCollection.h"
#include "DialogBox.h"
#include "HorizontalLayout.h"
#include "Unit.h"
#include "Building.h"

namespace evo
{
    class GameState : public State
    {
        public:
            GameState();
            ~GameState();

            // @override
            void gameLogic();
            void personalEvent(SDL_Event event);
            void animation();

        protected:
        private:
            Timelapse *timelapse;
            Board* board;
            HorizontalLayout* infoBar;
            Label *timeLabel;
            Label *fpsLabel;
            Menu *menu;
            VerticalLayout *verticalLayout;
            Label *averageTemperature;
            ProgressBar *progressBar;
            Button* btnMenu;
            Box* dialogBox;
            Box* infoBox;
            Label* zoomLabel;
            std::list<Unit*> unitList;
            std::list<Building*> buildingList;
    };
}
#endif // GAMESTATE_H
