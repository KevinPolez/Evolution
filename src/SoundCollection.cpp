#include "SoundCollection.h"

namespace evo
{
    SoundCollection::SoundCollection()
    {

    }

    SoundCollection::~SoundCollection()
    {
        for (auto itr = collection.begin(); itr != collection.end(); ++itr)
        {
            Mix_FreeMusic((*itr).second);
        }
    }

    void SoundCollection::loadSound(std::string name, std::string filename)
    {
        Mix_Music* music = Mix_LoadMUS(filename.c_str());
        collection.insert(std::pair<std::string,Mix_Music*>(name,music));
    }

    void SoundCollection::playSound(std::string name)
    {
        Mix_PlayMusic(collection[name], 1);
    }

    Mix_Music* SoundCollection::getSound(std::string name)
    {
        return collection[name];
    }

}
