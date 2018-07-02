
#include "Object.h"
#include "Display.h"
#include "evolution.h"

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <cmath>

namespace evo
{

    /**
     * Constructeur
     */
    Object::Object() :
                    zIndex(0),
                    texture(NULL),
                    displayRect({0,0,0,0}),
                    screenRect({0,0,0,0}),
                    backgroundColor({0,0,0}),
                    borderColor({0,0,0}),
                    alpha(255),
                    visible(true),
                    mouseIn(false),
                    mouseRightClick(false),
                    mouseLeftClick(false),
                    mouseRightDrag(false),
                    mouseLeftDrag(false),
                    focus(false),
                    focusable(false),
                    fadeSpeed(0.0f),
                    zoomTime(1000),
                    zoomTimeStart(0),
                    reachRect({0,0,0,0}),
                    oldScaleFactor(1.0f),
                    actualScaleFactor(1.0f),
                    reachScaleFactor(1.0f),
                    zoomSpeed(0.1f),
                    fadeOutEffect(false),
                    fadeInEffect(false),
                    zoomInEffect(false),
                    zoomOutEffect(false),
                    startDragPoint({0,0}),
                    stopDragPoint({0,0}),
                    parent(NULL)
    {
        this->setUUID(); // détermine un ID unique pour l'objet
        this->setName(this->UUID); // par défaut, le nom de l'objet correspond à son UUID
        evolution::log("Create object : " + this->UUID);
    }

    /**
     * Destructeur
     * - détruit les objets fils
     * - détruit la texture
     */
    Object::~Object() {
        std::list<Object*>::iterator it;
        for( it = objectList.begin(); it != objectList.end(); ++it) {
            delete (*it);
        }

        this->destroyTexture();

        // detruire les fonctions de callback !

        evolution::log("Destroy object : " + this->UUID);
    }

    void Object::setUUID()
    {
        std::ostringstream temp;
        temp << reinterpret_cast<int>(this);
        this->UUID = temp.str();
    }

    /**
     * Destruction de la texture de l'objet
     */
    void Object::destroyTexture()
    {
        if ( this->hasTexture() )
        {
            SDL_DestroyTexture(this->texture);
            this->texture = NULL;
        }
    }

    /**
     * Création de la texture de l'objet
     * Si celle-ci existe déjà, elle est détruite et recréée.
     */
    void Object::createTexture()
    {
        this->destroyTexture();
        this->texture = Display::CreateTexture(this->width, this->height);
        if ( this->alpha != 255 ) Display::SetTextureBlendMode(this->texture,SDL_BLENDMODE_BLEND);
    }

    /**
     * Défini la texture de l'objet
     */
    void Object::setTexture(SDL_Texture *texture)
    {
        this->destroyTexture();
        this->texture = texture;
    }

    /**
     * Récupére la texture de l'objet
     */
    SDL_Texture *Object::getTexture()
    {
        return this->texture;
    }

    /**
     * Indique si l'objet dispose d'une texture
     */
    bool Object::hasTexture()
    {
        if ( this->texture != NULL) return true;
        return false;
    }

    /**
     * Donne la largeur réelle de la texture
     */
    int Object::getWidth() {
        return this->width;
    }

    /**
     * Donne la hauteur réelle de la texture
     */
    int Object::getHeight() {
        return this->height;
    }

    /**
     * Détermine la largeur réelle de la texture
     */
    void Object::setWidth(int width)
    {
        this->width = width;
    }

    /**
     * Détermine la hauteur réelle de la texture
     */
    void Object::setHeight(int height)
    {
        this->height = height;
    }

    /**
     * Détermine l'emplacement de l'objet relatif à son parent
     */
    void Object::setPosition(SDL_Point point)
    {
        this->screenRect.x = point.x;
        this->screenRect.y = point.y;

        if ( this->parent )
        {
            SDL_Rect parentScreenRect = this->parent->getScreenRect();
            SDL_Rect parentDisplayRect = this->parent->getDisplayRect();
            this->screenRect.x += parentScreenRect.x;
            this->screenRect.y += parentScreenRect.y;
            this->screenRect.x -= parentDisplayRect.x;
            this->screenRect.y -= parentDisplayRect.y;
        }
    }

    /**
     * Fourni l'emplacement de l'objet relatif à son parent
     */
    SDL_Point Object::getPosition()
    {
        return {this->screenRect.x, this->screenRect.y};
    }

    /**
     * Défini le screenRect
     */
    void Object::setScreenRect(SDL_Rect rect) {
        this->screenRect = rect;
    }

