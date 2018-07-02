#ifndef VERTICALLAYOUT_H
#define VERTICALLAYOUT_H

#include <Object.h>

namespace evo
{
    class VerticalLayout : public Object
    {
        public:

            /**
             * Constructeur
             */
            VerticalLayout(int width, int height);

            /**
             * Alignement central des objets contenu dans la boite
             */
            void setCenter(bool center);

            /**
             * Padding vertical des objets contenu dans la boite
             */
            void setVerticalMargin(int verticalMargin);

            // @override
            void draw();
        protected:
        private:
            bool center;
            int verticalMargin;
    };
}
#endif // VERTICALLAYOUT_H
