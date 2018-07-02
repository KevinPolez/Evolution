#include "Map.h"
#include "Seed.h"

Map::Map(int width, int height) : width(width), height(height), mapSize(width*height)
{
    this->createGenerator();

    int index;
    for (index = 0; index < mapSize; index++)
    {
        data.insert(std::pair<int,int>(index,0));
    }
}

Map::~Map()
{
}

void Map::createGenerator()
{
    generator.seed(evo::Seed::seed);
}

/**
 * Génére du bruit
 */
void Map::generateNoize(int minValue, int maxValue)
{
    this->configureUniformRandomGenerator(minValue,maxValue);

    std::map<int,int>::iterator itr;
    for (itr = data.begin(); itr != data.end(); itr++)
    {
        itr->second = this->random();
    }
}

/**
 * Génére un bruit gaussian
 */
void Map::generateGaussianNoize()
{

}

/**
 * Génére un dégradé entre une ligne de départ et une ligne d'arrivée
 */
void Map::generateGradiant(int startValue, int stopValue, int startLine, int stopLine)
{
    std::map<int,int>::iterator itr;

    int step = stopLine - startLine;
    int diff = (int) (stopValue - startValue) / step;

    for (itr = data.begin(); itr != data.end(); itr++)
    {
        if ( getLine(itr->first) >= startLine
            && getLine(itr->first) <= stopLine )
        {
            itr->second = startValue + ( (getLine(itr->first) - startLine) * diff);
        }
    }
}

/**
 * Applique un algorthime cellulaire
 */
void Map::generateCellularAutomata(int step)
{

}

void Map::configureUniformRandomGenerator(int minValue, int maxValue)
{
    uniformIntDistribution.param(std::uniform_int_distribution<int>::param_type(minValue,maxValue));
}

/**
 * Fourni un nombre aléatoire selon une distribution uniforme
 */
int Map::random()
{
    return uniformIntDistribution(generator);
}

int Map::getWidth()
{
    return this->width;
}

int Map::getHeight()
{
    return this->height;
}

/**
 * Fourni la valeur d'une case
 */
int Map::get(int index)
{
    return this->data[index];
}

/**
 * Determine la valeur d'une case
 */
void Map::set(int index, int value)
{
    this->data[index] = value;
}

/**
 * Fourni la taille de la carte
 */
int Map::getMapSize()
{
    return this->mapSize;
}

/**
 * Insere une valeur dans le tableau de data
 */
void Map::insert(std::pair<int,int> d)
{
    this->data.insert(d);
}

/**
 * fourni la ligne correspondante
 */
int Map::getLine(int index)
{
   return (int) index / height;
}

/**
 * Determine si la case est sur le bord de la carte
 */
bool Map::isEdge(int index)
{
    if ( isNorthEdge(index)
        || isSouthEdge(index)
        || isEastEdge(index)
        || isWestEdge(index) ) return true;
    return false;
}

bool Map::isNorthEdge(int index)
{
    if ( index < this->height ) return true;
    return false;
}

bool Map::isSouthEdge(int index)
{
    if ( index > this->width * (this->height -1)) return true;
    return false;
}

bool Map::isEastEdge(int index)
{
    if ( (index + 1)  % this->width == 0 ) return true;
    return false;
}

bool Map::isWestEdge(int index)
{
     if ( index % this->width == 0 ) return true;
    return false;
}
