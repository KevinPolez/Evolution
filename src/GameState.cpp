#include "GameState.h"
#include "Display.h"
#include "evolution.h"
#include "Color.h"
#include "Seed.h"

#include <sstream>

namespace evo
{

    GameState::GameState() : State()
    {
        // une boite de 400x400 au centre de l'écran
        this->dialogBox = new Box(400,400);
        this->dialogBox->setPosition({ (Display::width/2) - 200,(Display::height/2)-200});
        this->dialogBox->setFontSize(16);
        this->dialogBox->setVisible(false);
        this->dialogBox->setAlpha(255);
        this->dialogBox->setName("Boite de dialogue");

        // TODO : parfois les label dans l'nifoBar ne sont pas visibles
        this->infoBar = new HorizontalLayout(Display::width, 50);
        this->infoBar->setName("barre d'information");
        this->infoBar->setPosition({0,0});
        this->infoBar->setCenter(true);
        this->infoBar->setAlpha(225);

        this->timeLabel = new Label();
        this->timeLabel->setFontSize(12);
        this->timeLabel->setText("Time : 0");
        this->timeLabel->setName("timelabel");
        this->infoBar->addObject(timeLabel);

        // ajoute un bouton de retour au menu dans la boite de dialogue
        this->btnMenu = new Button(190,50);
        this->btnMenu->setPosition({105,340});
        this->btnMenu->setName("bouton menu");
        this->btnMenu->setText("Back to menu");

        // attach a callback function to buttonQuit
        std::function<int ()> functionTest =
            [this] ()
            {
                State::nextState = States::STATE_MENU;
                return 0;
            };

        this->btnMenu->setCallback(Event::MouseLeftClick, functionTest);
        this->dialogBox->addObject(btnMenu);

        Button* btnResume = new Button(190,50);
        btnResume->setPosition({105,280});
        btnResume->setName("bouton resume");
        btnResume->setText("Resume game");
        std::function<int ()> functionResume =
            [this] ()
            {
                this->dialogBox->setVisible(false);
                this->board->toggleFreeze();
                this->timelapse->setPause(false);
                return 0;
            };

        btnResume->setCallback(Event::MouseLeftClick, functionResume);
        this->dialogBox->addObject(btnResume);

        this->timelapse = new Timelapse();
        this->timelapse->start();
        std::function<int ()> functionPause =
            [this] ()
            {
                std::ostringstream tempTime;
                tempTime << this->timelapse->getTime();
                this->timeLabel->setText("Time : " + tempTime.str() + " - PAUSED -");
                this->timeLabel->draw();
                return 0;
            };

        this->timelapse->setCallback(TimelapseEvent::Pause, functionPause);

        this->board = new Board(Display::width,Display::height);
        this->board->setPosition({0,0});
        this->board->setName("Plateau de jeu");
        this->board->setBackgroundColor(Color::Green);

        // ajoute un label donnant le seed dans la boite de dialogue
        Label* seedLabel = new Label();
        seedLabel->setFontSize(12);
        seedLabel->setPosition({6,6});
        seedLabel->setText("Seed : " + std::to_string(Seed::seed));
        seedLabel->setName("seedLabel");
        this->dialogBox->addObject(seedLabel);

        VerticalLayout* toolBar = new VerticalLayout(50,100);
        toolBar->setName("Barre d'outil");
        toolBar->setPosition({0,Display::height-100});

        Button* scienceBtn = new Button(50,50);
        scienceBtn->setText("S");
        toolBar->addObject(scienceBtn);

        Button* politiqueBtn = new Button(50,50);
        politiqueBtn->setText("P");
        toolBar->addObject(politiqueBtn);

        // prépare une boite d'info
        this->infoBox = new Box(100,60);
        this->infoBox->setName("infoBox");
        this->infoBox->setFontSize(12);
        this->infoBox->setVisible(false);
        this->infoBox->setZIndex(1);
        this->infoBox->setAlpha(225);
        this->infoBox->setPosition({Display::width-110, Display::height-70});
        this->addObject(infoBox);

        this->zoomLabel = new Label();
        this->zoomLabel->setName("zoomLabel");
        this->addObject(zoomLabel);
        this->zoomLabel->setPosition({Display::width-150,70});
        this->zoomLabel->setZIndex(2);
        this->zoomLabel->setFontColor(Color::Red);
        this->zoomLabel->setFontSize(32);
        this->zoomLabel->setVisible(false);

        this->addObject(this->board);
        this->addObject(this->dialogBox);
        this->addObject(this->infoBar);
        this->addObject(toolBar);

        this->board->setZIndex(0);
        this->infoBar->setZIndex(1);
        this->infoBox->setZIndex(1);
        toolBar->setZIndex(1);
        this->dialogBox->setZIndex(2);

        Unit* unitTest = new Unit(UnitType::Test);
        Tile* tile = this->board->getTile(2081);
        tile->setUnit(unitTest);
        this->unitList.push_back(unitTest);

        Building* buildingTest = new Building(BuildingType::Portail);
        tile = this->board->getTile(2080);
        tile->setBuilding(buildingTest);
        this->buildingList.push_back(buildingTest);

    }

    GameState::~GameState()
    {
        delete timelapse;

        for (auto it = unitList.begin(); it != unitList.end(); ++it) {
            delete (*it);
        }

        for (auto it = buildingList.begin(); it != buildingList.end(); ++it) {
            delete (*it);
        }
    }

