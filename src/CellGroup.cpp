#include "CellGroup.h"
#include "Display.h"

namespace evo
{

    CellGroup::CellGroup()
    {
        this->biome1 = TileBiome::Sea;
        this->biome2 = TileBiome::Sea;
        this->biome3 = TileBiome::Sea;
        this->biome4 = TileBiome::Sea;

        this->elevation1 = TileElevation::Null;
        this->elevation2 = TileElevation::Null;
        this->elevation3 = TileElevation::Null;
        this->elevation4 = TileElevation::Null;

        this->vegetation1 = TileVegetation::None;
        this->vegetation2 = TileVegetation::None;
        this->vegetation3 = TileVegetation::None;
        this->vegetation4 = TileVegetation::None;
    }

    CellGroup::~CellGroup()
    {

    }

    TileBiome CellGroup::getBiome1()
    {
        return this->biome1;
    }
    TileBiome CellGroup::getBiome2()
    {
        return this->biome2;
    }
    TileBiome CellGroup::getBiome3()
    {
        return this->biome3;
    }
    TileBiome CellGroup::getBiome4()
    {
        return this->biome4;
    }

    void CellGroup::setBiome1(TileBiome biome)
    {
        this->biome1 = biome;
    }
    void CellGroup::setBiome2(TileBiome biome)
    {
        this->biome2 = biome;
    }
    void CellGroup::setBiome3(TileBiome biome)
    {
        this->biome3 = biome;
    }
    void CellGroup::setBiome4(TileBiome biome)
    {
        this->biome4 = biome;
    }

    TileElevation CellGroup::getElevation1()
    {
        return this->elevation1;
    }
    TileElevation CellGroup::getElevation2()
    {
        return this->elevation2;
    }
    TileElevation CellGroup::getElevation3()
    {
        return this->elevation3;
    }
    TileElevation CellGroup::getElevation4()
    {
        return this->elevation4;
    }

    void CellGroup::setElevation1(TileElevation elevation)
    {
            this->elevation1 = elevation;
    }
    void CellGroup::setElevation2(TileElevation elevation)
    {
            this->elevation2 = elevation;
    }
    void CellGroup::setElevation3(TileElevation elevation)
    {
            this->elevation3 = elevation;
    }
    void CellGroup::setElevation4(TileElevation elevation)
    {
            this->elevation4 = elevation;
    }

    TileVegetation CellGroup::getVegetation1()
    {
        return this->vegetation1;
    }

    TileVegetation CellGroup::getVegetation2()
    {
        return this->vegetation2;
    }

    TileVegetation CellGroup::getVegetation3()
    {
        return this->vegetation3;
    }

    TileVegetation CellGroup::getVegetation4()
    {
        return this->vegetation4;
    }

    void CellGroup::setVegetation1(TileVegetation vegetation)
    {
        this->vegetation1 = vegetation;
    }
    void CellGroup::setVegetation2(TileVegetation vegetation)
    {
        this->vegetation2 = vegetation;
    }
    void CellGroup::setVegetation3(TileVegetation vegetation)
    {
        this->vegetation3 = vegetation;
    }
    void CellGroup::setVegetation4(TileVegetation vegetation)
    {
        this->vegetation4 = vegetation;
    }

    /**
     * Fourni la texture en fonction du biome principal
     */
    SDL_Texture* CellGroup::findTexture()
    {
        switch (this->biome1)
        {
            case TileBiome::Grass :
                return Display::textureCollection->getTexture("grass");
            case TileBiome::Sea :
                return Display::textureCollection->getTexture("sea");
            case TileBiome::Artic :
                return Display::textureCollection->getTexture("artic");
            case TileBiome::Toundra :
                return Display::textureCollection->getTexture("toundra");
            case TileBiome::Desert :
                return Display::textureCollection->getTexture("desert");
            case TileBiome::Swamp :
                return Display::textureCollection->getTexture("swamp");
            default:
                return Display::textureCollection->getTexture("sea");
        }

    }

