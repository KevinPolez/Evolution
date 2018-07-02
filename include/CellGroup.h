#ifndef CELLGROUP_H
#define CELLGROUP_H

#include "Tile.h"

namespace evo
{

    class CellGroup
    {
        public :
            CellGroup();
            ~CellGroup();

            void setBiome1(TileBiome biome);
            void setBiome2(TileBiome biome);
            void setBiome3(TileBiome biome);
            void setBiome4(TileBiome biome);

            TileBiome getBiome1();
            TileBiome getBiome2();
            TileBiome getBiome3();
            TileBiome getBiome4();

            void setElevation1(TileElevation elevation);
            void setElevation2(TileElevation elevation);
            void setElevation3(TileElevation elevation);
            void setElevation4(TileElevation elevation);

            TileElevation getElevation1();
            TileElevation getElevation2();
            TileElevation getElevation3();
            TileElevation getElevation4();

            void setVegetation1(TileVegetation vegetation);
            void setVegetation2(TileVegetation vegetation);
            void setVegetation3(TileVegetation vegetation);
            void setVegetation4(TileVegetation vegetation);

            TileVegetation getVegetation1();
            TileVegetation getVegetation2();
            TileVegetation getVegetation3();
            TileVegetation getVegetation4();

            SDL_Texture* findTexture();

            SDL_Point getPoint();
            SDL_Point getPointFrom(TileElevation elevation);
            SDL_Point getPointFrom(TileVegetation vegetation);
        private :
            TileBiome biome1;
            TileBiome biome2;
            TileBiome biome3;
            TileBiome biome4;

            TileElevation elevation1;
            TileElevation elevation2;
            TileElevation elevation3;
            TileElevation elevation4;

            TileVegetation vegetation1;
            TileVegetation vegetation2;
            TileVegetation vegetation3;
            TileVegetation vegetation4;
    };
}

#endif // CELLGROUP_H
