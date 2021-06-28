/**
 * @file world.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Die Spielwelt
 * @version 0.0
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Stoll Simon
 *
 */


/*
 * Includes
 *
 */

#include <stdio.h>
#include <string.h>

#include "world.h"
#include "error.h"
#include "sdlWrapper.h"
#include "entity.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

static const int width = 1024; //!< Die Höhe der Welt
static const int height = 576; //!< Die Breite der Welt

static int init = 0;                  //!< Ist dieses Modul initialisiert
static int loaded = 0;                //!< Ist eine Welt geladen
static unsigned char *worldCollision; //!< Array mit Informationen zum Vordergrund
static sprite_t foreground = {0};     //!< Der Vordergrundsprite, kann darauf gezeichnet werden
static sprite_t background = {0};     //!< Der Hintergrundsprite


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief Aktualisiert das Kollisionsraster.
 * @warning Diese Funktion ist langsam 10-20 ms. So wenig wie möglich benutzen.
 * 
 * @return 0 oder Fehlercode
 */
static int UpdateWorld();


/*
 * Implementation öffentlicher Funktionen
 *
 */

int World_LoadConfig(char *config, struct sdlwResource_s *resource) {
    char key[32] = {0}; // Welten ID
    char type[32] = {0}; // Ressourcentyp = world
    char foregroundID[32] = {0}; // Fordergrundstextur
    char backgroundID[32] = {0}; // Hintergrundstextur
    char bgMusicID[32] = {0}; // Hintergrundsmusik

    int count = sscanf(config, "%31s %31s %31s %31s %31s", key, type, foregroundID, backgroundID, bgMusicID);

    if (count < 4 || count > 5) { // Argumentenanzalüberprüfung
        SDL_Log("Konfigurationszeile ungueltig! World_LoadConfig()\n");
        return ERR_FAIL;
    }

    // Konfiguration erstellen
    resource->type = RESOURCETYPE_WORLD;
    worldConfig_t *worldConfig = malloc(sizeof(worldConfig_t));

    if (!worldConfig) {
        SDL_Log("Memory Error! World_LoadConfig()\n");
        return ERR_MEMORY;
    }

    // Vordergrund Textur holen
    int code = SDLW_GetResource(foregroundID, RESOURCETYPE_TEXTURE, (void **)&worldConfig->foreground);
    if (code != ERR_OK)
        return code;
    // Hintergrund Textur holen
    code = SDLW_GetResource(backgroundID, RESOURCETYPE_TEXTURE, (void **)&worldConfig->background);
    if (code != ERR_OK)
        return code;

    // Hintergrundsmusik definieren
    if (count == 5)
        strcpy(worldConfig->bgMusic, bgMusicID);    
    else
        worldConfig->bgMusic[0] = '\0'; // Es wird keine Musik abgespielt

    // Abschluss
    resource->resource.world = worldConfig;
    return ERR_OK;
}

int World_Init() {
    if (init) { // Fehlerüberprüfung
        SDL_Log("Welt wurde schon initialisiert!\n");
        return ERR_FAIL;
    }

    // Initialisieren des Kollisionsraster auf eine gewisse grösse
    worldCollision = malloc(sizeof(int) * width * height * 4);
    if (!worldCollision) {
        SDL_Log("Memory Error! World_Init()\n");
        return ERR_MEMORY;
    }

    // Zerstörbarer Vordergrund erstellen
    SDL_Renderer *renderer = SDLW_GetRenderer(); // Renderer hohlen
    if (!renderer) {
        free(worldCollision);
        worldCollision = 0;
        SDL_Log("SDLW nicht richtig initialisiert! World_Init()\n");
        return ERR_FAIL;
    }
    foreground.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height); // Textur erstellen
    if (!foreground.texture) {
        free(worldCollision);
        worldCollision = 0;
        SDL_Log("SDL CreateTexture fehler! [%s]\n", SDL_GetError());
        return ERR_FAIL;
    }
    SDL_SetTextureBlendMode(foreground.texture, SDL_BLENDMODE_BLEND);
    // Vordergrund einstellen, 1px = 1Raster
    foreground.destination.w = width;
    foreground.destination.h = height;
    foreground.destination.x = width / 2;
    foreground.destination.y = height / 2;
    foreground.source.w = width;
    foreground.source.h = height;
    // Hintergrund einstellen, Skalierung kann unterschiedlich sein
    background.destination.w = width;
    background.destination.h = height;
    background.destination.x = width / 2;
    background.destination.y = height / 2;

    // Abschluss
    init = 1;
    loaded = 0;
    return ERR_OK;
}

