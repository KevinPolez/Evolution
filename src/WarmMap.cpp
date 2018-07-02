#include "WarmMap.h"

WarmMap::WarmMap(int width, int height) : Map(width,height)
{
}

void WarmMap::generate()
{
    // génére du bruit
    this->generateNoize(-30,35);
    this->generateGradiant(-30,35,0,getWidth()/2);
    this->generateGradiant(35,-30,getWidth()/2,getWidth());

    // parcour toute la carte pour appliquer un algorithme cellulaire
    /*int index;
    for (index = 0; index < this->getMapSize(); index++)
    {
        if ( this->isNorthEdge(index)
            || this->isNorthEdge(index - (getHeight()))
            || this->isNorthEdge(index - (getHeight()*2))
            || this->isNorthEdge(index - (getHeight()*3)))
        {
            this->set(index,(int) ((rand() % 25) -30));
        }
        else
        {
            this->set(index,(int) ((rand() % 40) -5));
        }
    }

    int artic, toundra, grass, swamp, desert;
    for (index= 0; index < this->getMapSize(); index++)
    {
        artic = 0;
        toundra = 0;
        grass = 0;
        swamp = 0;
        desert = 0;

        if ( ! this->isEdge(index))
        {
            if ( this->get(index - 1) < -5 ) artic++;
            else if ( this->get(index - 1) < 10 ) toundra++;
            else if ( this->get(index - 1) < 25 ) grass++;
            else if ( this->get(index - 1) < 30 ) swamp++;
            else if ( this->get(index - 1) >= 30 ) desert++;

            if ( this->get(index - this->getWidth() - 1) < -5 ) artic++;
            else if ( this->get(index - this->getWidth() - 1) < 10 ) toundra++;
            else if ( this->get(index - this->getWidth() - 1) < 25 ) grass++;
            else if ( this->get(index - this->getWidth() - 1) < 30 ) swamp++;
            else if ( this->get(index - this->getWidth() - 1) >= 30 ) desert++;

            if ( this->get(index - this->getWidth() + 1) < -5 ) artic++;
            else if ( this->get(index - this->getWidth() + 1) < 10 ) toundra++;
            else if ( this->get(index - this->getWidth() + 1) < 25 ) grass++;
            else if ( this->get(index - this->getWidth() + 1) < 30 ) swamp++;
            else if ( this->get(index - this->getWidth() + 1) >= 30 ) desert++;

            if ( this->get(index - this->getWidth()) < -5 ) artic++;
            else if ( this->get(index - this->getWidth()) < 10 ) toundra++;
            else if ( this->get(index - this->getWidth()) < 25 ) grass++;
            else if ( this->get(index - this->getWidth()) < 30 ) swamp++;
            else if ( this->get(index - this->getWidth()) >= 30 ) desert++;

            if ( this->get(index +1) < -5 ) artic++;
            else if ( this->get(index + 1) < 10 ) toundra++;
            else if ( this->get(index + 1) < 25 ) grass++;
            else if ( this->get(index + 1) < 30 ) swamp++;
            else if ( this->get(index + 1) >= 30 ) desert++;

            if ( this->get(index + this->getWidth()) < -5 ) artic++;
            else if ( this->get(index + this->getWidth()) < 10 ) toundra++;
            else if ( this->get(index + this->getWidth()) < 25 ) grass++;
            else if ( this->get(index + this->getWidth()) < 30 ) swamp++;
            else if ( this->get(index + this->getWidth()) >= 30 ) desert++;

            if ( this->get(index + this->getWidth()-1) < -5 ) artic++;
            else if ( this->get(index + this->getWidth()-1) < 10 ) toundra++;
            else if ( this->get(index + this->getWidth()-1) < 25 ) grass++;
            else if ( this->get(index + this->getWidth()-1) < 30 ) swamp++;
            else if ( this->get(index + this->getWidth()-1) >= 30 ) desert++;

            if ( this->get(index + this->getWidth() +1 ) < -5 ) artic++;
            else if ( this->get(index + this->getWidth() +1) < 10 ) toundra++;
            else if ( this->get(index + this->getWidth()+1) < 25 ) grass++;
            else if ( this->get(index + this->getWidth()+1) < 30 ) swamp++;
            else if ( this->get(index + this->getWidth()+1) >= 30 ) desert++;

            if (artic >= 3) this->set(index,(int) ((rand() % 25) - 30));
            if (toundra >= 3) this->set(index, (int) ((rand() % 15) -5));
            if (grass >= 3) this->set(index, (int) ((rand() % 10) + 15));
            if (swamp >= 3) this->set(index, (int) ((rand() % 5) +25));
            if (desert > 4) this->set(index, (int) ((rand() % 5) +30));
        }
    }*/
}
