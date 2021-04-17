/**
 * @file sprite.h
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Funktionen für das Handling von sprites.
 * @version 1.0
 * @date 2021-04-03
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 * Siehe erweiterte Dokumentation in \ref Sprite
 *
 */

#pragma once


/*
 * Includes
 *
 */
#include <SDL_image.h>


/*
 * Typdeklarationen
 *
 */

struct sdlwResource_s; //!< Vorwärtsdeklaration einer \ref sdlwResource_t Struktur

/**
 * @brief Struktur für das Anzeigen von Texturen
 * <a href="https://docs.unity3d.com/Manual/SpriteEditor.html">Multisprite im Sinne von Automatic Slicing (Grid by cell count) von Unity</a>
 */
typedef struct {
    SDL_Texture *texture; //!< Die Sprite-Textur
    SDL_Rect source;     //!< Der Ausschnitt der gezeichnet werden soll
    SDL_Rect destination; //!< Die Grösse und Offset der mit der die Textur gezeichnet werden soll, relativ zu der Position
    SDL_Point pivot;      //!< Der Rotationspivot dieses Sprites, Relativ zum Zentrum der Textur
    double rotation;       //!< Die Rotation der Textur
    SDL_Point position;   //!< Die Position der Textur, zeigt auf das Zentrum der Textur

    SDL_Point multiSpriteSize; //!< Unterteilung von Sprite in Subbsprites in Anzahl x-Richtung und Anzahl y-Richtung
    int multiSpriteCount; //!< Anzahl von vorhandenen subsprites 0 = Kein multisprite
    int multiSpriteIndex;      //!< Jetziges angezeigter Subsprite
} sprite_t;


/*
 * Öffentliche Funktionen
 *
 */

/**
 * @brief Lädt ein Sprite anhand der Konfigurationszeile.
 * Darf nur von sdlWrapper aufgerufen werden.
 * 
 * @param[in] config Die Konfigurationszeile.
 * @param[in] spriteResource Speicherort des Sprites.
 *
 * @return 0 oder Errorcode
 */
int Sprite_Load(char *config, struct sdlwResource_s *spriteResource);

/**
 * @brief Rotiert ein Sprite um ein externes Pivot.
 * Es wird angenommen, dass die Position des \p parentPivot gleich ist wie die
 * des \p sprite. Das \p sprite wird nicht verändert.
 * 
 * @param[in] sprite Das Sprite dessen Angaben für Position und Rotation verwendet wird.
 * @param[in] parentRotation Die Rotation des parent Sprites.
 * @param[in] parentPivot Das Pivot des parent Sprites.
 * @param[out] calculatedSprite Die berechneten Daten des \p sprite.
 * 
 * @return 0 oder Errorcode
 */
int Sprite_SetRelativeToPivot(sprite_t sprite, double parentRotation, SDL_Point parentPivot, sprite_t *calculatedSprite);

/**
 * @brief Setzt das Subsprite auf den angegebenen Index.
 * Berechnet \ref sprite_t.source so, dass \p index auf das n-te Subsprite zeigt.
 * 
 * @param[in] sprite Das Multisprite dessen Anzeige geändert wird.
 * @param[in] index Der Index auf welches Subsprite gewechselt wird.
 *
 * @return 0 oder Errorcode
 */
int Sprite_SetFrame(sprite_t *sprite, int index);

/**
 * @brief Zeigt das nächste Subsprite an.
 * Ist das letzte Subsprite erreicht, wird wieder das erste angezeigt.
 * 
 * @param[in] sprite Das Multisprite von welchem das nächste Subsprite angezeigt wird.
 *
 * @return 0 oder Errorcode
 */
int Sprite_NextFrame(sprite_t *sprite);

/**
 * @brief Erstellt eine Textur anhand des angegebenen Strings.
 * Für verschiedene Schriftauflösung (Grösse) muss je eine andere Schriftart geladen sein.
 * Die \p font muss zuerst mit SDLW_LoadResources() geladen worden sein.
 *  
 * Die Grösse des angegebenen Sprites wird auf die Grösse der erstellten Textur gesetzt.
 *
 * @note text und font sollten nicht Null sein.
 * Nicht gebraucht Text Texturen müssen manuell befreit werden.
 *
 * @param[in] text Der Text der ausgegeben wird
 * @param[in] font Die ID der verwendeten Schriftart
 * @param[in] color Die Farbe in der der Text ausgegeben wird
 * @param[out] sprite Die erstellte Textur wird in den angegebenen Sprite geladen
 *
 * @return 0 oder Errorcode
 */
int Sprite_CreateText(char *text, char *font, SDL_Color color, sprite_t *sprite);
