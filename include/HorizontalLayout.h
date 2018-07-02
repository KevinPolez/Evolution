#ifndef HORIZONTALLAYOUT_H
#define HORIZONTALLAYOUT_H

#include "Object.h"

namespace evo
{
    class HorizontalLayout : public Object
    {
        public :
            HorizontalLayout(int width, int height);
            ~HorizontalLayout();

            void setCenter(bool center);
            void setHorizontalMargin(int horizontalMargin);
            void setPadding(int padding);

            // @override
            void draw();
        protected:
        private:
            int horizontalMargin;
            int padding;
            bool center;
    };
}
#endif // HORIZONTALLAYOUT_H
