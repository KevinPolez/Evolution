#include "Board.h"

#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include "Display.h"
#include "evolution.h"
#include "Color.h"
#include "HeightMap.h"
#include "WarmMap.h"
#include "HygroMap.h"
#include "CellGroup.h"
#include "Seed.h"
#include "Unit.h"

namespace evo
{

    /**
     * Constructeur
     */
    Board::Board(int width, int height) :
                    Object(),
                    boardWidth(64),
                    boardHeight(64),
                    tileWidth(90),
                    tileHeight(90),
                    gridColor(Color::Black),
                    backgroundColor(Color::Green),
                    tileSelected(nullptr),
                    freeze(false),
                    zoomLevel(3)
    {
        myType = ObjectType::Board;

        this->setDisplayRect({0,0,width,height});
        this->setScreenRect({0,0,width,height});

        // calcul de la taille de la texture necessaire pour dessiner toute la carte
        width = (boardWidth + 1) * 128;
        height = (boardHeight + 1) * 64;

        this->setWidth(width);
        this->setHeight(height);

        // on place la vue de départ (au centre de la carte)
        SDL_Rect rect = this->getDisplayRect();
        rect.x = width/2 - (displayRect.w /2);
        rect.y = height/2 - (displayRect.h / 2);
        this->setDisplayRect(rect);

        // détermination de la graine
        if (Seed::seed == 0) Seed::generateNewSeed();

        // Créer une HeightMap
        HeightMap* heightMap = new HeightMap(boardWidth, boardHeight);
        heightMap->generate();

        // Créer une WarmMap
        WarmMap* warmMap = new WarmMap(boardWidth, boardHeight);
        warmMap->generate();

        // Créer une HygrometrieMap
        HygroMap* hygroMap = new HygroMap(boardWidth, boardHeight);
        hygroMap->generate();

        int index;
        Tile* tile;

        for (index = 0; index < boardHeight * boardWidth; index++)
        {
            tile = new Tile();
            tile->setIndex(index);
            tile->setTemperature(warmMap->get(index));
            tile->setHeight(heightMap->get(index));
            tile->setHygrometrie(hygroMap->get(index));

            this->computeBiome(tile);
            this->computeElevation(tile);
            this->computeVegetation(tile);

            this->board.insert(std::pair<int,Tile*>(index,tile));
        }

        delete heightMap;
        delete warmMap;
        delete hygroMap;

        this->lightmap = Display::textureCollection->getTexture("lightmap");
        Display::SetTextureBlendMode(this->lightmap,SDL_BLENDMODE_MOD);
    }

    /**
     * destructeur
     */
    Board::~Board()
    {
        for (auto itr = board.begin(); itr != board.end(); ++itr) {
            delete (*itr).second;
        }
    }

    /**
     * Instancie event avec le premier événement de la queue
     * et retire cet événement
     */
    bool Board::getEvent(BoardEvent* event)
    {
        if ( this->boardEvent.empty() ) return false;
        (*event) = this->boardEvent.front();
        this->boardEvent.pop_front();
        return true;
    }

    /**
     * Ajoute un événement dans la queue
     * TODO: définir une taille maximum de la file
     */
    void Board::addEvent(BoardEvent event)
    {
        this->boardEvent.push_back(event);
    }

    /**
     * Fourni le scaleFactor en fonction du zoom
     */
    float Board::getScaleFactor(int zoomLevel)
    {
        switch (zoomLevel)
        {
            case 1: return 0.2f; break;
            case 2: return 0.5f; break;
            case 3: return 1.0f; break;
            case 4: return 1.5f; break;
            case 5: return 2.0f; break;
            default: return 1.0f;
        }
    }

