#ifndef SCROLLTEXT_H
#define SCROLLTEXT_H

#include "ObjectText.h"

namespace evo
{
    class ScrollText : public ObjectText
    {
        public:
            ScrollText(int widht, int height);
            ~ScrollText();

            //override method
            void nextAnimationFrame();
            void draw();

        protected:
        private:
            int speed;
            int ticks;
            int textPositionY;
    };
}
#endif // SCROLLTEXT_H
