#ifndef FPS_H
#define FPS_H

#include <SDL2/SDL.h>


class Fps
{
    public:
        Fps(int limit);
        virtual ~Fps();
        void setLimit(int maxFps);

        /**
         *
         */
        void limit();

        /**
         * Fourni la valeur Fps
         */
        int getCurrentFps();

        /**
         * Fourni le temps pass� depuis le dernier appel � cette m�thode
         */
        Uint32 getElapsedTime();

        /**
         * Fourni le temps pass� depuis la derni�re frame
         */
        Uint32 frameTimeElapsed();

        /**
         * Indique qu'une frame vient d'�te affich�e
         */
        void setFrameTime();

        /**
         * Fourni le temps courant
         */
        Uint32 getCurrentTime();

    protected:
    private:
        int maxFps;
        int frameCount;
        Uint32 lastTime, currentTime, targetTime, elapsedTime, frameTime;
        //Uint32 lastElapsedTime;
        int maxFrameTicks;
        int currentFps;
};

#endif // FPS_H