    /**
     * Fourni l'emplacement réél de l'objet sur la fenêtre principale
     */
    SDL_Rect Object::getScreenRect()
    {
        return this->screenRect;
    }

    /**
     * Fourni l'index de la frame courante
     */
    /*int Object::getCurrentFrame()
    {
        return this->currentFrame;
    }*/

    /**
     * Prépare la texture de l'objet (et de tous ses enfants)
     */
    void Object::finish()
    {
        std::list<Object*>::iterator it;
        for( it = objectList.begin(); it != objectList.end(); ++it) {
            (*it)->draw();
        }

        this->draw();
    }

    /**
     * Fourni le rectangle d'affichage (la vue camera)
     */
    SDL_Rect Object::getDisplayRect() {
        return this->displayRect;
    }

    /**
     * Defini le rectangle d'affichage (la vue camera)
     */
    void Object::setDisplayRect(SDL_Rect rect) {
        int newX = rect.x - displayRect.x;
        int newY = rect.y - displayRect.y;
        this->displayRect = rect;

        // met à jour le screenRect des enfants
        for( auto it = objectList.begin(); it != objectList.end(); ++it) {
            SDL_Rect newRect = (*it)->getScreenRect();
            newRect.x -= newX;
            newRect.y -= newY;
            (*it)->setScreenRect(newRect);
        }
    }

    void Object::updateScreenRect(SDL_Point p)
    {
        screenRect.x -= p.x;
        screenRect.y -= p.y;
        for( auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            (*it)->updateScreenRect(p);
        }
    }

    /**
     * Fourni le focus de l'objet
     */
    bool Object::getFocus()
    {
        return this->focus;
    }

    /**
     * Défini le focus de l'objet
     */
    void Object::setFocus(bool focus)
    {
        this->focus = focus;
    }

    /**
     * Vérifie si l'objet a un parent
     */
    bool Object::hasParent()
    {
        if ( this->parent != NULL) return true;
        return false;
    }

    /**
     * Ajoute un objet
     */
    void Object::addObject(Object* obj) {
        obj->setParent(this);
        obj->screenRect.x += this->screenRect.x;
        obj->screenRect.y += this->screenRect.y;

        this->objectList.push_back(obj);
    }

    /**
     * Défini le parent d'un objet
     */
    void Object::setParent(Object *obj)
    {
        this->parent = obj;
    }

    void Object::setReachRect(SDL_Rect reachRect)
    {
        this->reachRect = reachRect;
    }

    float Object::getActualScaleFactor() const
    {
        return this->actualScaleFactor;
    }

    float Object::getReachScaleFactor() const
    {
        return this->reachScaleFactor;
    }

    /**
     * Initialise les variable de début du zoom
     */
    void Object::startZoom(float scaleFactor)
    {
        this->zoomTimeStart = Fps::getCurrentTime();
        this->reachScaleFactor = scaleFactor;

        // calcul le nouveau displayRect à atteindre;
        this->reachRect = this->displayRect;
        this->reachRect.w = static_cast<int> (((float) displayRect.w / this->actualScaleFactor) * this->reachScaleFactor);
        this->reachRect.h = static_cast<int> (((float) displayRect.h / this->actualScaleFactor) * this->reachScaleFactor);
    }