    /**
     * Change de zoom
     * - l'animation de zoom In et de zoom Out prennent un termps prédéfinie (1000 ms)
     * - Durant le temps d'animation, le scaleFactor est incrémenté ou décrémenté
     * - le zoom aura donc une progression rapide au début puis lente sur la fin
     */
    bool Board::setZoomLevel(int zoomLevel)
    {
        if ( zoomLevel < 1 || zoomLevel > 5) return false;
        int oldZoomLevel = this->zoomLevel;
        this->zoomLevel = zoomLevel;

        this->startZoom(getScaleFactor(zoomLevel));

        /*this->setOldScaleFactor(getScaleFactor(oldZoomLevel));
        this->setReachScaleFactor();

        SDL_Rect reachRect;
        reachRect.w = static_cast<int> (((float) displayRect.w / this->getActualScaleFactor())* this->getReachScaleFactor());
        reachRect.h = static_cast<int> (((float) displayRect.h / this->getActualScaleFactor())* this->getReachScaleFactor());

        // correction necessaire pour éviter les imprécisions lié aux nombres à virgules flotantes
        reachRect.w = round((float)reachRect.w / 10)*10;
        reachRect.h = round((float)reachRect.h / 10)*10;

        this->setReachRect(reachRect);*/

        if ( this->getActualScaleFactor() > this->getReachScaleFactor()) this->setZoomIn();
        else this->setZoomOut();

        switch ( this->zoomLevel)
        {
            case 1: this->addEvent(BoardEvent::Zoom1); break;
            case 2: this->addEvent(BoardEvent::Zoom2); break;
            case 3: this->addEvent(BoardEvent::Zoom3); break;
            case 4: this->addEvent(BoardEvent::Zoom4); break;
            case 5: this->addEvent(BoardEvent::Zoom5); break;
        }
        return true;
    }

    /**
     * Freeze ou defreeze le plateau de jeu
     */
    void Board::toggleFreeze()
    {
        this->freeze = ! this->freeze;
        this->draw();
    }

    /**
     * Indique l'état du plateau de jeu
     */
    bool Board::isFreeze()
    {
        return this->freeze;
    }

    /**
     * Fourni une case en fonction de son index
     */
    Tile* Board::getTile(int index) const
    {
        if ( this->board.find(index) == this->board.end() )
        {
            return NULL;
        }

        return this->board.at(index);
    }

    /**
     * Fourni la case selectionnée
     */
    Tile* Board::getSelectedTile() const
    {
        return this->tileSelected;
    }

    /**
     * Défini la case selectionnée
     */
    void Board::setSelectedTile(Tile * tile)
    {
        this->tileSelected = tile;
    }

    /**
     * Calcul le biome en fonction de la temperature
     */
    void Board::computeBiome(Tile* tile)
    {
        tile->setBiome(TileBiome::Grass);

        if ( tile->getTemperature() < -5)
        {
            tile->setBiome(TileBiome::Artic);
        }
        else if ( tile->getTemperature() < 10)
        {
            tile->setBiome(TileBiome::Toundra);
        }
        else if ( tile->getTemperature() < 25 )
        {
            tile->setBiome(TileBiome::Grass);
        }
        else
        {
            tile->setBiome(TileBiome::Desert);
        }
    }

    /**
     * Calcul l'élévation
     */
    void Board::computeElevation(Tile *tile)
    {
        switch ( tile->getHeight())
        {
            case 0:
                tile->setElevation(TileElevation::Sea);
                tile->setBiome(TileBiome::Sea);
                break;
            case 1:
                tile->setElevation(TileElevation::Lowland);
                break;
            case 2:
                tile->setElevation(TileElevation::Hill);
                tile->setBiome(TileBiome::Grass);
                break;
            case 3:
                tile->setElevation(TileElevation::Montain);
                tile->setBiome(TileBiome::Grass);
                break;
            default :
                tile->setElevation(TileElevation::Sea);
                tile->setBiome(TileBiome::Sea);
                break;
        }
    }

    /**
     * Calcul la vegetation en fonction de l'hygrometrie
     */
    void Board::computeVegetation(Tile *tile)
    {
        tile->setVegetation(TileVegetation::None);

        if ( tile->getElevation() == TileElevation::Lowland
          && tile->getHygrometrie() > 60
          && tile->getTemperature() < 25 )
        {
            tile->setVegetation(TileVegetation::Forest);
        }

        if ( tile->getElevation() == TileElevation::Lowland
          && tile->getHygrometrie() > 60
          && tile->getTemperature() >= 25
          && tile->getTemperature() < 30 )
        {
            tile->setVegetation(TileVegetation::Jungle);
        }

    }

    int getTemperature(int side, int A, int B)
    {
        float a = (B-A) / (7-0);
        float b = B - (a*7);

        int temp = 0;
        temp = a * side + b;

        return temp;
    }

