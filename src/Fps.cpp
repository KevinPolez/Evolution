#include "Fps.h"

/**
 * Constructeur
 */
Fps::Fps(int maxFps):
    maxFps(maxFps),
    frameCount(0),
    lastTime(SDL_GetTicks()),
    currentTime(0),
    targetTime(0),
    currentFps(0),
    elapsedTime(0),
    frameTime(0),
    currentFps(0)
{
    this->maxFrameTicks = (1000.0/(float)this->maxFps) + 0.00001;
}

/**
 * Destructeur
 */
Fps::~Fps()
{
}

/**
 * Met en place le limitateur de FPS
 */
void Fps::setLimit(int maxFps) {
    this->maxFps = maxFps;
    this->maxFrameTicks = (1000.0/(float)this->maxFps) + 0.00001;
}

/**
 * Fourni la valeur du FPS
 */
int Fps::getCurrentFps()
{
    return this->currentFps;
}

/**
 * Limite au nombre de FPS déterminé
 */
void Fps::limit() {

    frameCount++;
    targetTime = lastTime + Uint32(frameCount * maxFrameTicks);
    currentTime = SDL_GetTicks();

    if ( currentTime < targetTime) {
        SDL_Delay(targetTime - currentTime);
        currentTime = SDL_GetTicks();
    }

    if (currentTime - lastTime >= 1000) {
        this->currentFps = frameCount;
        frameCount = 0;
        lastTime = SDL_GetTicks();
    }
}

/**
 * Fourni le temps passé depuis la dernière frame
 */
Uint32 Fps::frameTimeElapsed()
{
    return SDL_GetTicks() - this->frameTime;
}

void Fps::setFrameTime()
{
    this->frameTime = SDL_GetTicks();
}

/**
 * Fourni le temps passé depuis le dernier appel à cette méthode
 */
/*Uint32 Fps::getElapsedTime()
{
    Uint32 delay = SDL_GetTicks() - this->lastElapsedTime;
    this->lastElapsedTime = SDL_GetTicks();
    return delay;
}*/

/**
 * Fourni le temps courant
 */
Uint32 Fps::getCurrentTime()
{
    return SDL_GetTicks();
}
