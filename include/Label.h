#ifndef LABEL_H
#define LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Object.h"

namespace evo
{
    class Label : public Object
    {
        public:
            Label();
            virtual ~Label();
            void setText(std::string text);
            void setFontColor(SDL_Color color);
            void setFontSize(int fontSize);
            void setBackgroundTransparency(int transparency);

            // override method
            void draw();
        protected:
            void reloadFont();
        private:
             std::string text;
             SDL_Color fontColor;
             TTF_Font *font;
             int fontSize;
    };
}
#endif // LABEL_H
