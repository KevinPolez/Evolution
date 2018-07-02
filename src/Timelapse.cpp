#include "Timelapse.h"

namespace evo
{

    /**
     * Constructeur
     */
    Timelapse::Timelapse() : step(1000), pause(false), speed(1)
    {
    }

    /**
     * Destructeur
     */
    Timelapse::~Timelapse()
    {
    }

    /**
     * Instancie event avec le premier événement de la queue
     * et retire cet événement
     */
    bool Timelapse::getEvent(TimelapseEvent* event)
    {
        if ( this->timelapseEvent.empty() ) return false;
        (*event) = this->timelapseEvent.front();
        this->timelapseEvent.pop_front();
        return true;
    }

    /**
     * Ajoute un événement dans la queue
     * TODO: définir une taille maximum de la file
     */
    void Timelapse::addEvent(TimelapseEvent event)
    {
        this->timelapseEvent.push_back(event);
    }

    void Timelapse::start()
    {
        this->time = 0;
        this->currentTime = SDL_GetTicks();
        this->lastTime = currentTime;
    }

    bool Timelapse::nextTurn()
    {
        if ( this->pause == true ) return false;
        this->currentTime = SDL_GetTicks();
        if ( this->currentTime > this->lastTime + this->step) {
            this->time = this->time + 1;
            this->lastTime = this->currentTime;
            return true;
        }
        return false;
    }

    int Timelapse::getTime() const
    {
        return this->time;
    }


    void Timelapse::speedUp()
    {
        if ( this->setSpeed(speed+1) )
        {
            this->addEvent(TimelapseEvent::SpeedUp);
        }
    }

    void Timelapse::speedDown()
    {
        if ( this->setSpeed(speed-1) )
        {
            this->addEvent(TimelapseEvent::SpeedDown);
        }

    }

    bool Timelapse::setSpeed(int speed)
    {
        if ( speed < 1 || speed > 3 ) return false;
        this->speed = speed;
        switch ( speed)
        {
            case 1:
                this->step = 1000;
                this->addEvent(TimelapseEvent::Speed1);
                break;
            case 2:
                this->step = 2000;
                this->addEvent(TimelapseEvent::Speed2);
                break;
            case 3:
                this->step = 3000;
                this->addEvent(TimelapseEvent::Speed3);
                break;
        }
        return true;

    }

    int Timelapse::getSpeed()
    {
        return this->speed;
    }

    void Timelapse::togglePause()
    {
        this->pause = !this->pause;
        if ( this->pause == true) this->executeCallback(TimelapseEvent::Pause);
    }

    bool Timelapse::isPaused() const
    {
        return this->pause;
    }

    void Timelapse::setPause(bool pause)
    {
        this->pause = pause;
        if ( this->pause == true) this->executeCallback(TimelapseEvent::Pause);
    }

    /**
     * \brief Défini une fonction de callback associé à un event
     */
    void Timelapse::setCallback(TimelapseEvent event, std::function<bool()> callback) {
        callbackMap.insert( std::pair<TimelapseEvent,std::function<bool()> >(event,callback));
    }

    /**
     * \brief Execute une fonction de callback
     */
    void Timelapse::executeCallback(TimelapseEvent event)
    {
        auto it = callbackMap.find(TimelapseEvent::Pause);
        if ( it != callbackMap.end()) {
            (*it).second();
        }
    }
}
