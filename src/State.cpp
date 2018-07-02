#include <sstream>
#include <iostream>

#include "State.h"
#include "Display.h"
#include "evolution.h"

namespace evo
{
    int State::stateID = States::STATE_NULL;
    int State::nextState = States::STATE_NULL;

    /**
     * Constructeur
     */
    State::State() : focusObject(nullptr)
    {
        SDL_SetRenderTarget(Display::renderer, NULL);
        SDL_SetRenderDrawColor(Display::renderer, 0, 0, 0, 255);
        SDL_RenderClear(Display::renderer);
    }

    /**
     * Destructeur
     */
    State::~State()
    {
        for( auto it = objectList.begin(); it != objectList.end(); ++it) {
            delete (*it);
        }
    }

    /**
     * Ajoute un objet dans la liste
     */
    void State::addObject(Object *object)
    {
        this->objectList.push_back(object);
    }

    /**
     * Rendu de la scène
     */
    void State::render()
    {
        // efface la scene
        Display::ApplyColor(NULL,{0,0,0});

        // Affiche les textures dans l'ordre de superposition (zIndex)
        objectList.sort( ObjectComparator<Object>);
        for( auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            // ne pas rendre un objet qui est en dehors de l'écran
            if ( Display::IsOnScreen((*it)->getScreenRect()))
            {
                (*it)->refresh();
                (*it)->lighmapping();
            }

        }

        SDL_RenderPresent(Display::renderer);
        Fps::setFrameTime();
    }

    /**
     * Recherche tous les objets intersectant un rect
     */
    std::list<Object *> State::searchByRect(SDL_Rect rect)
    {
        std::list<Object *> objectListTmp;

        std::list<Object*>::iterator it;
        for( it = objectList.begin(); it != objectList.end(); ++it)
        {
            SDL_Rect screenRect = (*it)->getScreenRect();
            if ( SDL_HasIntersection(&rect, &screenRect) )
            {
                objectListTmp.push_back((*it));
            }
        }

        return objectListTmp;
    }

    /**
     * Recherche tous les objects intersectant un point
     */
    std::list<Object *> State::searchByPoint(SDL_Point point)
    {
        std::list<Object *> objectListTmp;

        std::list<Object*>::iterator it;
        for( it = objectList.begin(); it != objectList.end(); ++it)
        {
            SDL_Rect screenRect = (*it)->getScreenRect();
            if ( SDL_PointInRect(&point, &screenRect) )
            {
                objectListTmp.push_back((*it));
            }
        }

        return objectListTmp;
    }

    /**
     * Prépare les textures de tous les objets
     */
    void State::finish()
    {
        for( auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            (*it)->finish();
        }
    }

    /**
     * Gestion des événements
     */
    void State::event()
    {
        SDL_Event event;
        bool stopPropagation = false;


        this->objectList.sort( ObjectComparator<Object>);
        // l'ordre de traitement de événements est l'inverse de l'ordre d'affichage
        this->objectList.reverse();

        while ( SDL_PollEvent(&event) )
        {
            // Parcours la liste des objets pour leur envoyer l'événement
            // si l'événement a été traité, on interrompt sa propagation
            // si l'événement a été traité, on donne le focus à l'objet
            Object* returnObj;
            for( auto it = objectList.begin(); it != objectList.end(); ++it)
            {
                returnObj = (*it)->event(event);
                if (  returnObj != nullptr )
                {
                    //evolution::log("evenement traité par : "+ returnObj->getName());
                    // Gestion du focus
                    if ( returnObj->getFocus() == true )
                    {
                        if (this->focusObject != nullptr)
                        {
                            this->focusObject->setFocus(false);
                            this->focusObject->looseFocusEvent();
                        }
                        this->focusObject = returnObj;
                        this->focusObject->gainFocusEvent();
                    }
                    stopPropagation = true;
                    break;
                }
            }

            if ( ! stopPropagation )
            {
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        State::nextState = States::STATE_EXIT;
                        continue; // passer au prochain événement
                        break;
                }

                switch (event.type)
                {
                    case SDL_KEYDOWN:
                        if ( event.key.keysym.sym == SDLK_TAB)
                        {
                            if ( focusObject != nullptr)
                            {
                                focusObject->looseFocusEvent();
                            }

                            // donne le focus à l'objet le plus proche
                            for( auto it = objectList.begin(); it != objectList.end(); ++it)
                            {
                                focusObject = (*it)->findFocus(focusObject);
                                if ( focusObject != nullptr)
                                {
                                    focusObject->gainFocusEvent();
                                    break;
                                }
                            }
                            continue; // passer au prochain événement
                        }
                        break;
                }

                this->personalEvent(event);
            }

        }
    }
}
