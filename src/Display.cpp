#include "Display.h"
#include "evolution.h"

#include <SDL2/SDL_image.h>

#include <iostream>
#include <sstream>

int Display::fullscreen = Fullscreen::FULLSCREEN_OFF;
SDL_Window* Display::window = NULL;
SDL_Renderer* Display::renderer = NULL;
evo::TextureCollection* Display::textureCollection = NULL;
int Display::width = 1200;
int Display::height = 800;

/**
 * Passage en plein écran
 * // TODO: trouver une meilleur manière de faire pour mettre à jour le renderer
 */
void Display::ToggleFullscreen()
{
    // déterminer le mode
    bool IsFullscreen = SDL_GetWindowFlags(Display::window) & SDL_WINDOW_FULLSCREEN_DESKTOP;

    // changer le mode
    if (IsFullscreen == true)
    {
        SDL_SetWindowFullscreen(Display::window, 0);
    }
    else
    {
        SDL_SetWindowFullscreen(Display::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    // mettre à jour la hauteur et la largeur
    SDL_GetWindowSize(Display::window, &Display::width, &Display::height);

    // recréer le renderer
    SDL_DestroyRenderer(Display::renderer);
    Display::renderer = SDL_CreateRenderer(Display::window, -1, SDL_RENDERER_ACCELERATED);

    // recharger les assets
    delete Display::textureCollection;
    Display::LoadTextureCollection();
}

/**
 * Création de la fenêtre principale
 */
void Display::CreateWindow()
{
    Display::window = SDL_CreateWindow("Evolution",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                Display::width,
                                Display::height,
                                SDL_WINDOW_OPENGL);

    if ( Display::window == NULL ) {
        std::cout << "Error while creating main window" << std::endl;
    }

    Display::renderer = SDL_CreateRenderer(Display::window, -1, SDL_RENDERER_ACCELERATED);
}

/**
 * Chargement de la collection de texture
 */
void Display::LoadTextureCollection()
{
    // LOAD TEXTURE
    Display::textureCollection = new evo::TextureCollection();

    try
    {
        Display::textureCollection->loadTexture("grass","resource/freeland/big/terrains/g.png");
        Display::textureCollection->loadTexture("sea","resource/freeland/big/terrains/s.png");
        Display::textureCollection->loadTexture("toundra","resource/freeland/big/terrains/t.png");
        Display::textureCollection->loadTexture("swamp","resource/freeland/big/terrains/p.png");
        Display::textureCollection->loadTexture("artic","resource/freeland/big/terrains/a.png");
        Display::textureCollection->loadTexture("desert","resource/freeland/big/terrains/d.png");
        Display::textureCollection->loadTexture("hill","resource/freeland/big/terrains/hills.png");
        Display::textureCollection->loadTexture("mountain","resource/freeland/big/terrains/mountains.png");
        Display::textureCollection->loadTexture("forest","resource/freeland/big/terrains/forests.png");
        Display::textureCollection->loadTexture("jungle","resource/freeland/big/terrains/jungles.png");
        Display::textureCollection->loadTexture("grid","resource/freeland/big/grid.png");
        Display::textureCollection->loadTexture("units","resource/freeland/big/units.png");
        Display::textureCollection->loadTexture("walkingMan","resource/textures/platformer_sprites_base.png");
        Display::textureCollection->loadTexture("lightmap","resource/lightmap.png");
        Display::textureCollection->loadTexture("building","resource/portail.png");
    }
    catch (std::string const& error)
    {
        std::cerr << error << std::endl;
    }


    Display::textureCollection->setTextureAlphaMod("temperature",50);
}

/**
 * Creation d'une texture
 */
SDL_Texture* Display::CreateTexture(int w,int h)
{
    return SDL_CreateTexture(Display::renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);
}

/**
 * Création d'une surface
 */
SDL_Surface* Display::CreateSurface(int w, int h, SDL_Color color)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, color.r, color.g, color.b, 0);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
    return surface;
}

/**
 * Charge une texture à partir d'un fichier image
 */
