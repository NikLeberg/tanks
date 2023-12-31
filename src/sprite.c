﻿/**
 * @file sprite.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Funktionen für das Handling von sprites.
 * @version 1.0
 * @date 2021-04-03
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */

/*
 * Includes
 *
 */
#include <SDL_image.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sprite.h"
#include "sdlWrapper.h"
#include "error.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Sprite_Load(char *config, struct sdlwResource_s *spriteResource) {
    char key[32] = {0}; // Sprite ID-Schlüssel
    char type[32] = {0}; // Ressurcentyp = sprite
    char textureKey[32] = {0}; // Texturen ID
    char spriteType[16] = {0}; // Typ des Sprites
    int sizeXorDestX = 0; // Breite oder X-Offset
    int sizeYorDestY = 0; // Höhe oder Y-Offset
    int subSpriteCountorDestW = 0; // Anzahl Sprites oder Breite
    int destH = 0; // Höhe
    int pivotX = 0; // Pivot X-Offset
    int pivotY = 0; // Pivot Y-Offset

    spriteResource->type = RESOURCETYPE_SPRITE;

    // Allozieren eines Sprites
    sprite_t *loadedSprite = malloc(sizeof(sprite_t));
    if (!loadedSprite) {
        SDL_Log("Memory fuer Sprite konnte nicht alloziert werden! Sprite_Load()\n");
        return ERR_MEMORY;
    }
    (*loadedSprite) = (sprite_t){0};

    // Konfiguration lesen
    int count = sscanf(config, "%31s %31s %31s %15s %d %d %d %d %d %d", key, type, textureKey, spriteType, &sizeXorDestX, &sizeYorDestY, &subSpriteCountorDestW, &destH, &pivotX, &pivotY);

    if (count < 3) { // Arguentanzahl Fehlerüberwachung
        SDL_Log("Zu wenig Konfigurationsargumente! Sprite_Load()\n");
        free(loadedSprite);
        return ERR_FAIL;
    }

    // Laden der angegebenen Textur
    SDLW_GetTexture(textureKey, &loadedSprite->texture);
    if (!loadedSprite->texture) { 
        SDL_Log("Textur mit Schluessel [%s] existiert nicht! Sprite_Load()\n", textureKey);
        free(loadedSprite);
        return ERR_FAIL;
    }

    if (count == 10 && !strcmp(spriteType, "size")) { // Sprite mit destination grösse
        //Source des Sprites definieren
        int w, h;
        SDL_QueryTexture(loadedSprite->texture, NULL, NULL, &w, &h);
        loadedSprite->source.w = w;
        loadedSprite->source.h = h;
        // Destination des Sprites definieren
        loadedSprite->destination.x = sizeXorDestX;
        loadedSprite->destination.y = sizeYorDestY;
        if (subSpriteCountorDestW <= 0) // Breite undefiniert
            loadedSprite->destination.w = w;
        else
            loadedSprite->destination.w = subSpriteCountorDestW;
        if (destH <= 0) // Höhe undefiniert
            loadedSprite->destination.h = h;
        else
            loadedSprite->destination.h = destH;
        // Pivot offset definieren
        loadedSprite->pivot.x = pivotX;
        loadedSprite->pivot.y = pivotY;
        spriteResource->resource.sprite = loadedSprite;
        return ERR_OK;
    }

    if (count == 3) { // Laden einer normalen Textur
        // Setzen der standard Breite und Höhe
        int w, h;
        SDL_QueryTexture(loadedSprite->texture, NULL, NULL, &w, &h);
        loadedSprite->source.w = w;
        loadedSprite->source.h = h;
        // Definieren des destination
        loadedSprite->destination.w = w;
        loadedSprite->destination.h = h;
        spriteResource->resource.sprite = loadedSprite;
        return ERR_OK;
    }

    if (count == 7) { // Laden einer animations Textur
        if (!strcmp(spriteType, "animation")) {
            //Multisprite daten setzen
            loadedSprite->multiSpriteSize = (SDL_Point){sizeXorDestX, sizeYorDestY};
            loadedSprite->multiSpriteCount = subSpriteCountorDestW;
            // Setzen der standard Breite und Höhe
            Sprite_SetFrame(loadedSprite, 0);
            loadedSprite->destination.w = loadedSprite->source.w;
            loadedSprite->destination.h = loadedSprite->source.h;
            spriteResource->resource.sprite = loadedSprite;
            return ERR_OK;
        }
    }
    // Keine gültige konfiguration
    SDL_Log("Konfigurationsargumente fuer %s ungueltig! Sprite_Load()\n", key);
    free(loadedSprite);
    return ERR_FAIL;
}

