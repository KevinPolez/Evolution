#include "IntroState.h"
#include "Label.h"
#include "Color.h"
#include "Display.h"

namespace evo
{
    IntroState::IntroState() : State()
    {
        Label* skipLabel = new Label();
        skipLabel->setText("Press ESC for skip");
        skipLabel->setPosition({0,0});
        skipLabel->setFontColor(Color::White);
        skipLabel->setFontSize(12);


        scrolling = new ScrollText(400,400);
        scrolling->addParagraphe("La terre est maintenant surpeuplée ... Criminalité et pollution y font rage.");
        scrolling->addParagraphe("Dans les laboratoires de TransX, une nouvelle technologie est née, ouvrant sur un nouveau monde : le portail.");
        scrolling->addParagraphe("Les premiers explorateurs y ont été envoyé, sans espoir de retour. Le portail ne fonctionne que dans un seul sens.");
        scrolling->addParagraphe("Saurez-vous guider ces pionniers afin d'offrir à l'humanité un nouvel avenir ?");

        scrolling->setPosition({Display::width/2-200,Display::height/2-200});
        scrolling->setParagrapheMargin(25);
        scrolling->setBackgroundColor(Color::Black);
        scrolling->setFontColor(Color::White);

        this->addObject(skipLabel);
        this->addObject(scrolling);
    }

    IntroState::~IntroState()
    {

    }

    void IntroState::gameLogic()
    {

    }

    void IntroState::personalEvent(SDL_Event event)
    {
        switch (event.type) {
            case SDL_KEYDOWN:
                // skip intro
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    State::nextState = States::STATE_GAME;
                }
                break;
            case SDL_MOUSEBUTTONDOWN :
                State::nextState = States::STATE_GAME;
                break;
        }
    }

    void IntroState::animation()
    {
        this->scrolling->nextAnimationFrame();
    }
}
