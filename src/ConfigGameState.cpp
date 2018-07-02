#include "ConfigGameState.h"

#include "Button.h"
#include "InputText.h"
#include "VerticalLayout.h"
#include "HorizontalLayout.h"
#include "Display.h"
#include "Seed.h"
#include "Color.h"

namespace evo
{
    /**
     * Constructeur
     */
    ConfigGameState::ConfigGameState() : State()
    {
        HorizontalLayout *seedLayout = new HorizontalLayout(400,40);
        seedLayout->setName("seedLayout");
        seedLayout->setCenter(true);
        seedLayout->setHorizontalMargin(5);
        seedLayout->setPadding(10);
        seedLayout->setPosition({10,0});

        Label *labelSeed = new Label();
        labelSeed->setName("labelSeed");
        labelSeed->setText("Seed  :");
        labelSeed->setFontSize(14);
        seedLayout->addObject(labelSeed);

        this->inputText = new InputText(200,30);
        inputText->setName("input seed");
        seedLayout->addObject(inputText);

        HorizontalLayout *sizeLayout = new HorizontalLayout(400,40);
        sizeLayout->setName("sizeLayout");
        sizeLayout->setPadding(10);
        sizeLayout->setCenter(true);
        sizeLayout->setHorizontalMargin(5);
        sizeLayout->setPosition({10,40});

        Label *labelSize = new Label();
        labelSize->setName("labelSize");
        labelSize->setText("Map size  :");
        labelSize->setFontSize(14);
        sizeLayout->addObject(labelSize);

        this->sizeText = new InputText(200,30);
        sizeText->setName("Map size");
        sizeLayout->addObject(sizeText);

        VerticalLayout *verticalLayout = new VerticalLayout(400,Display::height-80);
        verticalLayout->setCenter(true);
        verticalLayout->setVerticalMargin(10);
        verticalLayout->setPosition({10,80});

        Button* btnGame = new Button(200,50);

        btnGame->setName("Bouton jouer");
        btnGame->setText("Jouer");
        std::function<bool ()> functionGame =
        [this] ()
        {
            State::nextState = States::STATE_INTRO;

            // si la seed a été donné
            std::string seed = this->inputText->getText();
            if ( ! seed.empty() )
            {
                try
                {
                    Seed::seed = stoul(seed, nullptr, 0);
                }
                catch (std::invalid_argument e)
                {
                    Seed::generateNewSeed();
                }

            }
            return true;
        };
        btnGame->setCallback(Event::MouseLeftClick, functionGame);

        verticalLayout->addObject(btnGame);


        Button* btnBack = new Button(200,50);
        btnBack->setName("Bouton retour");
        btnBack->setText("Retour");
        std::function<bool ()> functionBack =
        [this] ()
        {
            State::nextState = States::STATE_MENU;
            return true;
        };
        btnBack->setCallback(Event::MouseLeftClick, functionBack);

        verticalLayout->addObject(btnBack);

        this->addObject(seedLayout);
        this->addObject(sizeLayout);
        this->addObject(verticalLayout);
    }

    /**
     * Destructeur
     */
    ConfigGameState::~ConfigGameState()
    {
    }

    /**
     * Logique de jeu
     */
    void ConfigGameState::gameLogic()
    {
    }

    /**
     * Gestion des événements
     */
    void ConfigGameState::personalEvent(SDL_Event event)
    {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    State::nextState = States::STATE_MENU;
                }

                break;
        }
    }

    /**
     * Gestion des animations
     */
    void ConfigGameState::animation()
    {

    }
}