    /**
     * Gestion de la logique de jeu
     */
    void GameState::gameLogic()
    {
        // Gestion des événements provenant du plateau
        Tile* tile;
        std::stringstream strstream;

        BoardEvent boardEvent;
        while ( this->board->getEvent(&boardEvent))
        {
            switch (boardEvent)
            {
                case BoardEvent::TileSelected :
                    // TODO : parfois les label dans l'infoBox ne sont pas visible
                    tile = this->board->getSelectedTile();
                    strstream << "Index : " << tile->getIndex() << std::endl;
                    strstream << "Height : " << tile->getHeight() << std::endl;
                    strstream << "Temperature : " << tile->getTemperature() << std::endl;
                    strstream << "Hygrometrie : " << tile->getHygrometrie() << std::endl;
                    infoBox->setText(strstream.str());
                    infoBox->draw();
                    infoBox->setVisible(true);
                    break;
                case BoardEvent::ZoomIn:
                    // effet sur la musique

                    break;
                case BoardEvent::ZoomOut:
                    // effet sur la musique
                    break;
                case BoardEvent::Zoom1:
                    this->zoomLabel->setText("zoom 1");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case BoardEvent::Zoom2:
                    this->zoomLabel->setText("zoom 2");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case BoardEvent::Zoom3:
                    this->zoomLabel->setText("zoom 3");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case BoardEvent::Zoom4:
                    this->zoomLabel->setText("zoom 4");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case BoardEvent::Zoom5:
                    this->zoomLabel->setText("zoom 5");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
            }
        }

        TimelapseEvent timelapseEvent;
        while ( this->timelapse->getEvent(&timelapseEvent))
        {
            switch (timelapseEvent)
            {
                case TimelapseEvent::Speed1:
                    this->zoomLabel->setText("speed 1");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case TimelapseEvent::Speed2:
                    this->zoomLabel->setText("speed 2");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case TimelapseEvent::Speed3:
                    this->zoomLabel->setText("speed 3");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.15f);
                    this->zoomLabel->draw();
                    break;
                case TimelapseEvent::SpeedUp:
                    break;
                case TimelapseEvent::SpeedDown:
                    break;
                case TimelapseEvent::Pause:
                    this->zoomLabel->setText("Pause");
                    this->zoomLabel->setVisible(true);
                    this->zoomLabel->setFadeOut(0.5f);
                    this->zoomLabel->draw();
                    break;
            }
        }

        if ( this->timelapse->nextTurn() == true )
        {
            /**std::ostringstream tempTime;
            tempTime << this->timelapse->getTime();
            this->timeLabel->setText("Time : " + tempTime.str());
            this->timeLabel->draw();*/
        }
    }

    /**
     * Gestion des animations
     */
    void GameState::animation()
    {
        this->board->drawTilesContent(this->timelapse->getSpeed());
    }

    /**
     * Gestion des événements
     */
    void GameState::personalEvent(SDL_Event event)
    {
        switch (event.type) {
            case SDL_KEYDOWN:
                // Display or hide menu
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    this->dialogBox->setVisible(! this->dialogBox->isVisible());
                    if ( this->dialogBox->isVisible() )
                    {
                        this->timelapse->setPause(true);
                        this->board->toggleFreeze();
                    }
                    else
                    {
                        this->timelapse->setPause(false);
                        this->board->toggleFreeze();
                    }
                }

                // Speed-up the game
                if (event.key.keysym.sym == SDLK_KP_PLUS) {
                    this->timelapse->speedUp();
                }

                // Speed-down the game
                if (event.key.keysym.sym == SDLK_KP_MINUS) {
                    this->timelapse->speedDown();
                }

                // deplace l'unité selectionnée
                if (event.key.keysym.sym == SDLK_KP_1)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getSouthNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_2)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getSEastNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_3)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getEastNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_4)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getSWestNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_6)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getNEastNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_7)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getWestNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_8)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getNWestNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_KP_9)
                {
                    Tile* tile = this->board->getSelectedTile();
                    if ( tile != nullptr)
                    {
                        if ( tile->hasUnit())
                        {
                            Unit* unit = tile->getUnit();
                            Tile* newTile = this->board->getNorthNeighbour(tile->getIndex());
                            tile->setUnit(nullptr);
                            newTile->setUnit(unit);
                            this->board->setSelectedTile(newTile);
                            this->board->draw();
                        }
                    }
                }
                if (event.key.keysym.sym == SDLK_1)
                {
                    this->board->setZoomLevel(1);
                }
                if (event.key.keysym.sym == SDLK_2)
                {
                    this->board->setZoomLevel(2);
                }
                if (event.key.keysym.sym == SDLK_3)
                {
                    this->board->setZoomLevel(3);
                }
                if (event.key.keysym.sym == SDLK_4)
                {
                    this->board->setZoomLevel(4);
                }
                if (event.key.keysym.sym == SDLK_5)
                {
                    this->board->setZoomLevel(5);
                }

                // pause the game
                // TODO : lorsque le jeu est en pause, les animations doivent cesser
                if (event.key.keysym.sym == SDLK_p) {
                    if ( ! this->board->isFreeze() )
                    {
                        this->timelapse->togglePause();
                    }
                }

                break;
        }
    }
}
