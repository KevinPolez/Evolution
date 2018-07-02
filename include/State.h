#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>

#include "Object.h"

namespace evo
{

    enum States
    {
        STATE_NULL,
        STATE_MENU,
        STATE_CONFIG_GAME,
        STATE_INTRO,
        STATE_GAME,
        STATE_OPTION,
        STATE_EXIT,
    };

    class State {
        public:
            /**
             * Constructeur
             */
            State();

            /**
             * Destructeur
             */
            virtual ~State();

            /**
             * Gestion de la logique de jeu
             */
            virtual void gameLogic() = 0;

            /**
             * Gestion des �v�nements propre � la sc�ne
             */
            virtual void personalEvent(SDL_Event event) = 0;

            /**
             * Gestion des animations
             */
            virtual void animation() = 0;

            /**
             * Gestion des �v�nements commun � toutes les sc�nes
             */
            void event();

            /**
             * Rendu de la sc�ne
             */
            void render();

            /**
             * Construit tous les objets
             */
            void finish();

            static int nextState;
            static int stateID;

        protected:

            /**
             * Ajoute un objet
             */
            void addObject(Object *object);

            /**
             * Recherche des objets intersectant un rect
             */
            std::list<Object *> searchByRect(SDL_Rect rect);

            /**
             * Recherche des objets intersectant un point
             */
            std::list<Object *> searchByPoint(SDL_Point point);

            std::list<Object*> objectList;

            /**
             * Adresse de l'objet ayant le focus
             */
            Object* focusObject;
        private:



    };
}

#endif // STATE_H
