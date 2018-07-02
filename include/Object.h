#ifndef OBJECT_H
#define OBJECT_H

#include <SDL2/SDL.h>
#include <list>
#include <map>
#include <functional>

namespace evo
{

    enum class Event { MouseLeftClick, MouseRightClick, MouseEnter, MouseLeave, MouseMotion, MouseRightDrag, MouseLeftDrag, MouseDrop, MouseWheelUp, MouseWheelDown, KeyDown };
    enum class ObjectType { Animation, Board, Button, Label, InputText, VerticalLayout, HorizontalLayout, ProgressBar, Box, ScrollText};

    /**
     * Un objet est composé d'une texture, et de plusieurs rect :
     * - le screenRect : correspond à l'emplacement qu'il occupe sur l'écran
     * - le displayRect : correspond à la portion de texture copier sur l'écran
     */
    class Object
    {
        public:

            /**
             * Constructeur de base
             */
            Object();

            /**
             * Destructeur
             * - Tous les objets dépendant de cet objet seront détruit
             * - La texture (si elle existe) est détruite
             */
            virtual ~Object();

            /**
             * dessine la texture de l'objet (doit être surchargée)
             */
            virtual void draw() = 0;

            /**
             * Applique l'objet à l'écran
             */
            virtual void refresh();

            /**
             * Gestion des lumières
             */
            virtual void lighmapping();

            /**
             * Gestion des événements
             */
            virtual Object* event(SDL_Event event);

            /**
             * Prépare la texture de l'objet
             */
            void finish();

            /**
             * Récupére la hauteur de la texture
             */
            int getHeight();

            /**
             * Récupére la largeur de la texture
             */
            int getWidth();

            /**
             * Détermine la hauteur de la texture
             */
            void setHeight(int height);

            /**
             * Détermine la largeur de la texture
             */
            void setWidth(int width);

            /**
             * Détermine la position de l'objet (sur son objet parent)
             */
            void setPosition(SDL_Point point);

            /**
             * Fourni la position de l'objet (relatif à son objet parent)
             */
            SDL_Point getPosition();

            /**
             * Défini le véritable emplacement de l'objet sur l'écran
             */
            void setScreenRect(SDL_Rect parentScreenRect);

            /**
             * Fourni le screenRect de l'objet
             */
            SDL_Rect getScreenRect();

            /**
             * Modifie le screenRect
             */
            void updateScreenRect(SDL_Point p);

            /**
             * Récupére le rectangle à afficher (peut être plus petit que la taille de la texture)
             */
            SDL_Rect getDisplayRect();

            /**
             * Définie le rectangle à afficher (peut être plus petit que la taille de la texture)
             */
            void setDisplayRect(SDL_Rect rect);

            /**
             * Définie la couleur de base de la texture
             * - déclenche un clearTexture
             */
            void setBackgroundColor(SDL_Color color);

            /**
             * Récupére la couleur de base de la texture
             */
            SDL_Color getBackgroundColor();

            /**
             * Défini la couleur de la bordure
             */
            void setBorderColor(SDL_Color color);

            /**
             * Fourni la couleur de la bordure
             */
            SDL_Color getBorderColor();

            /**
             * Défini la valeur Alpha de la couleur de fond
             */
            void setAlpha(int alpha);

            /**
             * Récupére la valeur Alpha de la couleur de fond
             */
            int getAlpha();

            /**
             * Indique si l'objet est visible
             * - conditionne l'affichage de l'objet
             */
            void setVisible(bool visible);

            /**
             * Indique si l'objet est visible
             */
            bool isVisible();

            /**
             * Défini la texture de l'objet
             */
            void setTexture(SDL_Texture *texture);

            /**
             *  Récupére la texture de l'objet
             */
            SDL_Texture *getTexture();

            /**
             * Indique si l'objet dispose d'une texture
             */
            bool hasTexture();

            /**
             * Ajoute un objet dans l'objet
             */
            void addObject(Object* obj);

            /**
             * Défini le parent d'un objet
             */
            void setParent(Object *obj);

            /**
             * Defini le Z index de l'objet
             */
            void setZIndex(int zIndex);

            /**
             * Récupére le Z index
             */
            int getZIndex();

            /**
             * Défini une fonction de callback pour un événement
             */
            void setCallback(Event event,std::function<bool()> callback);