    /**
     * Converti un degré en radian
     */
    float Board::degToRad(float deg) {
        return deg * (M_PI / 180);
    }

    /**
     * Converti un radian en degré
     */
    float Board::radToDeg(float rad) {
        return rad * (180 / M_PI);
    }

    /**
     * Projection isometrique
     */
    SDL_Point Board::screenToIso(int x, int y) {
        return this->screenToIso({x,y});
    }

    /**
     * Projection isometrique d'un point de l'écran sur la carte
     */
    SDL_Point Board::screenToIso(SDL_Point p) {
        SDL_Point iso;

        // rotation to 45° = pi/4 rad
        float s = sin(this->degToRad(45.0));
        float c = cos(this->degToRad(45.0));

        iso.x = (p.x * c - p.y * s);
        iso.y = (p.x * s + p.y * c);

        iso.y = iso.y / 2;

        iso.x += (this->getWidth() / 2);
        iso.y += 32;

        return iso;
    }

    /**
     * inverse la projection isometrique
     */
    SDL_Point Board::isoToScreen(SDL_Point iso) {
        SDL_Point p;

        // rotation to 45° = pi/4 rad
        float s = sin(this->degToRad(45.0));
        float c = cos(this->degToRad(45.0));

        // add origin
        iso.x -= (this->getWidth() / 2);
        iso.y -= 32;

        iso.y = iso.y * 2;

        p.x =  (iso.x * c +  iso.y * c);
        p.y = (iso.x * -s + iso.y * s);

        return p;
    }

    SDL_Point Board::addView(SDL_Point p)
    {
        p.x += displayRect.x;
        p.y += displayRect.y;
        return p;
    }

    SDL_Point Board::removeView(SDL_Point p)
    {
        p.x -= displayRect.x;
        p.y -= displayRect.y;
        return p;
    }

    int Board::pointToIndex(SDL_Point p) {
        int index;

        p.x = p.x / tileWidth;
        p.y = p.y / tileHeight;

        index =  p.x + (p.y * boardWidth);

        return index;
    }

    /**
     * Transforme un index de case, en point sur la carte
     */
    SDL_Point Board::indexToPoint(int index) {
        SDL_Point p;

        // find tile coordonate
        p.y = index / boardWidth;
        p.x = index - (p.y * boardWidth);

        p.x = p.x * tileWidth;
        p.y = p.y * tileHeight;

        return p;
    }