int Sprite_CreateText(char *text, char *font, SDL_Color color, sprite_t *sprite) {
    if (!sprite) { // Fehlerüberprüfung
        SDL_Log("Sprite ungueltig! Sprite_CreateText()\n");
        return ERR_NULLPARAMETER;
    }

    if (SDLW_CreateTextTexture(text, font, color, &sprite->texture)) { // Erstellung einer Textur mit gegebenem Text
        return ERR_FAIL;
    }

    // Grösse des Sprites auf görsse der Textur setzen
    int w, h;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
    sprite->source = (SDL_Rect){0, 0, w, h};
    sprite->destination.w = w;
    sprite->destination.h = h;

    return ERR_OK;
}

int Sprite_SetRelativeToPivot(sprite_t sprite, double parentRotation, SDL_Point parentPivot, sprite_t *calculatedSprite) {
    if (!calculatedSprite) { // Fehlerüberprüfung
        SDL_Log("Sprite resultat ungueltig! Sprite_SetRelativePivot()\n");
        return ERR_NULLPARAMETER;
    }

    // Cosinus und Sinus vorberechnen
    double cosRotation = cos(parentRotation * M_PI / 180.0);
    double sinRotation = sin(parentRotation * M_PI / 180.0);

    // Parentpivot relativ zu Spritepivot
    int deltaX = sprite.destination.x - parentPivot.x + sprite.pivot.x;
    int deltaY = sprite.destination.y - parentPivot.y + sprite.pivot.y;
    // Relativer Pivotvektor rotieren
    int rotatedX = (int)(cosRotation * deltaX - sinRotation * deltaY);
    int rotatedY = (int)(sinRotation * deltaX + cosRotation * deltaY);
    // Offset von Sprite wieder hinzufügen
    rotatedX -= -parentPivot.x + sprite.destination.x + sprite.pivot.x;
    rotatedY -= -parentPivot.y + sprite.destination.y + sprite.pivot.y;
    // Geänderte Daten setzen
    sprite.rotation += (float)parentRotation;
    sprite.position.x += rotatedX;
    sprite.position.y += rotatedY;

    (*calculatedSprite) = sprite;

    return ERR_OK;
}

int Sprite_SetFrame(sprite_t *sprite, int index) {
    // Fehlerüberprüfung
    if (!sprite) {
        SDL_Log("Sprite ungueltig! Sprite_SetFrame()\n");
        return ERR_NULLPARAMETER;
    }
    if (sprite->multiSpriteCount <= index || index < 0) {
        SDL_Log("Index zu gross! %d maximal %d Sprite_SetFrame()\n", index, sprite->multiSpriteIndex);
        return ERR_PARAMETER;
    }
    if (!sprite->texture) {
        SDL_Log("Sprite Textur ungueltig! Sprite_SetFrame()\n");
        return ERR_PARAMETER;
    }

    // Bestimmung der Breite und Höhe eines einzelnen Segments
    int w, h;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
    w /= sprite->multiSpriteSize.x;
    h /= sprite->multiSpriteSize.y;

    // Bestimmung der x- und y-Position anhand des Index;
    int x, y;
    x = index % sprite->multiSpriteSize.x;
    y = index / sprite->multiSpriteSize.x;

    // Setzen der berechneten Daten
    sprite->source.x = x * w;
    sprite->source.y = y * h;
    sprite->source.w = w;
    sprite->source.h = h;
    sprite->multiSpriteIndex = index;

    return ERR_OK;
}

int Sprite_NextFrame(sprite_t *sprite) {
    // Fehlerüberprüfung
    if (!sprite) {
        SDL_Log("Sprite ungueltig! Sprite_SetFrame()\n");
        return ERR_NULLPARAMETER;
    }

    // Setzen des nächsten Indexes, mit wrapping auf Index = 0
    int index = sprite->multiSpriteIndex + 1;
    index %= sprite->multiSpriteCount;
    return Sprite_SetFrame(sprite, index);
}
