#ifndef SOUNDCOLLECTION_H
#define SOUNDCOLLECTION_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

namespace evo
{
    class SoundCollection
    {
        public:
            SoundCollection();
            virtual ~SoundCollection();
            void loadSound(std::string name, std::string filename);
            Mix_Music* getSound(std::string name);
            void playSound(std::string name);
        protected:
        private:
            std::map<std::string,Mix_Music*> collection;
    };
}
#endif // SOUNDCOLLECTION_H
