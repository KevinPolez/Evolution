#ifndef TEXTURECOLLECTION_H
#define TEXTURECOLLECTION_H

#include <SDL2/SDL.h>

#include <iostream>
#include <map>

namespace evo
{
    class TextureCollection
    {
        public:
            TextureCollection();
            virtual ~TextureCollection();
            void loadTexture(std::string name, std::string filename);
            SDL_Texture* getTexture(std::string name);
            void setTextureAlphaMod(std::string name, Uint8 alpha);
        protected:
        private:
            std::map<std::string,SDL_Texture*> collection;
    };
}
#endif // TEXTURECOLLECTION_H
