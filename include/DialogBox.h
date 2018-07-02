#ifndef BOX_H
#define BOX_H

#include "ObjectText.h"

namespace evo
{
    class Box : public ObjectText
    {
         public:
            Box(int width, int height);
            ~Box();

            //override method
            void draw();
            bool mouseEnterEvent();
            bool mouseLeaveEvent();
            bool mouseLeftClickEvent(SDL_Point p);
            bool mouseLeftDragEvent(SDL_Point p);

        protected:
        private:
    };
}

#endif // BOX_H

