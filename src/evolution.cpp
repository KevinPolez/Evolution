#include "evolution.h"

#include "Display.h"
#include "Fps.h"

#include "ConfigGameState.h"
#include "GameState.h"
#include "MenuState.h"
#include "OptionState.h"
#include "IntroState.h"

#include <sstream>
#include <SDL2/SDL_mixer.h>
#include <cmath>

// TODO: Faire une page pour les crédits

Fps* evolution::fps = new Fps(60);
evo::SoundCollection* evolution::soundCollection = new evo::SoundCollection();

/**
 * Constructeur
 */
evolution::evolution() : currentState(NULL)
{
    this->initLib();

    // LOAD AUDIO RESOURCE
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);

    // CREATE MAIN WINDOW
    Display::CreateWindow();

    // CREATE RENDERER
    SDL_GetRendererInfo(Display::renderer, &this->rendererInfo);
    SDL_SetRenderDrawColor(Display::renderer, 0, 0, 0, 255);

    // chargement des textures
    Display::LoadTextureCollection();

    // chargement des sons
    evolution::soundCollection->loadSound("ambiance","resource/sound/ambient.mp3");
    evolution::soundCollection->loadSound("click","resource/sound/UI_SFX_Set/click5.wav");

    // on lance le menu
    evo::State::stateID = evo::States::STATE_MENU;
    this->currentState = new evo::MenuState();
    this->currentState->finish();
}

/**
 * Desctructeur
 */
evolution::~evolution()
{
    delete this->currentState;

    delete Display::textureCollection;
    //delete fps;

    SDL_StopTextInput();

    SDL_DestroyRenderer(Display::renderer);
    SDL_DestroyWindow(Display::window);

    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

/**
 * Gestion des changements d'états
 */
void evolution::changeState()
{
    //If the state needs to be changed
    if( evo::State::nextState != evo::States::STATE_NULL )
    {
        //Delete the current state
        if( evo::State::nextState != evo::States::STATE_EXIT )
        {
            delete this->currentState;
        }

        //Change the state
        switch( evo::State::nextState )
        {
            case evo::States::STATE_MENU:
                this->currentState = new evo::MenuState();
                this->currentState->finish();
                break;
            case evo::States::STATE_CONFIG_GAME:
                this->currentState = new evo::ConfigGameState();
                this->currentState->finish();
                break;
            case evo::States::STATE_INTRO:
                this->currentState = new evo::IntroState();
                this->currentState->finish();
                break;
            case evo::States::STATE_GAME:
                this->currentState = new evo::GameState();
                this->currentState->finish();
                break;
            case evo::States::STATE_OPTION:
                this->currentState = new evo::OptionState();
                this->currentState->finish();
                break;
        }

        evo::State::stateID = evo::State::nextState;
        evo::State::nextState = evo::States::STATE_NULL;
    }

}

void evolution::run()
{
    while ( evo::State::stateID != evo::States::STATE_EXIT)
    {
        // logique de jeu
        this->currentState->gameLogic();

        // limitateur de fps
        fps->limit();

        // gestion des animations
        this->currentState->animation();

        // rendu de la scene
        this->currentState->render();

        // gestion des états de jeu
        this->changeState();

        // gestion des événements
        this->currentState->event();
    }
}

void evolution::initLib()
{
    // INIT SDL
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        std::cout << "Error while init SDL" << std::endl;
    }

    // INIT TTF
    if (TTF_Init() == -1) {
        std::cout << "Error while init SDL_TTF" << std::endl;
    }

    // INIT MIXER
    int flags=MIX_INIT_MP3;
    if ( Mix_Init(flags) == -1 ) {
        std::cout << "Error while init SDL_MIXER" << std::endl;
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);

    SDL_StartTextInput();
}

/**
 * Fonction de log
 * //TODO: doit rediriger le flux vers un fichier
 */
void evolution::log(std::string log)
{
    std::ostringstream time;
    time << SDL_GetTicks();

    std::cout << "[" + time.str() + "] " + log << std::endl;
}

/**
 * Comparaison entre deux nombres à virgules flotantes
 */
bool evolution::cmpf(float A, float B, float epsilon = 0.005f)
{
    return (fabs(A - B) < epsilon);
}

float evolution::roundf(float f)
{
    return round(f * 100) / 100;
}

float evolution::ceilf(float f)
{
    return ceil(f * 100) / 100;
}

float evolution::floorf(float f)
{
    return floor(f * 100) / 100;
}
