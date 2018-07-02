#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#include "TextureCollection.h"

enum Fullscreen
{
    FULLSCREEN_OFF,
    FULLSCREEN_ON
};

class Display {
    public :
        static int fullscreen;
        static int width;
        static int height;
        static SDL_Window* window;
        static SDL_Renderer* renderer;
        static evo::TextureCollection* textureCollection;

        static void ToggleFullscreen();
        static void CreateWindow();
        static void LoadTextureCollection();
        static SDL_Texture* CreateTexture(int w, int h);
        static SDL_Surface* CreateSurface(int w, int h, SDL_Color color);
        static SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
        static SDL_Texture* LoadImage(std::string filename);
        static void SetTextureBlendMode(SDL_Texture *texture, SDL_BlendMode mode);
        static void ApplyTexture(SDL_Texture* texture_src, SDL_Rect rect_src, SDL_Texture* texture_dest, SDL_Rect rect_dest);
        static void ApplyColor(SDL_Texture* texture, SDL_Color color);
        static void ApplyColorRect(SDL_Texture* texture, SDL_Rect rect, SDL_Color color);
        static void DrawLine(SDL_Texture* texture, SDL_Color color, SDL_Point a, SDL_Point b);
        static void ClearTexture(SDL_Texture *texture);
        // TODO : utiliser la surcharge de l'opérateur de flux à la place
        static std::string RectToStr(SDL_Rect rect);
        static std::string PointToStr(SDL_Point point);
        static bool IsOnScreen(SDL_Rect rect);
        static void ScaleRect(SDL_Rect* rect, float scale);
};
#endif // DISPLAY_H
