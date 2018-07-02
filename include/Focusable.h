#ifndef FOCUSABLE_H
#define FOCUSABLE_H

namespace evo
{
    class Focusable
    {
        public:
            Focusable();
            virtual ~Focusable();

            /**
             * Indique si l'objet à le focus
             */
            bool getFocus();

            /**
             * Change le focus de l'objet
             */
            void setFocus(bool focus);
        protected:
        private:
            bool focus;
    };
}
#endif // FOCUSABLE_H
