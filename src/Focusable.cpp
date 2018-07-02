#include "Focusable.h"

namespace evo
{
    Focusable::Focusable(): focus(false)
    {

    }

    bool Focusable::getFocus()
    {
        return this->focus;
    }

    void Focusable::setFocus(bool focus)
    {
        this->focus = focus;
    }

}
