#ifndef OBJECTTEXT_H
#define OBJECTTEXT_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Object.h"

namespace evo
{
    class ObjectText : public Object
    {
        public:
            ObjectText();
            virtual ~ObjectText();

            void setText(std::string text);
            void addParagraphe(std::string paragraphe);

            void setParagrapheMargin(int paragrapheMargin);
            int getParagrapheMargin();

            void setFontColor(SDL_Color fontColor);
            void setFontSize(int fontSize);

            void setTextNeedRedraw(bool textNeedRedraw);
            bool getTextNeedRedraw();

            int getTextHeight();
            int getTextWidth();

            void buildTextTexture();

            SDL_Texture* getTextTexture();
        protected:
        private:
            std::list<std::string> paragraphes;
            int paragrapheMargin;
            SDL_Color fontColor;
            TTF_Font *font;
            int fontSize;

            SDL_Texture *textTexture;
            int textW, textH;
            bool textNeedRedraw;
    };
}
#endif // OBJECTTEXT_H
