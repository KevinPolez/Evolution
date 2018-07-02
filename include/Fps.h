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
         * Fourni le temps passé depuis le dernier appel à cette méthode
         */
        Uint32 getElapsedTime();

        /**
         * Fourni le temps passé depuis la dernière frame
         */
        Uint32 frameTimeElapsed();

        /**
         * Indique qu'une frame vient d'ête affichée
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
