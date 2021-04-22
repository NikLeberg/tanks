/**
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
    char key[32] = {0};
    char type[32] = {0};
    char textureKey[32] = {0};
    char spriteType[16] = {0};
    int sizeXorDestX = 0;
    int sizeYorDestY = 0;
    int subSpriteCountorDestW = 0;
    int destH = 0;
    int pivotX = 0;
    int pivotY = 0;

    spriteResource->type = RESOURCETYPE_SPRITE;

    // Allozieren eines Sprites
    sprite_t *loadedSprite = malloc(sizeof(sprite_t));
    if (!loadedSprite) {
        printf("Memory fuer Sprite konnte nicht alloziert werden! Sprite_Load()\n");
        return ERR_MEMORY;
    }
    (*loadedSprite) = (sprite_t){0};

    // Konfiguration lesen
    int count = sscanf(config, "%31s %31s %31s %15s %d %d %d %d %d %d", key, type, textureKey, spriteType, &sizeXorDestX, &sizeYorDestY, &subSpriteCountorDestW, &destH, &pivotX, &pivotY);

    if (count < 3) { // Arguentanzahl Fehlerüberwachung
        printf("Zu wenig Konfigurationsargumente! Sprite_Load()\n");
        free(loadedSprite);
        return ERR_FAIL;
    }

    // Laden der angegebenen Textur
    SDLW_GetTexture(textureKey, &loadedSprite->texture);
    if (!loadedSprite->texture) { 
        printf("Textur mit Schluessel [%s] existiert nicht! Sprite_Load()\n", textureKey);
        free(loadedSprite);
        return ERR_FAIL;
    }

    if (count == 10) {
        int w, h;
        SDL_QueryTexture(loadedSprite->texture, NULL, NULL, &w, &h);
        loadedSprite->source.w = w;
        loadedSprite->source.h = h;
        loadedSprite->destination.x = sizeXorDestX;
        loadedSprite->destination.y = sizeYorDestY;
        if (subSpriteCountorDestW <= 0)
            loadedSprite->destination.w = w;
        else
            loadedSprite->destination.w = subSpriteCountorDestW;
        if (destH <= 0)
            loadedSprite->destination.h = h;
        else
            loadedSprite->destination.h = destH;
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
        loadedSprite->destination.w = w;
        loadedSprite->destination.h = h;
        spriteResource->resource.sprite = loadedSprite;
        return ERR_OK;
    }

    if (count == 7) { // Laden einer animations Textur
        if (!strcmp(spriteType, "animation")) {
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

    printf("Konfigurationsargumente fuer %s ungueltig! Sprite_Load()\n", key);
    free(loadedSprite);
    return ERR_FAIL;
}

int Sprite_CreateText(char *text, char *font, SDL_Color color, sprite_t *sprite) {
    if (!sprite) {
        printf("Sprite ungueltig! Sprite_CreateText()\n");
        return ERR_NULLPARAMETER;
    }

    if (SDLW_CreateTextTexture(text, font, color, &sprite->texture)) {
        return ERR_FAIL;
    }

    int w, h;
    SDL_QueryTexture(sprite->texture, NULL, NULL, &w, &h);
    sprite->source = (SDL_Rect){0, 0, w, h};
    sprite->destination.w = w;
    sprite->destination.h = h;

    return ERR_OK;
}

int Sprite_SetRelativeToPivot(sprite_t sprite, double parentRotation, SDL_Point parentPivot, sprite_t *calculatedSprite) {
    if (!calculatedSprite) {
        printf("Sprite resultat ungueltig! Sprite_SetRelativePivot()\n");
        return ERR_NULLPARAMETER;
    }

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
        printf("Sprite ungueltig! Sprite_SetFrame()\n");
        return ERR_NULLPARAMETER;
    }
    if (sprite->multiSpriteCount <= index || index < 0) {
        printf("Index zu gross! %d maximal %d Sprite_SetFrame()\n", index, sprite->multiSpriteIndex);
        return ERR_PARAMETER;
    }
    if (!sprite->texture) {
        printf("Sprite Textur ungueltig! Sprite_SetFrame()\n");
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
        printf("Sprite ungueltig! Sprite_SetFrame()\n");
        return ERR_NULLPARAMETER;
    }

    // Setzen des nächsten Indexes, mit wrapping auf Index = 0
    int index = sprite->multiSpriteIndex + 1;
    index %= sprite->multiSpriteCount;
    return Sprite_SetFrame(sprite, index);
}
