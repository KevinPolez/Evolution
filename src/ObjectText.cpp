#include "ObjectText.h"

#include <vector>
#include <sstream>
#include <iterator>
#include <string>

#include "Color.h"
#include "Display.h"

namespace evo
{
    /**
     * Constructeur
     */
    ObjectText::ObjectText() : Object(), fontSize(24), textTexture(NULL), textNeedRedraw(false)
    {
        this->setFontColor(Color::Font);
        this->setBackgroundColor(Color::Background);
        this->setBorderColor(Color::Border);
    }

    /**
     * Destructeur
     */
    ObjectText::~ObjectText() {

        SDL_DestroyTexture(this->textTexture);
    }

    /**
     * Definie le texte
     */
    void ObjectText::setText(std::string text)
    {
        this->paragraphes.clear();
        this->paragraphes.push_back(text);
        this->textNeedRedraw = true;
    }

    /**
     * Ajoute un paragraphe
     */
    void ObjectText::addParagraphe(std::string paragraphe)
    {
        this->paragraphes.push_back(paragraphe);
        this->textNeedRedraw = true;
    }

    void ObjectText::setParagrapheMargin(int paragrapheMargin)
    {
        this->paragrapheMargin = paragrapheMargin;
    }

    int ObjectText::getParagrapheMargin()
    {
        return this->paragrapheMargin;
    }

    /**
     * Defini la couleur de la police
     */
    void ObjectText::setFontColor(SDL_Color fontColor)
    {
        this->fontColor = fontColor;
        this->textNeedRedraw = true;
    }

    /**
     * Defini la taille de la police
     */
    void ObjectText::setFontSize(int fontSize)
    {
        this->fontSize = fontSize;
        this->textNeedRedraw = true;
    }

    /**
     * Indique si le texte doit être réappliqué sur la texture
     */
    bool ObjectText::getTextNeedRedraw()
    {
        return this->textNeedRedraw;
    }

    /**
     * Defini si le texte doit être réappliqué sur la texture
     */
    void ObjectText::setTextNeedRedraw(bool textNeedRedraw)
    {
        this->textNeedRedraw = textNeedRedraw;
    }

    /**
     * Fourni la longueur de la texture text
     */
    int ObjectText::getTextWidth()
    {
        return this->textW;
    }

    /**
     * Fourni la largeur de la texture text
     */
    int ObjectText::getTextHeight()
    {
        return this->textH;
    }

    /**
     * Fourni la texture texte
     */
    SDL_Texture* ObjectText::getTextTexture()
    {
        return this->textTexture;
    }

    /**
     * Construit la texture texte
     */
    void ObjectText::buildTextTexture()
    {
        if ( this->paragraphes.empty() )
        {
            return;
        }

        this->font = TTF_OpenFont("resource/font/calibri.ttf",this->fontSize);

        if ( this->textTexture != NULL)
        {
            SDL_DestroyTexture(this->textTexture);
            this->textTexture = NULL;
        }

        // pour chacun des paragraphes
        std::vector<SDL_Surface*> lines;
        std::vector<int> paragrapheCounter;

        int h_used = 0;
        std::list<std::string>::iterator itr;
        for (itr = paragraphes.begin(); itr != paragraphes.end(); itr++)
        {
            std::vector<std::string> tokens;

            std::istringstream iss((*itr));

            // division du texte en un tableau de mots
            std::copy(
                std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                back_inserter(tokens));

            int w,h;
            int w_used = 0;
            std::string line;

            line.clear();
            // pour chacun des mots
            std::vector<std::string>::iterator it;
            for( it = tokens.begin(); it != tokens.end(); ++it) {

                // ajout du caractère d'espacement si il ne s'agit pas du dernier mot
                if ( it != tokens.end())
                {
                    (*it) += ' ';
                }

                // calcul de la taille de la surface necessaire
                TTF_SizeText(this->font, (*it).c_str(), &w, &h);

                // si il faut passer à la ligne
                if ( w + w_used >= this->getWidth() )
                {
                    // on créé la surface correspondant à la ligne précédente
                    SDL_Surface* surface;
                    surface = TTF_RenderText_Blended(this->font, line.c_str(),this->fontColor);
                    lines.push_back(surface);
                    h_used += h;

                    w_used = 0;
                    line.clear();
                }

                // ajout du mot à la ligne
                line += (*it);
                w_used += w;
            }
            // gestion de la dernière ligne
            SDL_Surface* surface;
            surface = TTF_RenderText_Blended(this->font, line.c_str(),this->fontColor);
            lines.push_back(surface);
            h_used += h;

            paragrapheCounter.push_back(lines.size()); // ajoute le nombre de ligne avant le prochain paragraphe
        }

        h_used += (paragraphes.size()) * paragrapheMargin; // ajoute les marges entre paragraphe à la texture

        // création de la texture de largeur et de hauteur correspondant a la totalité du texte
        this->textTexture = Display::CreateTexture(this->getWidth(),h_used);

        // cette texture doit être transparente.
        SDL_SetTextureBlendMode(this->textTexture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(Display::renderer, this->textTexture);
        SDL_SetRenderDrawColor(Display::renderer, 0,0,0,0);
        SDL_RenderClear(Display::renderer);

        // pour chacune des lignes
        int lineCounter = 1;
        this->textH = 0;
        std::vector<SDL_Surface*>::iterator itSurface;
        auto it = paragrapheCounter.begin();
        for( itSurface = lines.begin(); itSurface != lines.end(); ++itSurface)
        {
            // creation de la texture temporaire correspond à la ligne
            SDL_Texture* texture = Display::CreateTextureFromSurface((*itSurface));

            SDL_Rect rect_src, rect_dest;
            rect_src.h = (*itSurface)->h;
            rect_src.w = (*itSurface)->w;
            rect_src.x = 0;
            rect_src.y = 0;


            rect_dest.h = (*itSurface)->h;
            rect_dest.w = (*itSurface)->w;
            rect_dest.x = 0;
            rect_dest.y = this->textH ;

            this->textH += (*itSurface)->h;

            // application de cette texture sur la texture principale
            Display::ApplyTexture(texture, rect_src, this->textTexture, rect_dest);

            // destruction de la texture temporaire
            SDL_DestroyTexture(texture);

            // gestion de la marge entre paragraphe
            if ( lineCounter == (*it) )
            {
                it++;
                this->textH += paragrapheMargin;
            }

            lineCounter++;
        }

        // libére toutes les surfaces
        for( itSurface = lines.begin(); itSurface != lines.end(); ++itSurface) {
            SDL_FreeSurface((*itSurface));
        }

        TTF_CloseFont(this->font);
    }
}
