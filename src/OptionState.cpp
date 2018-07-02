#include "OptionState.h"
#include "Display.h"
#include "Color.h"

#include <iostream>

namespace evo
{

    OptionState::OptionState() : State()
    {
        this->verticalLayout = new VerticalLayout(400,Display::height);
        this->verticalLayout->setPosition({10,0});
        this->verticalLayout->setName("Barre de menu");
        this->verticalLayout->setCenter(true);
        this->verticalLayout->setVerticalMargin(10);

        this->title = new Label();
        this->title->setName("Titre");
        this->title->setFontSize(24);
        this->title->setText("Options");

        this->btnFullscreen = new Button(200,50);
        this->btnFullscreen->setName("Bouton fullscreen");
        this->btnFullscreen->setText("Fullscreen");
        std::function<int ()> functionFullscreen =
        [this] ()
        {
            Display::ToggleFullscreen();
            State::nextState = States::STATE_OPTION;
            return 0;
        };
        this->btnFullscreen->setCallback(Event::MouseLeftClick, functionFullscreen);

        this->btnRetour = new Button(200,50);
        this->btnRetour->setName("Bouton retour");
        this->btnRetour->setText("Retour");
        std::function<int ()> functionRetour =
        [this] ()
        {
            State::nextState = States::STATE_MENU;
            return 0;
        };
        this->btnRetour->setCallback(Event::MouseLeftClick, functionRetour);

        this->verticalLayout->addObject(title);
        this->verticalLayout->addObject(btnFullscreen);
        this->verticalLayout->addObject(btnRetour);
        this->addObject(verticalLayout);
    }

    OptionState::~OptionState()
    {
    }

    void OptionState::gameLogic()
    {


    }

    void OptionState::personalEvent(SDL_Event event)
    {
    }

    void OptionState::animation()
    {

    }
}