void World_Quit() {
    if (init) {
        free(worldCollision);
        worldCollision = 0;
        SDL_DestroyTexture(foreground.texture);
        Mix_HaltMusic();
        init = 0;
    }
}

int World_Load(char *worldID) {
    int errCode = ERR_OK;
    if (!worldID) { // Fehlerüberprüfung
        SDL_Log("Ungueltige worldID! World_Load()\n");
        return ERR_NULLPARAMETER;
    }

    // Laden der Konfiguration
    worldConfig_t *config;
    if (SDLW_GetResource(worldID, RESOURCETYPE_WORLD, (void **)&config)) {
        SDL_Log("Weltenkonfiguration konnte nicht geladen werden! %s World_Load()\n", worldID);
        return ERR_FAIL;
    }

    // Renderer vom SDLW holen
    SDL_Renderer *renderer = SDLW_GetRenderer();
    if (!renderer) {
        SDL_Log("SDLW nicht richtig initialisiert! World_Load()\n");
        return ERR_FAIL;
    }

    // Vordergrund zum Schreiben vorbereiten
    if (SDL_SetRenderTarget(renderer, foreground.texture)) {
        SDL_Log("SDL_SetRenderTarget Error! [%s]\n", SDL_GetError());
        return ERR_FAIL;
    }

    // Vordergrund Schreiben
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, config->foreground, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    // Kollisionsraster aktualisieren
    UpdateWorld();

    // Hintergrund definieren
    background.texture = config->background;
    int w, h;
    SDL_QueryTexture(background.texture, NULL, NULL, &w, &h);
    background.source.w = w;
    background.source.h = h;

    // Hintergrundmusik abspielen
    if (config->bgMusic[0] != '\0')
        errCode = SDLW_PlayMusic(config->bgMusic);

    // Abschluss
    loaded = 1;
    return errCode;
}

int World_DrawBackground() {
    if (!loaded) { // Fehlerüberprüfung
        SDL_Log("Keine Welt geladen! World_DrawBackground()\n");
        return ERR_FAIL;
    }
    return SDLW_DrawTexture(background); // Zeichnen des Hintergrunds
}

int World_DrawForeground() {
    if (!loaded) { // Fehlerüberprüfung
        SDL_Log("Keine Welt geladen! World_DrawForeground()\n");
        return ERR_FAIL;
    }
    return SDLW_DrawTexture(foreground); // Zeichnen des Vordergrunds
}

int World_CheckCollision(SDL_Rect aabb, struct entityCollision_s *collision) {
    // Fehlerüberprüfung
    if (!loaded) {
        SDL_Log("Keine Welt geladen! World_CheckCollision()\n");
        return ERR_FAIL;
    }

    if (!collision) {
        SDL_Log("Kollisionsbuffer ungueltig! World_CheckCollision()\n");
        return ERR_NULLPARAMETER;
    }

    if (!worldCollision) {
        printf("Weltbuffer ungueltig! World_CheckCollision()\n");
        return ERR_FAIL;
    }

    // Initialisierung der Rückgabe
    collision->flags = 0;

    // Initialisierung des Überprüfungrechtecks
    int x1 = aabb.x;
    int x2 = aabb.x + aabb.w;
    int y1 = aabb.y;
    int y2 = aabb.y + aabb.h;
    SDL_Point hitNormal = {0};


    // Kollision an der Weltenrand und korrigieren des Suchrechtecks
    if (x1 < 0) { // Linker Rand
        x1 = 0;
        collision->flags |= ENTITY_COLLISION_BORDER_LEFT;
    }
    if (x2 > width) { // Rechter Rand
        x2 = width;
        collision->flags |= ENTITY_COLLISION_BORDER_RIGHT;
    }
    if (y1 < 0) { // Oberer Rand
        y1 = 0;
        collision->flags |= ENTITY_COLLISION_BORDER_TOP;
    }
    if (y2 > height) { // Unterer Rand
        y2 = height;
        collision->flags |= ENTITY_COLLISION_BORDER_BOTTOM;
    }
    // Kollision mit der Welt
    for (int x = x1; x < x2; x++) {
        for (int y = y1; y < y2; y++) {
            if (worldCollision[(x + y * width) * 4] > 0) {
                collision->flags |= ENTITY_COLLISION_WORLD;
                if (x - aabb.x - aabb.w / 2 > 0) {
                    hitNormal.x--;
                } else if (x - aabb.x - aabb.w / 2 < aabb.w % 2 - 1) {
                    hitNormal.x++;
                }
                if (y - aabb.y - aabb.h / 2 > 0) {
                    hitNormal.y--;
                } else if (y - aabb.y - aabb.h / 2 < aabb.h % 2 - 1) {
                    hitNormal.y++;
                }
            }
        }
    }

    collision->normal.x = (float)hitNormal.x;
    collision->normal.y = (float)hitNormal.y;
    return ERR_OK;
}

