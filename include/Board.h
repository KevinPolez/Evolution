#ifndef BOARD_H
#define BOARD_H

#include <SDL2/SDL.h>

#include "Object.h"
#include "Tile.h"
#include "TextureCollection.h"
#include "CellGroup.h"
#include "DialogBox.h"

#include <map>
#include <list>

namespace evo
{

    enum class BoardEvent { TileSelected, ZoomIn, ZoomOut, Zoom1, Zoom2, Zoom3, Zoom4, Zoom5 };

    class Board : public Object
    {
        public:
            Board(int width, int height);
            virtual ~Board();

            void toggleFreeze();
            bool setZoomLevel(int zoomLevel);
            bool isFreeze();

            void drawTilesContent(int gameSpeed);
            Tile* getTile(int index) const;
            Tile* getSelectedTile() const;
            void setSelectedTile(Tile* tile);

            Tile* getEastNeighbour(int index);
            Tile* getNEastNeighbour(int index);
            Tile* getSEastNeighbour(int index);
            Tile* getWestNeighbour(int index);
            Tile* getNWestNeighbour(int index);
            Tile* getSWestNeighbour(int index);
            Tile* getNorthNeighbour(int index);
            Tile* getSouthNeighbour(int index);

            bool getEvent(BoardEvent* event);
            void addEvent(BoardEvent event);

            // override method
            void draw();
            void lighmapping();

            // override event method
            bool mouseLeftClickEvent(SDL_Point p);
            bool mouseRightDragEvent(SDL_Point p);
            bool mouseWheelUp(int amount);
            bool mouseWheelDown(int amount);
            bool keyDownEvent(SDL_Keysym keysym);

        protected:

            float getScaleFactor(int zoomLevel);

            void computeBiome(Tile* tile);
            void computeElevation(Tile* tile);
            void computeVegetation(Tile* tile);

            CellGroup computeTerrainTag(Tile* tile);
            CellGroup computeVegetationTag(Tile* tile);
            CellGroup computeTag(Tile* tile);
            CellGroup computeEastEdgeTag(Tile* tile);
            CellGroup computeSouthEdgeTag(Tile* tile);
            CellGroup computeSouthEastEdgeTag(Tile* tile);

            int indexCalculation(int i, int j);
            int pointToIndex(SDL_Point p);
            SDL_Point indexToPoint(int index);
            SDL_Point screenToIso(SDL_Point p);
            SDL_Point screenToIso(int x, int y);
            SDL_Point isoToScreen(SDL_Point p);
            SDL_Point addView(SDL_Point p);
            SDL_Point removeView(SDL_Point p);
            float radToDeg(float rad);
            float degToRad(float deg);

        private:
            int boardWidth;
            int boardHeight;
            int tileWidth;
            int tileHeight;
            std::map<int,Tile*> board;
            std::list<BoardEvent> boardEvent;

            SDL_Color gridColor;
            SDL_Color backgroundColor;
            Tile* tileSelected;
            SDL_Rect display;

            bool freeze;
            int zoomLevel;

            SDL_Texture* lightmap;
    };
}

#endif // BOARD_H