    /**
     * dessine la vue
     */
    void Board::draw()
    {
        evolution::log("draw : " + this->getName());

        // si la texture de base n'est pas créée, la créer
        if ( ! this->hasTexture() ) {
            this->createTexture();
        }

        // si le jeu est freezé, modifier simplement l'intensité du rendu en appliquant un calque transparent
        // (la texture est déjà dessiné, inutile de refaire le travail
        if ( this->freeze == true )
        {
            SDL_Texture* calque = Display::CreateTexture(this->displayRect.w, this->displayRect.h);
            Display::SetTextureBlendMode(calque,SDL_BLENDMODE_BLEND);
            Display::ApplyColor(calque,{0,0,0,125});
            Display::ApplyTexture(calque,{0,0,this->displayRect.w, this->displayRect.h},this->texture,this->displayRect);
            SDL_DestroyTexture(calque);
            calque = NULL;
            return;
        }

        // effacer la texture entierement
        this->applyBackgroundColor();

        int key;
        SDL_Rect dest;
        SDL_Rect src;
        SDL_Texture* texture;
        SDL_Point textureLocation;

        // parcours chacune des cases du plateau de jeu
        Tile* tile;
        SDL_Point p;
        CellGroup cellGroup;
        std::map<int,Tile*>::iterator itr;

        // applique le terrain
        for (itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            tile = ((*itr).second);

            cellGroup = this->computeTag(tile);

            // point de référence
            p = this->indexToPoint(key);

            // choisie la texture
            texture = cellGroup.findTexture();

            SDL_Point pointFromTag = cellGroup.getPoint();
            src.x = 128*pointFromTag.y;
            src.y = 64*pointFromTag.x;
            src.w = 128;
            src.h = 64;

            textureLocation = screenToIso(p);

            dest.x = textureLocation.x - 64;
            dest.y = textureLocation.y - 32;
            dest.w = 128;
            dest.h = 64;

            Display::ApplyTexture(texture,src,this->texture, dest);

            // si on est sur le bord EAST
            if ( (key+1) % boardWidth == 0)
            {
                cellGroup = this->computeEastEdgeTag(tile);
                texture = cellGroup.findTexture();

                SDL_Point pointFromTag = cellGroup.getPoint();
                src.x = 128*pointFromTag.y;
                src.y = 64*pointFromTag.x;
                src.w = 128;
                src.h = 64;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x;
                dest.y = textureLocation.y;
                dest.w = 128;
                dest.h = 64;

                Display::ApplyTexture(texture,src,this->texture, dest);
            }

            // si on est sur le bord SOUTH
            if ( key - (boardWidth * (boardHeight -1) ) < boardHeight
             && key - (boardWidth * (boardHeight -1) ) >= 0)
            {
                cellGroup = this->computeSouthEdgeTag(tile);
                texture = cellGroup.findTexture();

                SDL_Point pointFromTag = cellGroup.getPoint();
                src.x = 128*pointFromTag.y;
                src.y = 64*pointFromTag.x;
                src.w = 128;
                src.h = 64;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-128;
                dest.y = textureLocation.y;
                dest.w = 128;
                dest.h = 64;

                Display::ApplyTexture(texture,src,this->texture, dest);
            }

            // si on est sur la dernière case
            if ( key == (boardHeight * boardWidth) -1 )
            {
                cellGroup = this->computeSouthEastEdgeTag(tile);
                texture = cellGroup.findTexture();
                SDL_Point pointFromTag = cellGroup.getPoint();
                src.x = 128*pointFromTag.y;
                src.y = 64*pointFromTag.x;
                src.w = 128;
                src.h = 64;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y+32;
                dest.w = 128;
                dest.h = 64;

                Display::ApplyTexture(texture,src,this->texture, dest);
            }
        }

        // affiche la grille
        for (itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            p = this->indexToPoint(key);

            SDL_Point a = {p.x,p.y};
            SDL_Point b = {p.x,p.y+tileHeight};
            SDL_Point c = {p.x+tileWidth,p.y};
            SDL_Point d = {p.x+tileWidth,p.y+tileHeight};

            a = screenToIso(a);
            b = screenToIso(b);
            c = screenToIso(c);
            d = screenToIso(d);

            Display::DrawLine(this->texture,gridColor,a,b);
            Display::DrawLine(this->texture,gridColor,a,c);
            Display::DrawLine(this->texture,gridColor,c,d);
            Display::DrawLine(this->texture,gridColor,b,d);
        }

        // applique les montagne et les colline
        for (itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            tile = ((*itr).second);

            if ( tile->getElevation() == TileElevation::Montain)
            {
                texture = Display::textureCollection->getTexture("mountain");
                cellGroup = this->computeTerrainTag(tile);
                p = cellGroup.getPointFrom(TileElevation::Montain);

                src.x = 128*p.y;
                src.y = 88*p.x;
                src.w = 128;
                src.h = 88;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y-24;
                dest.w = 128;
                dest.h = 88;
                Display::ApplyTexture(texture,src,this->texture, dest);
            }

            if ( tile->getElevation() == TileElevation::Hill)
            {
                texture = Display::textureCollection->getTexture("hill");
                cellGroup = this->computeTerrainTag(tile);
                p = cellGroup.getPointFrom(TileElevation::Hill);

                src.x = 128*p.y;
                src.y = 64*p.x;
                src.w = 128;
                src.h = 64;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y;
                dest.w = 128;
                dest.h = 64;
                Display::ApplyTexture(texture,src,this->texture, dest);
            }
        }

        // Applique la végétation
        for (itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            tile = ((*itr).second);

            if ( tile->getVegetation() == TileVegetation::Forest)
            {
                texture = Display::textureCollection->getTexture("forest");
                cellGroup = this->computeVegetationTag(tile);
                p = cellGroup.getPointFrom(TileVegetation::Forest);

                src.x = 128*p.y;
                src.y = 88*p.x;
                src.w = 128;
                src.h = 88;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y-24;
                dest.w = 128;
                dest.h = 88;
                Display::ApplyTexture(texture,src,this->texture, dest);
            }
            if ( tile->getVegetation() == TileVegetation::Jungle)
            {
                texture = Display::textureCollection->getTexture("jungle");
                cellGroup = this->computeVegetationTag(tile);
                p = cellGroup.getPointFrom(TileVegetation::Jungle);

                src.x = 128*p.y;
                src.y = 88*p.x;
                src.w = 128;
                src.h = 88;

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y-24;
                dest.w = 128;
                dest.h = 88;
                Display::ApplyTexture(texture,src,this->texture, dest);
            }
        }

        // applique les batiments
        for (itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            p = this->indexToPoint(key);

            if ( ((*itr).second)->hasBuilding() )
            {
                Building* building = ((*itr).second)->getBuilding();
                texture = building->getSpriteSheet();
                src = building->getSrcRect();

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y;
                dest.w = 128;
                dest.h = 64;
                Display::ApplyTexture(texture,src,this->texture, dest);
            }
        }

        // applique les unitée
        for (itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            p = this->indexToPoint(key);

            if ( ((*itr).second)->hasUnit() )
            {
                Unit* unit = ((*itr).second)->getUnit();
                texture = unit->getSpriteSheet();
                src = unit->getSrcRect();

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-26;
                dest.y = textureLocation.y+8;
                dest.w = 64;
                dest.h = 48;
                Display::ApplyTexture(texture,src,this->texture, dest);
            }
        }

        // applique  le marqueur de selection
        if ( tileSelected != nullptr)
        {
            for (itr = board.begin(); itr != board.end(); ++itr)
            {
                key = ((*itr).first);
                p = this->indexToPoint(key);

                // case selectionnée
                if ( key == tileSelected->getIndex())
                {
                    texture = Display::textureCollection->getTexture("grid");
                    src.x = 128*3;
                    src.y = 64*1;
                    src.w = 128;
                    src.h = 64;

                    textureLocation = screenToIso(p);
                    textureLocation.x -= 64;

                    dest.x = textureLocation.x;
                    dest.y = textureLocation.y;
                    dest.w = 128;
                    dest.h = 64;

                    Display::ApplyTexture(texture,src,this->texture, dest);
                }
            }
        }
    }

