#include "MenuState.h"
#include "Display.h"
#include "evolution.h"
#include "Color.h"

namespace evo
{

    MenuState::MenuState() : State()
    {
        this->verticalLayout = new VerticalLayout(400,Display::height);
        this->verticalLayout->setPosition({10,0});
        this->verticalLayout->setName("Vertical Layout");
        this->verticalLayout->setCenter(true);
        this->verticalLayout->setVerticalMargin(10);

        this->title = new Label();
        this->title->setName("Titre");
        this->title->setFontSize(32);
        this->title->setText("Evolution");

        this->btnGame = new Button(200,50);
        this->btnGame->setName("Bouton jouer");
        this->btnGame->setText("Jouer");
        std::function<bool ()> functionGame =
        [this] ()
        {
            State::nextState = States::STATE_CONFIG_GAME;
            return true;
        };
        this->btnGame->setCallback(Event::MouseLeftClick, functionGame);

        this->btnOption = new Button(200,50);
        this->btnOption->setName("Bouton option");
        this->btnOption->setText("Options");
        std::function<bool ()> functionOption =
        [this] ()
        {
            State::nextState = States::STATE_OPTION;
            return true;
        };
        this->btnOption->setCallback(Event::MouseLeftClick, functionOption);

        this->btnQuit = new Button(200,50);
        this->btnQuit->setName("Bouton quitter");
        this->btnQuit->setText("Quitter");

        std::function<bool ()> functionTest =
        [this] ()
        {
            State::nextState = States::STATE_EXIT;
            return true;
        };
        this->btnQuit->setCallback(Event::MouseLeftClick, functionTest);

        this->verticalLayout->addObject(title);
        this->verticalLayout->addObject(btnGame);
        this->verticalLayout->addObject(btnOption);
        this->verticalLayout->addObject(btnQuit);

        this->addObject(verticalLayout);
    }


    void MenuState::gameLogic()
    {
    }

    void MenuState::personalEvent(SDL_Event event)
    {
    }

    void MenuState::animation()
    {

    }
}
