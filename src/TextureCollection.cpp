#include "TextureCollection.h"
#include "Display.h"

#include <map>

namespace evo
{

    /**
     * Constructeur
     */
    TextureCollection::TextureCollection()
    {
        //ctor
    }

    /**
     * Destructeur
     * - Appel de SDL_DestroyTexture pour toutes les textures stockée
     */
    TextureCollection::~TextureCollection()
    {
        for (auto itr = collection.begin(); itr != collection.end(); ++itr)
        {
            SDL_DestroyTexture( (*itr).second );
            std::cout << "Texture " << (*itr).first << " is free." << std::endl;
        }

    }

    /**
     * Chargement d'une texture
     */
    void TextureCollection::loadTexture(std::string name, std::string filename)
    {
        SDL_Texture* texture = Display::LoadImage(filename);
        collection.insert(std::pair<std::string, SDL_Texture*> (name,texture));
        std::cout << "Texture " << name << " is load." << std::endl;
    }


    /**
     * Récupére une texture à partir de son nom
     */
    SDL_Texture* TextureCollection::getTexture(std::string name)
    {
        return collection[name];
    }

    /**
     * Determine le mode Alpha d'une texture
     */
    void TextureCollection::setTextureAlphaMod(std::string name, Uint8 alpha)
    {
        SDL_SetTextureAlphaMod(collection[name],alpha);
    }
}