    /**
     * Gestion de la lumière
     * - Pour chacune des sources de lumières
     * - Si elles intersectent la vue écran
     * - Appliquer la lightmap correspondante (la lightmap doit être dans le bon mode BLEND)
     */
    void Board::lighmapping()
    {
        if ( this->isVisible() == false ) return;

        // soleil
        Display::ApplyTexture(lightmap,{0,0,800,600}, NULL,this->screenRect);
    }

    /**
     * Fourni la case au NORD
     */
    Tile* Board::getNorthNeighbour(int index)
    {
        return this->getTile(index-boardWidth);
    }

    /**
     * Fourni la case NORD OUEST
     */
    Tile* Board::getNWestNeighbour(int index)
    {
        return this->getTile(index-boardWidth-1);
    }

    /**
     * Fourni la case NORD EST
     */
    Tile* Board::getNEastNeighbour(int index)
    {
        return this->getTile(index-boardWidth+1);
    }

    /**
     * Fourni la case au SUD
     */
    Tile* Board::getSouthNeighbour(int index)
    {
        return this->getTile(index+boardWidth);
    }

    /**
     * Fourni la case au SUD OUEST
     */
    Tile* Board::getSWestNeighbour(int index)
    {
        return this->getTile(index+boardWidth - 1);
    }

    /**
     * Fourni la case au SUD EST
     */
    Tile* Board::getSEastNeighbour(int index)
    {
        return this->getTile(index+boardWidth + 1 );
    }

    /**
     * Fourni la case à l'EST
     */
    Tile* Board::getEastNeighbour(int index)
    {
        return this->getTile(index+1);
    }

    /**
     * Fourni la case à l'OUEST
     */
    Tile* Board::getWestNeighbour(int index)
    {
        return this->getTile(index-1);
    }

    /**
     * Calcul le tag pour les terrains de type végétation
     */
    CellGroup Board::computeVegetationTag(Tile *tile)
    {
        CellGroup cellGroup;
        Tile* neighbour;
        int key = tile->getIndex();

        neighbour = this->getWestNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setVegetation4(neighbour->getVegetation());
        }