int World_Modify(sprite_t sprite) {
    if (!loaded) {
        SDL_Log("Keine Welt geladen! World_Modify()\n");
        return ERR_FAIL;
    }

    // Renderer vom SDLW holen
    SDL_Renderer *renderer = SDLW_GetRenderer();
    if (!renderer) {
        SDL_Log("SDLW nicht richtig initialisiert! World_Load()\n");
        return ERR_FAIL;
    }

    // Vordergrund zum Schreiben vorbereiten
    if (SDL_SetRenderTarget(renderer, foreground.texture)) {
        SDL_Log("SDL_SetRenderTarget Error! [%s]\n", SDL_GetError());
        return ERR_FAIL;
    }

    // Vordergrund Schreiben
    SDLW_DrawTexture(sprite);
    SDL_SetRenderTarget(renderer, NULL);
    // Welt aktualisieren
    UpdateWorld();

    return ERR_OK;
}

int World_VerticalLineIntersection(SDL_Point searchStart, SDL_Point *hitPoint) {
    // Fehlerüberprüfung
    if (!loaded) {
        SDL_Log("Keine Welt geladen! World_VerticalLineIntersection()\n");
        return ERR_FAIL;
    }

    if (!hitPoint) {
        SDL_Log("Rueckgabespeicher hitPoint ungueltig! World_VerticalLineIntersection()\n");
        return ERR_NULLPARAMETER;
    }

    if (!worldCollision) {
        printf("Weltbuffer ungueltig! World_CheckCollision()\n");
        return ERR_FAIL;
    }

    // Out of bounds check
    if (searchStart.y < 0)
        searchStart.y = 0;
    if (searchStart.x < 0 || searchStart.x >= width) {
        SDL_Log("Suchstart out of bounds! [%d]\n", searchStart.x);
        return ERR_PARAMETER;
    }

    for (int y = searchStart.y; y < height; y++) {
        if (worldCollision[(searchStart.x + y * width) * 4] > 0) {
            hitPoint->x = searchStart.x;
            hitPoint->y = y;
            return ERR_OK;
        }
    }

    hitPoint->x = searchStart.x;
    hitPoint->y = height;
    return ERR_OK;
}

int World_CreateStartLocations(SDL_Rect aabb, int count, SDL_Point *locations) {
    // Fehlerüberprüfung
    if (!loaded) {
        SDL_Log("Keine Welt geladen! World_VerticalLineIntersection()\n");
        return ERR_FAIL;
    }

    if (!locations) {
        SDL_Log("Rueckgabespeicher locations ungueltig! World_VerticalLineIntersection()\n");
        return ERR_NULLPARAMETER;
    }

    // Nächster solider freier Raum suchen, in gleichmässig verteilten Abständen.
    int distance = width / (count + 1);
    for (int c = 1; c <= count; c++) {
        SDL_Point hit = {0}; // Bei error VerticalLineIntersection hit = 0,0
        entityCollision_t collision;
        World_VerticalLineIntersection((SDL_Point){c * distance, 0}, &hit);
        do { // AABB nach oben verschieben, bis der Raum total frei ist.
            hit.y--;
            if (hit.y < 0)
                break;
            collision.flags = 0; // Bei error CheckCollision collision.hasCollision = 0
            World_CheckCollision((SDL_Rect){hit.x + aabb.x, hit.y + aabb.y, aabb.w, aabb.h}, &collision);
        } while (collision.flags & ENTITY_COLLISION_WORLD);
        locations[c - 1] = hit;
    }

    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int UpdateWorld() {
    SDL_Renderer *renderer = SDLW_GetRenderer(); // Holt den Renderer von SDLW
    if (!renderer) {
        SDL_Log("SDLW nicht richtig initialisiert! World_Load()\n");
        return ERR_FAIL;
    }

    if (SDL_SetRenderTarget(renderer, foreground.texture)) { // Bereitet Vordergrund zum Lesen vor
        SDL_Log("SDL_SetRenderTarget Error! [%s]\n", SDL_GetError());
        return ERR_FAIL;
    }

    // Liest die Daten vom Vordergrund
    SDL_Rect r = (SDL_Rect){0, 0, width, height};

    SDL_RenderReadPixels(renderer, &r, SDL_PIXELFORMAT_RGBA8888, worldCollision, width * 4);
    SDL_SetRenderTarget(renderer, NULL);

    return ERR_OK;
}