    /**
     * Ré-applique la texture d'un objet
     */
    void Object::refresh()
    {
        if ( this->isVisible() == false ) return;

        // Gestion de l'effet fadeOut
        if ( this->fadeOutEffect == true )
        {
            int newAlpha = this->getAlpha() - 5;
            if ( newAlpha <= SDL_ALPHA_TRANSPARENT)
            {
                this->fadeOutEffect = false;
                newAlpha = SDL_ALPHA_TRANSPARENT;
            }
            this->setAlpha(newAlpha);
        }

        // gestion de l'effet fadeIn
        if ( this->fadeInEffect == true)
        {
            int newAlpha = this->getAlpha() + 5;
            if ( newAlpha >= SDL_ALPHA_OPAQUE)
            {
                this->fadeInEffect = false;
                newAlpha = SDL_ALPHA_OPAQUE;
            }
            this->setAlpha(newAlpha);
        }

        if ( this->zoomInEffect == true || this->zoomOutEffect == true)
        {

            Uint32 delay = Fps::getCurrentTime() - this->zoomTimeStart; // Calcul le temps restant avant la fin de l'animation
            UInt32 elapsed = Fps::frameTimeElapsed(); // temps passé depuis la dernière frame

            float scale = 0.1f * elapsed; // zoomSpeed * frame time elapsed

            int step = static_cast<int>(evolution::roundf((actualScaleFactor - reachScaleFactor) / scale));
            if ( step !=  0)
            {
                int addw = round((float)((reachRect.w - displayRect.w) / step) / 10) * 10;
                int addh = round((float)((reachRect.h - displayRect.h) / step) / 10) * 10;

                //addw = round((float)addw / 10)*10;
                //addh = round((float)addh / 10)*10;
                if ( zoomOutEffect )
                {
                    displayRect.w -= addw;
                    displayRect.h -= addh;
                    displayRect.x += (addw / 2);
                    displayRect.y += (addh / 2);
                    actualScaleFactor = evolution::roundf(actualScaleFactor + scale);
                }
                if ( zoomInEffect )
                {
                    displayRect.w += addw;
                    displayRect.h += addh;
                    displayRect.x -= (addw / 2);
                    displayRect.y -= (addh / 2);
                    actualScaleFactor = evolution::roundf(actualScaleFactor - scale);
                }
            }
            else
            {
                this->zoomInEffect = false;
                this->zoomOutEffect = false;
            }


            // fin de l'animation
            if ( delay >= 1000 )
            {
                this->zoomInEffect = false;
                this->zoomOutEffect = false;
            }

            if ( evolution::cmpf(actualScaleFactor, reachScaleFactor,0.005f) )
            {
                this->zoomInEffect = false;
                this->zoomOutEffect = false;
            }
        }

        Display::ApplyTexture(this->texture, this->displayRect, NULL, this->screenRect);

        for( auto it = objectList.begin(); it != objectList.end(); ++it) {
            (*it)->refresh();
        }
    }

    /**
     * Gestion de la lumière
     */
    void Object::lighmapping()
    {
        for( auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            (*it)->lighmapping();
        }
        return;
    }

    void Object::setReachScaleFactor(float scaleFactor)
    {
        this->reachScaleFactor = scaleFactor;
    }

    void Object::setOldScaleFactor(float scaleFactor)
    {
        this->oldScaleFactor = scaleFactor;
    }

    /**
     * Affiche la prochaine frame de l'annimation
     */
    /*void Object::nextAnimationFrame()
    {
        for( auto it = objectList.begin(); it != objectList.end(); ++it)
        {
            (*it)->nextAnimationFrame();
        }
        return;
    }*/

    /**
     * Gestion des événements
     */
    Object* Object::event(SDL_Event event) {
        if ( ! this->isVisible() ) return nullptr;

        Object* returnObj;
        for( auto it = objectList.begin(); it != objectList.end(); ++it) {
            returnObj = (*it)->event(event);
            if (  returnObj != nullptr )
            {
                return returnObj; // l'événement a été traité, arrêter sa propagation
            }
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        switch (event.type) {
            case SDL_MOUSEMOTION :
                if ( this->mouseRightDrag || this->mouseLeftDrag )
                {
                    SDL_Point p = {startDragPoint.x - event.motion.x, startDragPoint.y - event.motion.y};
                    startDragPoint = {event.motion.x, event.motion.y};
                    if ( this->mouseRightDrag )
                    {
                        if ( this->mouseRightDragEvent(p) == true ) return this;
                    }
                    if ( this->mouseLeftDrag )
                    {
                        if ( this->mouseLeftDragEvent(p) == true ) return this;
                    }

                    startDragPoint = {event.motion.x, event.motion.y};
                }
                if ( this->isOnScreenRect({event.motion.x, event.motion.y})) {

                    if ( mouseIn == false) {
                        mouseIn = true;
                        if ( this->mouseEnterEvent() == true ) return this;
                    }


                    if ( this->mouseMotionEvent() == true ) return this;

                    return this;
                }
                else {
                    if ( mouseIn == true) {
                        mouseIn = false;
                        if ( this->mouseLeaveEvent() == true) return this;
                    }
                    if ( mouseRightDrag == true ) {
                        mouseRightDrag = false;
                    }
                    if ( mouseLeftDrag == true ) {
                        mouseLeftDrag = false;
                    }
                }

                break;
            case SDL_MOUSEBUTTONDOWN :
                if ( this->isOnScreenRect({mouse_x, mouse_y}))
                {
                    if ( SDL_BUTTON_RIGHT == event.button.button) {
                        mouseRightClick = true;
                        mouseRightDrag = true;
                        startDragPoint = {event.motion.x, event.motion.y};
                    } else if (SDL_BUTTON_LEFT == event.button.button) {
                        mouseLeftClick = true;
                        mouseLeftDrag = true;
                        startDragPoint = {event.motion.x, event.motion.y};
                    }
                    return this;
                }
                break;
            case SDL_MOUSEBUTTONUP :
                if ( this->isOnScreenRect({mouse_x, mouse_y}))
                {
                    if ( SDL_BUTTON_RIGHT == event.button.button) {
                        if ( mouseRightClick == true ) {
                            mouseRightClick = false;
                            if ( this->mouseRightDrag )
                            {
                                mouseRightDrag = false;
                                stopDragPoint = {screenRect.x, screenRect.y};
                                if ( this->mouseDropEvent(stopDragPoint) == true ) return this;
                            }
                            if ( this->mouseRightClickEvent({mouse_x, mouse_y}) == true ) return this;
                        }

                    } else if (SDL_BUTTON_LEFT == event.button.button) {
                        if ( mouseLeftClick == true ) {
                            mouseLeftClick = false;
                            if ( this->mouseLeftDrag )
                            {
                                mouseLeftDrag = false;
                                stopDragPoint = {screenRect.x, screenRect.y};
                                if ( this->mouseDropEvent(stopDragPoint) == true ) return this;
                            }
                            if ( this->mouseLeftClickEvent({mouse_x, mouse_y}) == true ) return this;
                        }
                    }
                }
                break;
            case SDL_MOUSEWHEEL:
                if ( this->isOnScreenRect({mouse_x, mouse_y}))
                {
                    if ( event.wheel.y > 0 )
                    {
                        if ( this->mouseWheelUp(event.wheel.y) == true ) return this;
                    }
                    else if ( event.wheel.y < 0 )
                    {
                        if ( this->mouseWheelDown(event.wheel.y) == true ) return this;
                    }
                }
                break;
            case SDL_TEXTINPUT :
                if ( this->textInputEvent(event.text.text) == true ) return this;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if ( this->textBackspaceEvent() == true ) return this;
                }
                if ( this->keyDownEvent(event.key.keysym) == true ) return this;
                break;
        }
        return nullptr;
    }

