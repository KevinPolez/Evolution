#ifndef TIMELAPSE_H
#define TIMELAPSE_H

#include <SDL2/SDL.h>
#include <map>
#include <functional>
#include <list>

namespace evo
{

    enum class TimelapseEvent { Pause, SpeedUp, SpeedDown, Speed1, Speed2, Speed3 };

    class Timelapse
    {
        public:
            Timelapse();
            virtual ~Timelapse();

            bool getEvent(TimelapseEvent* event);
            void addEvent(TimelapseEvent event);

            int getTime() const;
            bool isPaused() const;

            void speedDown();
            void speedUp();
            bool setSpeed(int speed);
            int getSpeed();
            bool nextTurn();
            void start();
            void togglePause();
            void setPause(bool pause);
            /**
             * Défini une fonction de callback pour un événement
             */
            void setCallback(TimelapseEvent event,std::function<bool()> callback);
            /**
             * Execute une fonction de callback lié à un événement
             */
            void executeCallback(TimelapseEvent event);
        protected:

        private:
            /**
             * Stockage des fonctions de callback
             */
            std::map<TimelapseEvent,std::function<bool()> > callbackMap;
            int time;
            int currentTime;
            int lastTime;
            int step;
            bool pause;
            std::list<TimelapseEvent> timelapseEvent;
            int speed;
    };
}
#endif // TIMELAPSE_H
