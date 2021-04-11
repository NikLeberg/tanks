/**
 * @file sdlWrapper.h
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Wrapper der SDL Bibliothek
 * @version 0.3
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
#define SDL_MAIN_HANDLED //!< Weise SDL an kein SDL_main zu erstellen.
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "sprite.h"


/*
 * Typdeklarationen
 *
 */

/**
 * @brief Enum (Flag) welche Struktur in \ref sdlwResourceUnion_t gebraucht wird
 *
 */
typedef enum {
    RESOURCETYPE_NONE = 0,                                                                             //!< Keine ressource
    RESOURCETYPE_TEXTURE_N = 1,                                                                        //!< Normale Textur
    RESOURCETYPE_TEXTURE_RT = 2,                                                                       //!< RenderTarget textur, Unbenutzt
    RESOURCETYPE_TEXTURE_BM_STAMP = 4,                                                                 //!< Stamp Textur
    RESOURCETYPE_TEXTURE_BM_MASK = 8,                                                                  //!< Mask Textur
    RESOURCETYPE_SOUND_EFFECT = 16,                                                                    //!< Soundeffekt
    RESOURCETYPE_SOUND_MUSIC = 32,                                                                     //!< Hintergrundmusik
    RESOURCETYPE_FONT = 64,                                                                            //!< Font
    RESOURCETYPE_SPRITE = 128,                                                                         //!< Sprite, mit oder ohne animationsdaten
    RESOURCETYPE_TEXTURE_BM = RESOURCETYPE_TEXTURE_BM_STAMP | RESOURCETYPE_TEXTURE_BM_MASK,            //!< Eine Maske oder Stempel
    RESOURCETYPE_TEXTURE = RESOURCETYPE_TEXTURE_BM | RESOURCETYPE_TEXTURE_N | RESOURCETYPE_TEXTURE_RT, //!< Eine Textur
    RESOURCETYPE_ANY = 2147483647,                                                                     //!< Alle flags sind gesetzt
} resourceType_t;

/**
 * @brief Union von möglichen Ressourcen
 *
 */
typedef union {
    void *any;              //!< Zugriff per void-Pointer
    SDL_Texture *texture;   //!< Speicher für Textur
    TTF_Font *font;         //!< Speicher für Font
    Mix_Chunk *soundEffect; //!< Speicher für Soundeffekte
    Mix_Music *bgMusic;     //!< Speicher für Hintergrundmusik
    sprite_t *sprite;       //!< Speicher für Sprite
} sdlwResourceUnion_t;

/**
 * @brief Struktur zum Speichern von Ressourcen
 *
 */
typedef struct sdlwResource_s {
    resourceType_t type;          //!< Typ der Ressource gemäss \ref resourceType_t
    char key[32];                 //!< Die ID die dieses Element besitzt und über die sie gefunden werden kann
    sdlwResourceUnion_t resource; //!< Die Ressource die geladen wurde
} sdlwResource_t;


/*
 * Öffentliche Funktionen
 *
 */

/**
  * @brief Initialisiert sie SDL Bibliotheken und öffnet ein Fenster.
  * Trifft bein Initialisieren ein Fehler auf, werden alle Bibliotheken entladen und das Fenster geschlossen. 
  *
  * @note Erlaubte Wertebereich von \p windowWidth und \p windowHeight 1..2000
  * 
  * @param[in] windowWidth Breite des zu erstellenden Fensters. 
  * @param[in] windowHeight Höhe des zu erstellenden Fensters.
  *
  * @return 0 oder Errorcode
  */
int SDLW_Init(int windowWidth, int windowHeight);

/**
 * @brief Schliesst das Fenster und alle geladenen Bibliotheken.
 * Zusätzlich werden alle geladenen Ressourcen freigegeben.
 *
 * @return 0 oder Errorcode
 */
int SDLW_Quit();

