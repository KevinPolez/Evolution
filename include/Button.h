#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL_ttf.h>

#include "Label.h"

namespace evo
{

    class Button: public Object
    {
        public:
            Button(int width, int height);
            virtual ~Button();

            void setText(std::string text);
            void setFontColor(SDL_Color color);
            void setFontSize(int fontSize);
            void setActiveColor(SDL_Color color);
            void setPassiveColor(SDL_Color color);

            void buildTextTexture();

            // override method
            bool mouseEnterEvent();
            bool mouseLeaveEvent();
            bool mouseLeftClickEvent(SDL_Point p);
            void draw();

        protected:
        private:
            SDL_Color fontColor;
            int fontSize;
            std::string text;
            TTF_Font *font;

            SDL_Color activeColor;
            SDL_Color passiveColor;

            SDL_Texture *textTexture;
            int textW, textH;
            bool textNeedRedraw;
    };
}
#endif // BUTTON_H
