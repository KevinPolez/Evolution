#ifndef INPUTTEXT_H
#define INPUTTEXT_H

#include "Object.h"
#include <SDL2/SDL_ttf.h>

namespace evo
{
    class InputText : public Object
    {
        public:
            InputText(int width, int height);
            virtual ~InputText();

            std::string getText();

            void setActiveColor(SDL_Color color);
            void setPassiveColor(SDL_Color color);

            void buildTextTexture();
            void setText(std::string text);
            void setFontColor(SDL_Color color);
            void setFontSize(int fontSize);

            // @override
            void draw();
            bool mouseLeftClickEvent(SDL_Point p);
            bool textInputEvent(char* e);
            bool textBackspaceEvent();
            bool gainFocusEvent();
            bool looseFocusEvent();
        protected:
        private:
            bool inputMode;
            SDL_Color activeColor;
            SDL_Color passiveColor;

            SDL_Color fontColor;
            int fontSize;
            std::string text;
            TTF_Font *font;

            bool textNeedRedraw;
            int textW, textH;
            SDL_Texture *textTexture;
    };
}

#endif // INPUTTEXT_H