    SDL_Point CellGroup::getPointFrom(TileVegetation vegetation)
    {
        SDL_Point p;
        std::string tag = "0000";
        if ( vegetation1 == vegetation)
        {
            tag[0] = '1';
        }
        if ( vegetation2 == vegetation)
        {
            tag[1] = '1';
        }
        if ( vegetation3 == vegetation)
        {
            tag[2] = '1';
        }
        if (vegetation4 == vegetation)
        {
            tag[3] = '1';
        }

        if ( tag == "0000")
        {
                p.x = 0;
                p.y = 0;
                return p;
        }
        else if ( tag == "0001")
        {
                p.x = 0;
                p.y = 1;
                return p;
        }
        else if ( tag == "0010")
        {
                p.x = 0;
                p.y = 2;
                return p;
        }
        else if ( tag == "0011")
        {
                p.x = 0;
                p.y = 3;
                return p;
        }
        else if ( tag == "0100")
        {
                p.x = 1;
                p.y = 0;
                return p;
        }
        else if ( tag == "0101")
        {
                p.x = 1;
                p.y = 1;
                return p;
        }
        else if ( tag == "0110")
        {
                p.x = 1;
                p.y = 2;
                return p;
        }
        else if ( tag == "0111")
        {
                p.x = 1;
                p.y = 3;
                return p;
        }
        else if ( tag == "1000")
        {
                p.x = 2;
                p.y = 0;
                return p;
        }
        else if ( tag == "1001")
        {
                p.x = 2;
                p.y = 1;
                return p;
        }
        else if ( tag == "1010")
        {
                p.x = 2;
                p.y = 2;
                return p;
        }
        else if ( tag == "1011")
        {
                p.x = 2;
                p.y = 3;
                return p;
        }
        else if ( tag == "1100")
        {
                p.x = 3;
                p.y = 0;
                return p;
        }
        else if ( tag == "1101")
        {
                p.x = 3;
                p.y = 1;
                return p;
        }
        else if ( tag == "1110")
        {
                p.x = 3;
                p.y = 2;
                return p;
        }
        else if ( tag == "1111")
        {
                p.x = 3;
                p.y = 3;
                return p;
        }
        return p;
    }

    SDL_Point CellGroup::getPointFrom(TileElevation elevation)
    {
        SDL_Point p;
        std::string tag = "0000";
        if ( elevation1 == elevation)
        {
            tag[0] = '1';
        }
        if ( elevation2 == elevation)
        {
            tag[1] = '1';
        }
        if ( elevation3 == elevation)
        {
            tag[2] = '1';
        }
        if (elevation4 == elevation)
        {
            tag[3] = '1';
        }

        if ( tag == "0000")
        {
                p.x = 0;
                p.y = 0;
                return p;
        }
        else if ( tag == "0001")
        {
                p.x = 0;
                p.y = 1;
                return p;
        }
        else if ( tag == "1000")
        {
                p.x = 0;
                p.y = 2;
                return p;
        }
        else if ( tag == "1001")
        {
                p.x = 0;
                p.y = 3;
                return p;
        }
        else if ( tag == "0010")
        {
                p.x = 1;
                p.y = 0;
                return p;
        }
        else if ( tag == "0011")
        {
                p.x = 1;
                p.y = 1;
                return p;
        }
        else if ( tag == "1010")
        {
                p.x = 1;
                p.y = 2;
                return p;
        }
        else if ( tag == "1011")
        {
                p.x = 1;
                p.y = 3;
                return p;
        }
        else if ( tag == "0100")
        {
                p.x = 2;
                p.y = 0;
                return p;
        }
        else if ( tag == "0101")
        {
                p.x = 2;
                p.y = 1;
                return p;
        }
        else if ( tag == "1100")
        {
                p.x = 2;
                p.y = 2;
                return p;
        }
        else if ( tag == "1101")
        {
                p.x = 2;
                p.y = 3;
                return p;
        }
        else if ( tag == "0110")
        {
                p.x = 3;
                p.y = 0;
                return p;
        }
        else if ( tag == "0111")
        {
                p.x = 3;
                p.y = 1;
                return p;
        }
        else if ( tag == "1110")
        {
                p.x = 3;
                p.y = 2;
                return p;
        }
        else if ( tag == "1111")
        {
                p.x = 3;
                p.y = 3;
                return p;
        }
        return p;
    }

    SDL_Point CellGroup::getPoint()
    {
        SDL_Point p;

        int i = 0,j =0, k=0;
        switch ( biome2 )
        {
            case TileBiome::Sea : i = 0; break;
            case TileBiome::Grass : i = 6; break;
            case TileBiome::Swamp : i = 12; break;
            case TileBiome::Desert : i = 18; break;
            case TileBiome::Toundra : i = 24; break;
            case TileBiome::Artic : i = 30; break;
            default : i = 0;
        }
        switch ( biome3 )
        {
            case TileBiome::Sea : k = 0; break;
            case TileBiome::Grass : k = 1; break;
            case TileBiome::Swamp : k = 2; break;
            case TileBiome::Desert : k = 3; break;
            case TileBiome::Toundra : k = 4; break;
            case TileBiome::Artic : k = 5; break;
            default : k = 0;
        }
        switch ( biome4 )
        {
            case TileBiome::Sea : j = 0; break;
            case TileBiome::Grass : j = 1; break;
            case TileBiome::Swamp: j = 2; break;
            case TileBiome::Desert : j = 3; break;
            case TileBiome::Toundra: j = 4; break;
            case TileBiome::Artic : j = 5; break;
            default : j = 0;
        }
        p.x = k+i;
        p.y = j;
        return p;
    }
}