        neighbour = this->getSouthNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setVegetation3(neighbour->getVegetation());
        }

        neighbour = this->getEastNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setVegetation2(neighbour->getVegetation());
        }

        neighbour = this->getNorthNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setVegetation1(neighbour->getVegetation());
        }

        return cellGroup;
    }

    /**
     * Calcul le tag pour les terrains de type montagne ou coline
     */
    CellGroup Board::computeTerrainTag(Tile* tile)
    {
        CellGroup cellGroup;
        Tile* neighbour;
        int key = tile->getIndex();

        neighbour = this->getWestNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setElevation4(neighbour->getElevation());
        }

        neighbour = this->getSouthNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setElevation3(neighbour->getElevation());
        }

        neighbour = this->getEastNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setElevation2(neighbour->getElevation());
        }

        neighbour = this->getNorthNeighbour(key);
        if ( neighbour != NULL)
        {
            cellGroup.setElevation1(neighbour->getElevation());
        }

        return cellGroup;
    }

    /**
     * Calcul le tag pour le bord est
     */
    CellGroup Board::computeEastEdgeTag(Tile *tile)
    {
        CellGroup cellGroup;
        Tile* neighbour;
        int key;
        key = tile->getIndex();

        neighbour = this->getNorthNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome4(neighbour->getBiome());
        }

        neighbour = this->getNEastNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome3(neighbour->getBiome());
        }

        neighbour = this->getEastNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome2(neighbour->getBiome());
        }

        cellGroup.setBiome1(tile->getBiome());

        return cellGroup;
    }

    /**
     * Calcul le tag pour le bord Sud
     */
    CellGroup Board::computeSouthEdgeTag(Tile *tile)
    {
        CellGroup cellGroup;
        Tile* neighbour;
        int key;
        key = tile->getIndex();

        neighbour = this->getWestNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome4(neighbour->getBiome());
        }

        cellGroup.setBiome3(tile->getBiome());

        neighbour = this->getSouthNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome2(neighbour->getBiome());
        }

        neighbour = this->getSWestNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome1(neighbour->getBiome());
        }

        return cellGroup;
    }

    /**
     * Calcul le tag pour le coin sud-est
     */
    CellGroup Board::computeSouthEastEdgeTag(Tile *tile)
    {
        CellGroup cellGroup;

        cellGroup.setBiome4(tile->getBiome());

        return cellGroup;
    }

    /**
     * Calcul le tag pour les autres terrains
     */
    CellGroup Board::computeTag(Tile *tile)
    {
        CellGroup cellGroup;
        Tile* neighbour;
        int key;
        key = tile->getIndex();

        neighbour = this->getWestNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome4(neighbour->getBiome());
        }

        cellGroup.setBiome3(tile->getBiome());

        neighbour = this->getNorthNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome2(neighbour->getBiome());
        }

        neighbour = this->getNWestNeighbour(key);
        if ( neighbour != NULL )
        {
            cellGroup.setBiome1(neighbour->getBiome());
        }

        return cellGroup;
    }

    /**
     * \brief Gestion de l'événement click gauche
     */
    bool Board::mouseLeftClickEvent(SDL_Point p)
    {
        if ( this->freeze == true ) return false;
        int index;
        SDL_Point screen;

        // prise en compte du zoom
        p.x = (int) p.x * this->getActualScaleFactor();
        p.y = (int) p.y * this->getActualScaleFactor();

        // Ajoute la position de la camera
        screen.x = p.x + displayRect.x;
        screen.y = p.y + displayRect.y;

        // transformation isometrique
        screen = this->isoToScreen(screen);

        if (screen.x > boardWidth * tileWidth
            || screen.y > boardHeight * tileHeight
            || screen.x < 0
            || screen.y < 0 ) {
                evolution::log("User click outside, do nothing.");
                return false;
        }
        index = this->pointToIndex(screen);
        tileSelected = this->getTile(index);
        this->addEvent(BoardEvent::TileSelected); // ajoute l'événement TileSelected dans la queue

        // TODO: Faire une boucle pour présenter les éléments un par un. (Info de la tile, Info du batiment, Info de l'unité)
        // un nouveau click doit présenter l'éléments suivant

        // TODO: est-ce utile de redessiner toute la carte pour placer un marqueur de selection ?
        this->draw();
        return true;
    }

    /**
     * \brief Gestion de l'événement RightDrag
     */
    bool Board::mouseRightDragEvent(SDL_Point p)
    {
        if ( this->freeze == true ) return false;

        // prise en compte du zoom
        p.x = (int) p.x * this->getActualScaleFactor();
        p.y = (int) p.y * this->getActualScaleFactor();

        if (displayRect.x + p.x + displayRect.w < this->getWidth()
            && displayRect.x + p.x >= 0)
        {
            this->setDisplayRect({displayRect.x+p.x, displayRect.y, displayRect.w, displayRect.h});

        }
        if ( displayRect.y + p.y + displayRect.h < this->getHeight()
            && displayRect.y + p.y >= 0)
        {
            this->setDisplayRect({displayRect.x, displayRect.y+p.y, displayRect.w, displayRect.h});
        }
        return true;
    }

    /**
     * \brief Gestion de l'événement KeyDown
     */
    bool Board::keyDownEvent(SDL_Keysym keysym)
    {
        if ( SDL_SCANCODE_RIGHT == SDL_GetScancodeFromKey(keysym.sym) )
        {
            if ( displayRect.x + 16 + displayRect.w < this->getWidth() )
            {
                this->setDisplayRect({displayRect.x+16, displayRect.y, displayRect.w, displayRect.h});
            }
            return true;
        }
        if ( SDL_SCANCODE_LEFT == SDL_GetScancodeFromKey(keysym.sym) )
        {
            if ( displayRect.x - 16 >= 0 )
            {
                this->setDisplayRect({displayRect.x-16, displayRect.y, displayRect.w, displayRect.h});
            }
            return true;
        }
        if ( SDL_SCANCODE_UP == SDL_GetScancodeFromKey(keysym.sym) )
        {
            if ( displayRect.y - 16 >= 0 )
            {
                this->setDisplayRect({displayRect.x, displayRect.y-16, displayRect.w, displayRect.h});
            }
            return true;
        }
        if ( SDL_SCANCODE_DOWN == SDL_GetScancodeFromKey(keysym.sym) )
        {
            if ( displayRect.y + 16 + displayRect.h < this->getHeight() )
            {
                this->setDisplayRect({displayRect.x, displayRect.y+16, displayRect.w, displayRect.h});
            }
            return true;
        }
        if ( SDL_SCANCODE_B == SDL_GetScancodeFromKey(keysym.sym) )
        {
            if ( tileSelected == nullptr )
                std::cout << "User want to build something, but there are no selected tile !" << std::endl;
            else
                std::cout << "User want to build something on tile " << tileSelected << std::endl;
            return true;
        }
        return false;
    }

    bool Board::mouseWheelUp(int amount)
    {
        if ( this->freeze == true ) return false;
        if ( this->setZoomLevel(zoomLevel -1) == true )
        {
            this->addEvent(BoardEvent::ZoomIn);
            return true;
        }
        return false;
    }

    bool Board::mouseWheelDown(int amount)
    {
        if ( this->freeze == true ) return false;
        if ( this->setZoomLevel(zoomLevel +1) == true )
        {
            this->addEvent(BoardEvent::ZoomOut);
            return true;
        }
        return false;
    }


    /**
     * \brief Gestion des annimations sur la carte
     */
    void Board::drawTilesContent(int gameSpeed)
    {
        if ( this->freeze == true ) return;

        // TODO : gestion de la transparence des animations

        int key;
        SDL_Rect src, dest;
        SDL_Texture* texture;
        SDL_Point p, textureLocation;
        for (auto itr = board.begin(); itr != board.end(); ++itr)
        {
            key = ((*itr).first);
            if ( ((*itr).second)->hasBuilding() )
            {
                Building* building = ((*itr).second)->getBuilding();
                building->nextAnimationFrame(gameSpeed);

                texture = building->getSpriteSheet();
                src = building->getSrcRect();

                p = this->indexToPoint(key);
                textureLocation = screenToIso(p);

                dest.x = textureLocation.x-64;
                dest.y = textureLocation.y;
                dest.w = 128;
                dest.h = 64;
                Display::ApplyTexture(texture,src,this->texture, dest);

            }
        }

    }
}