SDL_Texture* Display::LoadImage(std::string filename)
{
    SDL_Texture* texture = IMG_LoadTexture(Display::renderer, filename.c_str());
    if (texture == nullptr)
    {
        throw std::string("Error loading texture : " + std::string(IMG_GetError()));
    }
    return texture;
}

/**
 * Choisi le blend mode d'une texture
 * SDL_BLENDMODE_NONE : no blending
 * SDL_BLENDMODE_BLEND : Alpha blending
 * SDL_BLENDMODE_ADD : Additive blending
 * SDL_BLENDMODE_MOD : Color modulation
 */
void Display::SetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode mode)
{
    SDL_SetTextureBlendMode(texture,mode);
}

/**
 * Création d'une texture à partir d'une surface
 */
SDL_Texture* Display::CreateTextureFromSurface(SDL_Surface* surface)
{
    SDL_Texture* texture;
    texture = SDL_CreateTextureFromSurface(Display::renderer, surface);

    if (texture == NULL)
    {
        evolution::log("CreateTextureFromSurface failed");
    }

    return texture;
}

/**
 * Applique une texture sur une autre
 */
void Display::ApplyTexture(SDL_Texture* texture_src, SDL_Rect rect_src, SDL_Texture* texture_dest, SDL_Rect rect_dest)
{
    SDL_SetRenderTarget(Display::renderer, texture_dest);
    if ( SDL_RenderCopy(Display::renderer, texture_src, &rect_src, &rect_dest) < 0 )
    {
        evolution::log("Erreur ApplyTexture");
    }
}

/**
 * Applique une couleur sur une texture
 */
void Display::ApplyColor(SDL_Texture* texture, SDL_Color color)
{
    SDL_SetRenderTarget(Display::renderer, texture);
    SDL_SetRenderDrawColor(Display::renderer, color.r, color.g, color.b, color.a);

    if ( SDL_RenderClear(Display::renderer) < 0 )
    {
        evolution::log("Erreur ApplyColor");
    }
}

/**
 * Applique une couleur sur une partie de la texture
 */
void Display::ApplyColorRect(SDL_Texture* texture, SDL_Rect rect, SDL_Color color)
{
    SDL_SetRenderTarget(Display::renderer, texture);
    SDL_SetRenderDrawColor(Display::renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(Display::renderer,&rect);
}

/**
 * Dessine une ligne sur une texture
 */
void Display::DrawLine(SDL_Texture *texture, SDL_Color color, SDL_Point a, SDL_Point b)
{
    SDL_SetRenderTarget(Display::renderer, texture);
    SDL_SetRenderDrawColor(Display::renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(Display::renderer,a.x,a.y,b.x,b.y);
}

/**
 * Nettoie une texture
 */
void Display::ClearTexture(SDL_Texture* texture)
{
    SDL_SetRenderTarget(Display::renderer, texture);
    SDL_SetRenderDrawColor(Display::renderer, 0, 0, 0, 255);
    if ( SDL_RenderClear(Display::renderer) < 0 )
    {
        evolution::log("Erreur ClearTexture");
    }
}

/**
 * Prépare une chaine de caractère pour affichage d'un SDL_Rect
 */
std::string Display::RectToStr(SDL_Rect rect)
{
    std::ostringstream str;
    str << rect.x << ',' << rect.y << ',' << rect.w << ',' << rect.h;
    return str.str();
}

/**
 * Prépare une chaine de caractère pour affichage d'un SDL_Point
 */
std::string Display::PointToStr(SDL_Point point)
{
    std::ostringstream str;
    str << point.x << ',' << point.y;
    return str.str();
}


/**
 * Determine si un Rect intersect l'écran
 */
bool Display::IsOnScreen(SDL_Rect rect)
{
    SDL_Rect screen = {0,0,Display::width, Display::height};
    SDL_Rect result;
    return SDL_IntersectRect(&rect,&screen, &result);
}

void Display::ScaleRect(SDL_Rect* rect, float scale)
{
    (*rect).w *= scale;
    (*rect).h *= scale;
}
