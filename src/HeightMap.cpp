#include "HeightMap.h"

/**
 * Constructeur
 * La hauteur varie de 0 à 3
 * 0 : mer
 * 1 : plaine
 * 2 : colline
 * 3 : montagne
 */
HeightMap::HeightMap(int width, int height) : Map(width, height)
{
}

void HeightMap::generate()
{
    // génére des valeurs aléatoire
    this->generateNoize(0,3);

    // les bords de la cartes doivent être de l'eau
    int index;
    for (index = 0; index < this->getMapSize(); index++)
    {
        if ( this->isEdge(index))
        {
            this->set(index,0);
        }
    }

    // parcourt toute la carte pour appliquer un algorithme cellulaire
    for (index= 0; index < this->getMapSize(); index++)
    {
        int sea = 0;
        int grass = 0;
        int hill = 0;
        int mountain = 0;
        if ( ! this->isEdge(index))
        {
            switch (this->get(index - 1))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index - getWidth() - 1))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index - getWidth() + 1))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index - getWidth()))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index + 1))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index + getWidth()))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index + getWidth() - 1))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }
            switch (this->get(index + getWidth() + 1))
            {
                case 0 : sea++; break;
                case 1 : grass++; break;
                case 2 : hill++; break;
                case 3 : mountain++; break;
            }

            if ( this->get(index) == 0 && sea < 3 ) this->set(index,1);
            if ( this->get(index) == 2 && hill < 3 ) this->set(index,1);
            if ( this->get(index) == 3 && mountain < 3 ) this->set(index,1);
        }
    }
}
