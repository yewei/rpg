#include "../include/font.h"

SDL_Surface* myTTF_RenderString_Blended(TTF_Font* font, const std::string& str, SDL_Color fg)
{
    SDL_Surface* textbuf;
    //Get Unicode
    std::vector<Uint16> unicodeUnit = getUnicode(str);
    int arraySize = unicodeUnit.size();
    Uint16* perOne = new Uint16[arraySize+1];
    for ( int i = 0; i < arraySize; i++ )
        perOne[i] = unicodeUnit[i];
    perOne[arraySize] = 0;

    
    //Render the new text
    textbuf = TTF_RenderUNICODE_Blended(font, perOne, fg);

    //Free the text buffer and return
    delete [] perOne;
    return textbuf;
}

SDL_Surface* myTTF_RenderString_Solid(TTF_Font* font, const std::string& str, SDL_Color fg)
{
    SDL_Surface* textbuf;
    //Get Unicode
    std::vector<Uint16> unicodeUnit = getUnicode(str);
    int arraySize = unicodeUnit.size();
    Uint16* perOne = new Uint16[arraySize+1];
    for ( int i = 0; i < arraySize; i++ )
        perOne[i] = unicodeUnit[i];
    perOne[arraySize] = 0;

    
    //Render the new text
    textbuf = TTF_RenderUNICODE_Solid(font, perOne, fg);

    //Free the text buffer and return
    delete [] perOne;
    return textbuf;
}

SDL_Surface* myTTF_RenderString_Shaded(TTF_Font* font, const std::string& str, SDL_Color fg, SDL_Color bg)
{
    SDL_Surface* textbuf;
    //Get Unicode
    std::vector<Uint16> unicodeUnit = getUnicode(str);
    int arraySize = unicodeUnit.size();
    Uint16* perOne = new Uint16[arraySize+1];
    for ( int i = 0; i < arraySize; i++ )
        perOne[i] = unicodeUnit[i];
    perOne[arraySize] = 0;

    
    //Render the new text
    textbuf = TTF_RenderUNICODE_Shaded(font, perOne, fg, bg);

    //Free the text buffer and return
    delete [] perOne;
    return textbuf;
}