/**
 * @brief Lädt Ressourcen anhand der angegebenen Ressourcenkonfigurationsdatei.
 * Die Konfigurationsdatei ist im Format [Key File Type] aufgebaut.
 * Key maximal 31 Zeichen: ID
 * File maximal 63 Zeichen: Pfad
 * \ref ConfigDoku
 *
 * @param[in] resourceConfigLocation Pfad der Ressourcenkonfigurationsdatei
 *
 * @return 0 oder Errorcode
 */
int SDLW_LoadResources(char *resourceConfigLocation);

/**
 * @brief Gibt eine Geladene Ressource zurück.
 * Die ID muss mit SDLW_LoadResources() per Key geladen worden sein.
 *
 * @param[in] id Die ID der Ressource die gesucht wird
 * @param[in] type Der Typ der Ressource die geladen werden soll
 * @param[out] resource Pointer zur Ressource. Null bei nicht gefundener Ressource
 *
 * @return 0 oder Errorcode
 */
int SDLW_GetResource(char *id, resourceType_t type, void **resource);

/**
 * @brief Vereinfachendes Makro für \ref SDLW_GetResource.
 * Lädt die per \p id angegebene Textur.
 *
 * @param[in] id Die ID der Textur die gesucht wird
 * @param[out] texture Pointer zur Textur. Null bei nicht gefundener Textur
 *
 * @return 0 oder Errorcode
 */
#define SDLW_GetTexture(id, texture) SDLW_GetResource((id), RESOURCETYPE_TEXTURE, (void **)(texture))

/**
 *  @brief Zeichnet einen Sprite.
 *  
 *  @note Die \ref sprite_t.texture und das \ref sprite_t.destination dürfen nicht Null sein.
 *
 *  @param[in] sprite Der Sprite der gezeichnet wird
 *
 *  @return 0 oder Errorcode
 */
int SDLW_DrawTexture(sprite_t sprite);

/**
 * @brief Zeichnet ein gefülltes Reckteck anhand der angegebenen Farbe
 * 
 * @param[in] rect Die Position und Grösse des zu zeichnenden Rechtecks
 * @param[in] color Die Farbe des Rechtecks
 *
 * @return 0 oder Errorcode
 */
int SDLW_DrawFilledRect(SDL_Rect rect, SDL_Color color);

/**
 * @brief Erstellt eine Textur anhand des angegebenen Strings.
 * Für verschiedene Schriftauflösung (Grösse) muss je ein anderer Font geladen sein.
 * Die \p font muss zuerst mit SDLW_LoadResources() geladen worden sein.
 * 
 * @note text und font sollten nicht Null sein.
 * Nicht gebraucht Text Texturen müssen manuell befreit werden.
 *
 * @param[in] text Der Text der ausgegeben wird
 * @param[in] font Die ID der verwendeten Font
 * @param[in] color Die Farbe in der der Text ausgegeben wird
 * @param[out] texture Die erstellte Textur mit dem gegebenen Text darauf
 *
 * @return 0 oder Errorcode
 */
int SDLW_CreateTextTexture(char *text, char *font, SDL_Color color, SDL_Texture **texture);

/**
 * @brief Übermalt das ganze Fenster mit der angegebenen Farbe.
 * 
 * @param[in] color Die Farbe mit der das Fenster übermalt wird.
 * 
 * @return 0 oder Errorcode
 */
int SDLW_Clear(SDL_Color color);

/**
 * @brief Ruft SDL_RenderPresent auf.
 *
 * @return 0 oder Errorcode
 */
int SDLW_Render();

/**
 * @brief Spielt eine Musik ab.
 * Es kann nur eine Musik gleichzeitig laufen.
 * Sollte nur für Hintergrundmusik benötigt werden.
 * 
 * @param[in] musicid Die Id der Musik die gespielt werden soll
 * 
 * @return 0 oder Errorcode
 */
int SDLW_PlayMusic(char *musicid);

/**
 * @brief Spielt einen Soundeffekt ab.
 * 
 * @param[in] chunk Die Id des Effekts der gespielt werden soll
 * 
 * @return 0 oder Errorcode
 */
int SDLW_PlaySoundEffect(char *chunk);
