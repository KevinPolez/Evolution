#ifndef TILE_H
#define TILE_H

#include "Building.h"
#include "Unit.h"

#include <SDL2/SDL.h>

namespace evo
{

    enum class TileBiome { Null, Sea, Artic, Toundra, Grass, Desert, Swamp};
    enum class TileElevation { Null, Sea, Lowland, Hill, Montain};
    enum class TileVegetation { None, Forest, Jungle};

    class Tile
    {
        public:
            Tile();
            virtual ~Tile();

            void setCoord(int x, int y);
            void setCoord(SDL_Point p);

            void setCode(char code);

            int getX() const;
            int getY() const;

            TileBiome getBiome() const;
            TileElevation getElevation() const;
            TileVegetation getVegetation() const;

            void setBiome(TileBiome biome);
            void setElevation(TileElevation elevation);
            void setVegetation(TileVegetation vegetation);

            char getCode();

            void setBackgroundColor(SDL_Color color);

            void setIndex(int index);
            int getIndex() const;
            int getHeight() const;
            void setHeight(int height);

            int getTemperature() const;
            void setTemperature(int temperature);

            int getHygrometrie() const;
            void setHygrometrie(int hygrometrie);

            int getRessource() const;
            void setRessource(int ressource);

            void setBuilding(Building* building);
            Building* getBuilding() const;
            bool hasBuilding() const;

            void setUnit(Unit* unit);
            Unit* getUnit() const;
            bool hasUnit()const;

            void compute(int boardWidth, int boardHeight);
        protected:
        private:
            int x;
            int y;
            int index;

            TileBiome biome;
            TileElevation elevation;
            TileVegetation vegetation;

            char code;
            int temperature;
            int hygrometrie;
            int height;
            int ressource;
            SDL_Color backgroundColor;

            Building* building;
            Unit* unit;
    };
}
#endif // TILE_H