    /**
     * \brief Défini une fonction de callback associé à un event
     */
    void Object::setCallback(Event event, std::function<bool()> callback) {
        callbackMap.insert( std::pair<Event,std::function<bool()> >(event,callback));
    }

    /**
     * \brief Défini un effet fade out
     */
    void Object::setFadeOut(float fadeSpeed)
    {
        this->fadeSpeed = fadeSpeed;
        this->fadeOutEffect = true;
        this->setAlpha(SDL_ALPHA_OPAQUE);
    }

    /**
     * \brief Défini un effet fade in
     */
    void Object::setFadeIn(float fadeSpeed)
    {
        this->fadeSpeed = fadeSpeed;
        this->fadeInEffect = true;
        this->setAlpha(SDL_ALPHA_TRANSPARENT);
    }

    /**
     * \brief Défini un effet zoom in
     */
    void Object::setZoomIn()
    {
        this->zoomInEffect = true;
        this->zoomOutEffect = false;
    }

    /**
     * \brief Défini un effet zoom out
     */
    void Object::setZoomOut()
    {
        this->zoomOutEffect = true;
        this->zoomInEffect = false;
    }

    /**
     * \brief Indique si l'objet peut prendre le focus
     */
    bool Object::isFocusable()
    {
        return this->focusable;
    }

    /**
     * \brief Indique que l'objet peut prendre le focus
     */
    void Object::setFocusable(bool focusable)
    {
        this->focusable = focusable;
    }

    /**
     * \brief Recherche si cette objet possède le Focus
     */
    Object* Object::findFocus(Object* oldObjectFocused)
    {
        Object* tmpObj;
        for( auto it = objectList.begin(); it != objectList.end(); ++it) {
            tmpObj = (*it)->findFocus(oldObjectFocused);
            if ( tmpObj != nullptr) return tmpObj;
        }

        if (
            this->isFocusable() == true
            && this->getFocus() == false
            && this != oldObjectFocused )
                return this;

        return nullptr;

    }

    /**
     * \brief Gestion de l'événement perte de focus
     */
    bool Object::looseFocusEvent()
    {
        return false;
    }

    /**
     * \brief Gestion de l'événement gain de focus
     */
    bool Object::gainFocusEvent()
    {
        return false;
    }

