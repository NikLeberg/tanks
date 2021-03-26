/**
 * @file sdlWrapper.h
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Wrapper der SDL Bibliothek
 * @version 0.0
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */

#pragma once


 /*
  * Includes
  *
  */
#include <SDL_image.h>
#include <SDL_ttf.h>


  /*
  * Typdeklarationen
  *
  */

  /**
   * @brief Struktur für das Anzeigen von Texturen
   *
   */
typedef struct {
    SDL_Texture* texture;   //!< Die Sprite-Textur
    SDL_Rect* source;       //!< Der Ausschnitt der gezeichnet werden soll
    SDL_Rect* destination;  //!< Die Grösse und Offset der mit der die Textur gezeichnet werden soll, relativ zu der Position
    SDL_Rect *pivot;        //!< Der Rotationspivot dieses Sprites
    float rotation;         //!< Die Rotation der Textur
    float positionX;        //!< Die X-Position der Textur
    float positionY;        //!< Die Y-Position der Textur
} sprite_t;

/**
 * @brief Union von möglichen ressourcen
 *
 */
typedef union {
    SDL_Texture* texture;   //!< Speicher für Textur
    TTF_Font* font;         //!< Speicher für Font
}sdlwResourceUnion_t;

/**
 * @brief Struktur zum Speichern von ressourcen
 *
 */
typedef struct {
    int type;                       //!< Element Type 0 = Textur Normal, 10 = Textur Welt, 20 = Textur Explosion
    char key[32];                   //!< Die ID die dieses Element besitzt und zugegriffen werden kann
    sdlwResourceUnion_t resource;    //!< Die Ressource die geladen wurde
}sdlwResource_t;

/*
 * Öffentliche Funktionen
 *
 */

 /**
  * @brief Initialisierung der SDL Bibliotheken und öffnet ein Fenster.
  * Trifft bein Initialisieren ein Fehler auf, werden alle Bibliotheken entladen und das Fenster geschlossen. 
  *
  * @note Erlaubte Wertebereich von \p windowWidth und \p windowHeight 1..2000
  * 
  * @param[in] windowWidth Breite des zu erstellenden Windows. 
  * @param[in] windowHeight Höhe des zu erstellenden Windows.
  *
  * @return 0 oder Errorcode
  */
int SDLW_Init(int windowWidth, int windowHeight);

/**
 * @brief Schliesst das Fenster und alle geladenen Bibliotheken.
 * Zusätzlich werden alle geladenen Ressourcen freigegeben.
 *
 */
void SDLW_Quit();

/**
 * @brief Lädt ressourcen anhand der angegebenen Ressourcenkonfigurationsdatei.
 * Die Konfigurationsdatei ist im Format [Key File Type] aufgebaut.
 * Key maximal 31 Zeichen: ID
 * File maximal 31 Zeichen: Pfad
 * Type : texture, world, explosion, font, config
 *
 * @param[in] resourceConfigLocation Pfad der Ressourcenkonfigurationsdatei
 *
 * @return 0 oder Errorcode
 */
int SDLW_LoadResources(char* resourceConfigLocation);

/**
 * @brief Gibt eine Geladene Texturressource zurück.
 * Die ID muss mit SDLW_LoadResources() per Key geladen worden sein.
 *
 * @param[in] id Die ID der Textur die gesucht wird
 * @param[out] texture Pointer zur Textur. Null bei nicht gefundener Textur
 *
 * @return 0 oder Errorcode
 */
int SDLW_GetTexture(char* id, SDL_Texture** texture);

/**
 *  @brief Zeichnet einen Sprite 
 *  
 *  @note Die \ref sprite_t.texture und das \ref sprite_t.destination dürfen nicht Null sein.
 *
 *  @param[in] sprite Der Sprite der gezeichnet wird
 *
 *  @return 0 oder Errorcode
 */
int SDLW_DrawTexture(sprite_t sprite);

/**
 *  @brief Zeichnet ein gefülltes Reckteck anhand der angegebenen Farbe
 *  
 *  @param[in] rect Die Position und Grösse des zu zeichnenden Rechtecks
 *  @param[in] color Die Farbe des Rechtecks
 *
 *  @return 0 oder Errorcode
 */
int SDLW_DrawFilledRect(SDL_Rect rect, SDL_Color color);

/**
 *  @brief Erstellt eine Textur anhand des angegebenen strings
 *  Für verschiedene Schriftauflösung (Grösse) muss je ein anderer Font geladen sein.
 *  Die \p font muss zuerst mit SDLW_LoadResources() geladen worden sein.
 *   
 *  @note text und font sollten nicht Null sein.
 *  Nicht gebraucht Text Texturen müssen manuell befreit werden.
 *
 *  @param[in] text Der Text der ausgegeben wird
 *  @param[in] font Die ID der verwendeten Font
 *  @param[in] color Die Farbe in der der Text ausgegeben wird
 *  @param[out] texture Die erstellte Textur mit dem gegebenen Text darauf
 *
 *  @return 0 oder Errorcode
 */
int SDLW_CreateTextTexture(char* text, char* font, SDL_Color color, SDL_Texture** texture);
