#include "Tile.h"

#include <iostream>

namespace evo
{

    Tile::Tile() :
        index(0),
        temperature(0),
        hygrometrie(0),
        height(1),
        ressource(0),
        backgroundColor({0,103,38}),
        building(nullptr),
        unit(nullptr)
    {
    }

    Tile::~Tile()
    {
    }


    void Tile::setIndex(int index)
    {
        this->index = index;
    }

    int Tile::getIndex() const
    {
        return this->index;
    }

    void Tile::setBiome(TileBiome biome)
    {
        this->biome = biome;
    }

    void Tile::setElevation(TileElevation elevation)
    {
        this->elevation = elevation;
    }

    void Tile::setVegetation(TileVegetation vegetation)
    {
        this->vegetation = vegetation;
    }

    TileBiome Tile::getBiome() const
    {
        return this->biome;
    }

    TileElevation Tile::getElevation() const
    {
        return this->elevation;
    }

    TileVegetation Tile::getVegetation() const
    {
        return this->vegetation;
    }

    void Tile::setBackgroundColor(SDL_Color color) {
        this->backgroundColor = color;
    }

    void Tile::compute(int boardWidth, int boardHeight)
    {
    }

    void Tile::setHeight(int height)
    {
        this->height = height;
    }

    int Tile::getHeight() const
    {
        return this->height;
    }

    void Tile::setTemperature(int temperature)
    {
        this->temperature = temperature;
    }

    void Tile::setHygrometrie(int hygrometrie)
    {
        this->hygrometrie = hygrometrie;
    }

    void Tile::setRessource(int ressource)
    {
        this->ressource = ressource;
    }

    void Tile::setCoord(int x, int y)
    {
        this->x = x;
        this->y = y;

    }

    void Tile::setCoord(SDL_Point p)
    {
        this->x = p.x;
        this->y = p.y;

    }

    int Tile::getX() const
    {
        return this->x;
    }

    int Tile::getY() const
    {
        return this->y;
    }

    /**
     * Fourni la temperature de la case
     */
    int Tile::getTemperature() const
    {
        return this->temperature;
    }

    /**
     * Fourni l'hygrometrie de la case
     */
    int Tile::getHygrometrie() const
    {
        return this->hygrometrie;
    }

    /**
     * Récupére la quantité de ressource de cette case
     */
    int Tile::getRessource() const
    {
        return this->ressource;
    }

    /**
     * Attache une unité à une case
     */
    void Tile::setUnit(evo::Unit *unit)
    {
        this->unit = unit;
    }

    /**
     * Fourni l'unité attaché à cette case
     */
    Unit* Tile::getUnit() const
    {
        return this->unit;
    }

    /**
     * Indique si un batiment est présent sur la case
     */
    bool Tile::hasUnit() const
    {
        if ( this->unit != nullptr) return true;
        return false;
    }

    /**
     * Attache un batiment à une case
     * - Si un batiment était déjà présent, le batiment précédent est détruit
     */
    void Tile::setBuilding(Building *building)
    {
        this->building = building;
    }

    /**
     * Récupére le batiment présent sur la case
     */
    Building* Tile::getBuilding() const
    {
        return this->building;
    }

    /**
     * Indique si un batiment est présent sur la case
     */
    bool Tile::hasBuilding() const
    {
        if ( this->building != nullptr) return true;
        return false;
    }
}