    /**
     * \brief Gestion de l'événement mouseMotion
     */
    bool Object::mouseMotionEvent() {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseMotion);
        if ( it != callbackMap.end()) {
            return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement mouseEnter
     */
    bool Object::mouseEnterEvent() {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseEnter);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement mouseLeave
     */
    bool Object::mouseLeaveEvent() {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseLeave);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement click-droit
     */
    bool Object::mouseRightClickEvent(SDL_Point p) {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseRightClick);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement click-gauche
     */
    bool Object::mouseLeftClickEvent(SDL_Point p) {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseLeftClick);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement RightDrag
     */
    bool Object::mouseRightDragEvent(SDL_Point p)
    {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseRightDrag);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

        /**
     * \brief Gestion de l'événement LeftDrag
     */
    bool Object::mouseLeftDragEvent(SDL_Point p)
    {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseLeftDrag);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement Drop
     */
    bool Object::mouseDropEvent(SDL_Point p)
    {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseDrop);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement WheelUp
     */
    bool Object::mouseWheelUp(int amount)
    {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseWheelUp);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement WheelDown
     */
    bool Object::mouseWheelDown(int amount)
    {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::MouseWheelDown);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * \brief Gestion de l'événement KeyDown
     */
    bool Object::keyDownEvent(SDL_Keysym keysym)
    {
        std::map<Event,std::function<bool()> >::iterator it;
        it = callbackMap.find(Event::KeyDown);
        if ( it != callbackMap.end()) {
           return (*it).second();
        }
        return false;
    }

    /**
     * Gestion de l'événement TextInput
     */
    bool Object::textInputEvent(char* c)
    {
        return false;
    }

    bool Object::textBackspaceEvent()
    {
        return false;
    }

    /**
     * Vérifie qu'un point est dans le screenRect
     */
    bool Object::isOnScreenRect(SDL_Point p)
    {
        return SDL_PointInRect(&p, &this->screenRect);
    }

    /**
     * Détermine si un point est dans le displayRect
     */
    bool Object::isOnDisplayRect(SDL_Point p)
    {
        return SDL_PointInRect(&p, &this->displayRect);
    }

    /**
     * Détermine la visibilité de l'objet
     */
    void Object::setVisible(bool visible) {
        this->visible = visible;
    }

    /**
     * Indique si l'objet est visible
     */
    bool Object::isVisible() {
        return this->visible;
    }

    /**
     * Détermine la couleur de fond de l'objet
     */
    void Object::setBackgroundColor(SDL_Color color) {
        this->backgroundColor = color;
    }

    /**
     * Fourni la couleur de fond de l'objet
     */
    SDL_Color Object::getBackgroundColor() {
        return this->backgroundColor;
    }

    /**
     * Détermine la couleur de bordure de l'objet
     */
    void Object::setBorderColor(SDL_Color color) {
        this->borderColor = color;
    }

    /**
     * Fourni la couleur de bordure de l'objet
     */
    SDL_Color Object::getBorderColor() {
        return this->borderColor;
    }

    /**
     * Applique la couleur de fond en prennant en compte la valeur de alpha
     */
    void Object::applyBackgroundColor()
    {
        SDL_Color color = this->backgroundColor;
        color.a = this->alpha;
        Display::ApplyColor(this->texture, color);
    }

    /**
     * Applique les bordures
     */
    void Object::applyBorderColor()
    {
        Display::DrawLine(this->texture, this->borderColor, {0,0},{0,getHeight()-1});
        Display::DrawLine(this->texture, this->borderColor, {0,0},{getWidth()-1,0});
        Display::DrawLine(this->texture, this->borderColor, {getWidth()-1,0},{getWidth()-1,getHeight()-1});
        Display::DrawLine(this->texture, this->borderColor, {0,getHeight()-1},{getWidth()-1,getHeight()-1});
    }

    /**
     * Détermine la valeur Alpha de la couleur de fond
     */
    void Object::setAlpha(int alpha)
    {
        this->alpha = alpha;
        if ( this->texture != NULL )
        {
            if ( SDL_SetTextureAlphaMod(this->texture, alpha) == -1 )
            {
                evolution::log("Alpha modulation not supported");
            }
        }
    }

    /**
     * Récupére la valeur Alpha de la couleur de fond
     */
    int Object::getAlpha()
    {
        return this->alpha;
    }

    /**
     * Fourni la valeur de zIndex
     */
    int Object::getZIndex()
    {
        return this->zIndex;
    }

    /**
     * Défini la valeur de zIndex
     */
    void Object::setZIndex(int zIndex)
    {
        this->zIndex = zIndex;
    }

    /**
     * Défini le nom de l'objet
     */
    void Object::setName(std::string name)
    {
        this->name = name;
    }

    /**
     * Fourni le nom de l'objet
     */
    std::string Object::getName()
    {
        return this->name;
    }
}