            //virtual void nextAnimationFrame();

            /**
             * Nomme l'objet (utile pour le deboggage)
             */
            void setName(std::string name);

            /**
             * Récupére le nom de l'objet
             */
            std::string getName();

            /**
             * Vérifie si un point est dans le rectangle d'affichage réél d'un objet
             */
            bool isOnScreenRect(SDL_Point p);

            /**
             * Vérifie si un point est dans le rectangle d'affichage d'un objet
             */
            bool isOnDisplayRect(SDL_Point p);

            /**
             * Vérifie si l'objet posséde un parent
             */
            bool hasParent();

            /**
             * Indique si l'objet à le focus
             */
            bool getFocus();

            /**
             * Change le focus de l'objet
             */
            void setFocus(bool focus);

            /**
             * Défini un effet fade out
             */
            void setFadeOut(float fadeSpeed);

            /**
             * Défini un effet fade in
             */
            void setFadeIn(float fadeSpeed);

            void setZoomIn();

            void setZoomOut();

            int zIndex;

            Object* findFocus(Object* oldObjectFocused);
            void setFocusable(bool focusable);
            bool isFocusable();
            // TODO : utiliser SDL_RegisterEvents pour gérer le focus (et faire repasser ces deux méthodes en protected)
            virtual bool gainFocusEvent();
            virtual bool looseFocusEvent();

        protected:

            /**
             * Détruit la texture
             */
            void destroyTexture();

            /**
             * Création de la texture
             */
            void createTexture();

            virtual bool mouseMotionEvent();
            virtual bool mouseEnterEvent();
            virtual bool mouseLeaveEvent();
            virtual bool mouseRightClickEvent(SDL_Point p);
            virtual bool mouseLeftClickEvent(SDL_Point p);
            virtual bool mouseRightDragEvent(SDL_Point p);
            virtual bool mouseLeftDragEvent(SDL_Point p);
            virtual bool mouseDropEvent(SDL_Point p);
            virtual bool mouseWheelUp(int amount);
            virtual bool mouseWheelDown(int amount);
            virtual bool textInputEvent(char* e);
            virtual bool textBackspaceEvent();
            virtual bool keyDownEvent(SDL_Keysym keysym);

            // Liste des objets contenu dans cet objet
            std::list<Object*> objectList;
            std::map<Event,std::function<bool()> > callbackMap;

            ObjectType myType;
            void setUUID();

            // texture de l'objet
            SDL_Texture* texture;

            // les différents rects necessaire à l'affichage de l'objet
            SDL_Rect displayRect, screenRect;

            /**
             * Applique la couleur de fond (en prenant en compte la valeur de alpha)
             */
            void applyBackgroundColor();

            /**
             * Applique les bordures
             */
            void applyBorderColor();

            void startZoom(float scaleFactor);
            void setReachScaleFactor(float scaleFactor);
            void setOldScaleFactor(float scaleFactor);
            void setReachRect(SDL_Rect rect);
            float getActualScaleFactor() const;
            float getReachScaleFactor() const;

            SDL_Color backgroundColor;
            SDL_Color borderColor;
            int alpha;


            //float reachScaleFactor, stepScaleFactor, oldScaleFactor;
        private:
            // identifiant unique de l'objet
            std::string UUID;

            // nom donné à l'objet, correspond à l'identifiant unique si non défini
            std::string name;

            // largeur et hauteur de l'objet
            int width, height;


            bool visible;
            bool mouseIn;
            bool mouseRightClick;
            bool mouseLeftClick;
            bool mouseRightDrag;
            bool mouseLeftDrag;
            bool focus;
            bool focusable;

            float fadeSpeed;
            int zoomTime, zoomTimeStart;
            SDL_Rect reachRect;
            float oldScaleFactor, actualScaleFactor, reachScaleFactor, zoomSpeed;
            bool fadeOutEffect, fadeInEffect, zoomInEffect, zoomOutEffect;

            SDL_Point startDragPoint;
            SDL_Point stopDragPoint;
            Object *parent;

            float stepX, stepY, stepW, stepH;

    };

    /**
     * Fonction de comparaison du Zindex entre deux objets
     */
    template<typename T> bool ObjectComparator( const T * const &obj1, const T * const &obj2)
    {
        return obj1->zIndex < obj2->zIndex;
    };

}

#endif // OBJECT_H
