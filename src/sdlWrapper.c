/**
 * @file sdlWrapper.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Wrapper der SDL Bibliothek
 * @version 0.3
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
#include <stdlib.h>
#include <string.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include "list.h"
#include "sdlWrapper.h"
#include "error.h"
#include "sprite.h"
#include "world.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

static int initialized = 0;           //!< Ist der SDLWrapper initialisiert
static SDL_Window *window = NULL;     //!< Das geöffnete Fenster
static SDL_Renderer *renderer = NULL; //!< Der Benutzte Renderer

static list_t resourceList; //!< Liste aller geladenen Ressourcen


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief Vergleiche Ressourcenschlüssel.
 * Vergleicht ob das gegebene Element gesucht wird.
 * \p searchKey muss Null terminated sein!
 *
 * @param[in] resource Die zu überprüfende Ressource
 * @param[in] searchKey Nach welchem Ressourcenschlüssel gesucht wird
 *
 * @return 0 = false, 1 = true
 */
static int SDLW_CmpKey(sdlwResource_t *resource, char *searchKey);

/**
 * @brief Lädt die angegebene Datei in die Ressource als eine Textur
 *
 * @param[in] config Zeile aus Konfigurationsdatei mit Argumenten
 * @param[out] resource Wohin die Ressource geladen werden soll
 *
 * @return 0 oder Errorcode
 */
static int SDLW_LoadTexture(char *config, sdlwResource_t *resource);

/**
 * @brief Lädt die angegebene Datei in die Ressource als eine Schrift
 *
 * @param[in] config Zeile aus Konfigurationsdatei mit Argumenten
 * @param[out] resource Wohin die Ressource geladen werden soll
 *
 * @return 0 oder Errorcode
 */
static int SDLW_LoadFont(char *config, sdlwResource_t *resource);

/**
 * @brief Lädt die angegebene Datei in die Resource als einen Sound
 *
 * @param[in] config Zeile aus Konfigurationsdatei mit Argumenten
 * @param[out] resource Wohin die Ressource geladen werden soll
 *
 * @return 0 oder Errorcode
 */
static int SDLW_LoadSound(char *config, sdlwResource_t *resource);

/**
 * @brief Befreit allozierte Ressource.
 * Wird von SDLW_Quit() aufgerufen.
 * 
 * @param[in] resource Eine Ressource von \p resourceList
 *
 * @return 0 oder Errorcode
 */
static int FreeSDLWResource(sdlwResource_t *resource);


/*
 * Implementation öffentlicher Funktionen
 *
 */

int SDLW_Init(int windowWidth, int windowHeight) {
    if (initialized) {
        printf("SDLW ist schon Initialisiert! SDLW_Init()\n");
        return ERR_FAIL;
    }

    if (windowWidth < 1 || windowWidth > 2000 || windowHeight < 1 || windowHeight > 2000) { // Boundcheck der Grössenangabe
        printf("Fenstergroesse ausserhalb Schranken! width:%d, height:%d SDLW_Init()\n", windowWidth, windowHeight);
        return ERR_PARAMETER;
    }

    // Initialisierung der verschiedenen SDL Bibliotheken
    if (SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_SENSOR)) { // ohne Sensor-System
        SDLW_Quit();
        printf("SDL Error beim initialisieren! [%s] SDLW_Init()\n", SDL_GetError());
        return ERR_FAIL;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        printf("Mix Error beim Audio oeffnen! [%s] SDLW_Init()\n", Mix_GetError());
        SDLW_Quit();
        return ERR_FAIL;
    }

    if (!Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_OPUS)) { // Initialisierung Sound
        SDLW_Quit();
        printf("Mix Error beim initialisieren! [%s] SDLW_Init()\n", Mix_GetError());
        return ERR_FAIL;
    }

    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) { // Initialisierung IMG
        SDLW_Quit();
        printf("SDL_image Error beim initialisieren! [%s] SDLW_Init()\n", IMG_GetError());
        return ERR_FAIL;
    }

    if (TTF_Init()) { // Initialisierung Fonts
        SDLW_Quit();
        printf("TTF Error beim initialisieren! [%s] SDLW_Init()\n", TTF_GetError());
        return ERR_FAIL;
    }

    if (SDLNet_Init()) { // Initialisierung Networking
        SDLW_Quit();
        printf("SDLNet Error beim initialisieren! [%s] SDLW_Init()\n", SDLNet_GetError());
        return ERR_FAIL;
    }

    // Erstellen eines Fensters
    window = SDL_CreateWindow("Tanks", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        SDLW_Quit();
        printf("SDL_CreateWindow Error! [%s] SDLW_Init()\n", SDL_GetError());
        return ERR_FAIL;
    }

    // Erstellen des Graphikrenderers
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDLW_Quit();
        printf("SDL_CreateRenderer Error! [%s] SDLW_Init()\n", SDL_GetError());
        return ERR_FAIL;
    }

    // Initialisierung der Ressourcenliste
    if (List_Init(&resourceList)) {
        SDLW_Quit();
        printf("Ressourcenliste konnte nicht initialisiert werden! SDLW_Init()\n");
        return ERR_FAIL;
    }
    resourceList.dataAutoFree = (fnPntrDataCallback)FreeSDLWResource;

    // Abschluss der Initialisierung
    initialized = 1;
    return ERR_OK;
}

int SDLW_Quit() {
    int errorCode = ERR_OK;

    // Löschen der geladenen Ressourcen
    if (initialized)
        errorCode = List_Clear(&resourceList);

    // Schliessen des Fensters
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    // Ausstehende Events konsumieren
    SDL_Event e;
    while(SDL_PollEvent(&e)) {};

    // Schliessen der Bibliotheken
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDLNet_Quit();

    initialized = 0;

    return errorCode;
}

int SDLW_LoadResources(char *resourceConfigLocation) {
    // Fehlerüberprüfung
    if (!initialized) {
        printf("SLDW nicht initialisiert! SDLW_LoadRessource()\n");
        return ERR_FAIL;
    }

    if (!resourceConfigLocation) {
        printf("Dateipfad ist ungueltig! SDLW_LoadRessource()\n");
        return ERR_NULLPARAMETER;
    }

    // Öffnen der Konfigurationsdatei
    FILE *file;
    file = fopen(resourceConfigLocation, "r");

    if (!file) { // Fehlerüberprüfungs
        printf("Fehler beim öffnen des Dokuments %s! SDLW_LoadRessource()\n", resourceConfigLocation);
        return ERR_PARAMETER;
    }

    char config[256] = {0};
    char key[64] = {0};  // Die ID der zu ladenen Ressource
    char type[32] = {0}; // Der Typ der zu ladenen Ressource

    int line = 0;

    while (fgets(config, 255, file)) { // Lesen der Konfigurationszeilen
        line++;

        int count = sscanf(config, "%63s %31s", key, type);
        if (count == 0)
            continue;
        if (!strcmp(key, "#")) // Kommentar
            continue;

        if(count < 2){ // Zu wenig Argumente in der Konfigurationszeile
            printf("Konfigurationszeile %d ungueltig! [%s]\n", line, resourceConfigLocation);
            fclose(file);
            return ERR_FAIL;
        }

        sdlwResource_t *resource = malloc(sizeof(sdlwResource_t));
        if (!resource) { // Allozierung überprüfen
            printf("Ressource konnte nicht alloziert werden! SDLW_LoadRessource()!\n");
            fclose(file);
            return ERR_MEMORY;
        }

        strcpy(resource->key, key); // Setzen des Schlüssels der Ressource

        // Lesen und überprüfen der verschiedenen Ressourcentypen
        int errorCode = ERR_FAIL;
        if (!strcmp(type, "texture")) // Textur
            errorCode = SDLW_LoadTexture(config, resource);
        else if (!strcmp(type, "font")) // Font
            errorCode = SDLW_LoadFont(config, resource);
        else if (!strcmp(type, "sound"))
            errorCode = SDLW_LoadSound(config, resource);
        else if (!strcmp(type, "sprite"))
            errorCode = Sprite_Load(config, resource);
        else if (!strcmp(type, "world"))
            errorCode = World_LoadConfig(config, resource);
        else if (!strcmp(type, "config")) { // Weitere Konfigurationsdatei
            free(resource);
            errorCode = SDLW_LoadResources(key);
            if (errorCode) {
                fclose(file);
                return errorCode;
            }
            continue;
        }

        if (errorCode) { // Fehlerüberprüfung
            free(resource);
            fclose(file);
            return errorCode;
        }

        // Hinzufügen der geladenen Ressource
        printf("Geladen %s als %s\n", key, type);
        List_Add(&resourceList, resource);
    }

    // Abschluss
    fclose(file);
    return ERR_OK;
}

int SDLW_GetResource(char *id, resourceType_t type, void **resource) {
    if (!resource) {
        printf("Zielpointer ungueltig!\n");
        return ERR_NULLPARAMETER;
    }
    // Garantieren dass bei allen anderen Fehlern der Rückgabewert NULL ist
    (*resource) = NULL;

    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_GetTexture()\n");
        return ERR_FAIL;
    }

    if (!id) {
        printf("Id ungueltig!\n");
        return ERR_NULLPARAMETER;
    }

    sdlwResource_t *foundResource; // Suche initialisieren

    // Ressource nach id = key suchen
    List_SearchArg(&resourceList, (fnPntrDataCallbackArg)SDLW_CmpKey, id, (void **)&foundResource);
    if (foundResource && (foundResource->type & type)) {
        (*resource) = foundResource->resource.any;
        return ERR_OK;
    }
    printf("Ressource mit id [%s] nicht gefunden!\n", id);
    return ERR_FAIL;
}

int SDLW_DrawTexture(sprite_t sprite) {
    // Fehlerüberprüfung
    if (!initialized) {
        printf("SLDW nicht initialisiert! SDLW_DrawTexture()\n");
        return ERR_FAIL;
    }
    if (!sprite.texture) {
        printf("Keine Textur definiert! SDLW_DrawTexture()\n");
        return ERR_NULLPARAMETER;
    }

    SDL_Rect destinationRect = sprite.destination; // Setzen der Destination relativ auf das Zentrum der Textur
    destinationRect.x += (int)(sprite.position.x - sprite.destination.w / 2);
    destinationRect.y += (int)(sprite.position.y - sprite.destination.h / 2);

    SDL_Point pivot; // Setzen des Pivots relativ auf das Zentrum der Textur
    pivot.x = (int)(sprite.pivot.x + sprite.destination.w / 2);
    pivot.y = (int)(sprite.pivot.y + sprite.destination.h / 2);

    // Zeichnen der Textur
    SDL_RenderCopyEx(renderer, sprite.texture, &sprite.source, &destinationRect, sprite.rotation, &pivot, SDL_FLIP_NONE);
    return ERR_OK;
}

int SDLW_DrawFilledRect(SDL_Rect rect, SDL_Color color) {
    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_DrawFilledRect()\n");
        return ERR_FAIL;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    return ERR_OK;
}

int SDLW_CreateTextTexture(char *text, char *font, SDL_Color color, SDL_Texture **texture) {
    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_CreateTextTexture()\n");
        return ERR_FAIL;
    }
    if (!text) { // Fehlerüberprüfung
        printf("Text ungueltig! SDLW_CreateTextTexture()\n");
        return ERR_NULLPARAMETER;
    }
    if (!font) { // Fehlerüberprüfung
        printf("font ungueltig! SDLW_CreateTextTexture()\n");
        return ERR_NULLPARAMETER;
    }
    if (!texture) { // Fehlerüberprüfung
        printf("Texturespeicherort ungluetig! SDLW_CreateTextTexture()\n");
        return ERR_NULLPARAMETER;
    }

    TTF_Font *fontResource;
    SDLW_GetResource(font, RESOURCETYPE_FONT, (void **)&fontResource); // Suche nach der Font

    if (!fontResource) { // Fehlerüberprüfung
        return ERR_PARAMETER;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(fontResource, text, color); // Erstellen der Graphik
    if (!surface) {                                                         // Fehlerüberprüfung
        return ERR_MEMORY;
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, surface); // Erstellen der Textur
    if (!textTexture) {                                                         // Fehlerüberprüfung
        SDL_FreeSurface(surface);
        return ERR_MEMORY;
    }

    // Abschluss
    SDL_FreeSurface(surface);

    (*texture) = textTexture;
    return 0;
}

int SDLW_Clear(SDL_Color color) {
    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_Clear()\n");
        return ERR_FAIL;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    return ERR_OK;
}

int SDLW_Render() {
    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_Render()\n");
        return ERR_FAIL;
    }
    SDL_RenderPresent(renderer);
    return ERR_OK;
}

int SDLW_PlayMusic(char *musicid) {
    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_PlayMusic()\n");
        return ERR_FAIL;
    }
    if (!musicid) { // Fehlerüberprüfung
        printf("Musikid ungueltig! SDLW_PlayMusic()\n");
        return ERR_NULLPARAMETER;
    }

    Mix_Music *music;
    SDLW_GetResource(musicid, RESOURCETYPE_SOUND_MUSIC, (void **)&music);

    if (!music) { // Fehlerüberprüfung
        printf("Musik konnte nicht geladen werden!\n");
        return ERR_FAIL;
    }

    Mix_PlayMusic(music, -1); // Spielt Musik in Endlosschleife, bis neue Musik abgespielt wird.

    return ERR_OK;
}

int SDLW_PlaySoundEffect(char *chunk) {
    if (!initialized) { // Fehlerüberprüfung
        printf("SLDW nicht initialisiert! SDLW_PlayMusic()\n");
        return ERR_FAIL;
    }
    if (!chunk) { // Fehlerüberprüfung
        printf("Musikid ungueltig! SDLW_PlayMusic()\n");
        return ERR_NULLPARAMETER;
    }

    Mix_Chunk *sound;
    SDLW_GetResource(chunk, RESOURCETYPE_SOUND_EFFECT, (void **)&sound);

    if (!sound) { // Fehlerüberprüfung
        printf("Soundeffekt konnte nicht geladen werden!\n");
        return ERR_FAIL;
    }

    Mix_PlayChannel(-1, sound, 0); // Spielt den Soundeffekt einmal ab

    return ERR_OK;
}

SDL_Renderer* SDLW_GetRenderer() {
    if (!initialized)
        return NULL;
    return renderer;
}


/*
 * Implementation privater Funktionen
 *
 */

static int SDLW_CmpKey(sdlwResource_t *resource, char *searchKey) {
    if (!searchKey)
        return 0;
    if (!strcmp(resource->key, searchKey))
        return 1;
    return 0;
}

static int SDLW_LoadTexture(char *config, sdlwResource_t *resource) {
    char key[32] = {0};
    char type[32] = {0};
    char fileName[64] = {0};
    char mode[16] = {0};
    char modeType[8] = {0};

    // Konfiguration lesen
    int count = sscanf(config, "%31s %31s %63s %15s %7s", key, type, fileName, mode, modeType);
    if (count < 3) {
        printf("Pfad nicht angegeben %s! static SDLW_LoadTexture()\n", key);
        return ERR_FAIL;
    }

    resource->type = RESOURCETYPE_TEXTURE_N;
    resource->resource.texture = IMG_LoadTexture(renderer, fileName);
    if (!resource->resource.texture) { // Fehlerüberprüfung
        printf("IMG_LoadTexture error! [%s]\n", IMG_GetError());
        return ERR_FAIL;
    }

    if (count == 4) { // Entweder 3 oder 5 Argumente, 4 sind nicht zlässig
        return ERR_FAIL;
    }

    if (count == 5) {                    // Textur als stamp oder mask laden
        if (strcmp(mode, "blendmode")) { // Konfigurationsfehlerüberprüfung
            printf("Rendermodus für %s ungültig! [%s]\n", key, mode);
            SDL_DestroyTexture(resource->resource.texture);
            return ERR_FAIL;
        }
        if (strcmp(modeType, "stamp") && strcmp(modeType, "mask")) { // Konfigurationsfehlerüberprüfung
            printf("Rendermodustyp für %s ungültig! [%s]\n", key, modeType);
            SDL_DestroyTexture(resource->resource.texture);
            return ERR_FAIL;
        }
        SDL_BlendMode blendMode;
        if (!strcmp(modeType, "stamp")) {
            resource->type = RESOURCETYPE_TEXTURE_BM_STAMP;
            // Zeichnet Textur dort, wo dst_alpha < 1 ist. Alpha wird zusammengezählt
            blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDOPERATION_ADD,
                                                   SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
        } else {
            resource->type = RESOURCETYPE_TEXTURE_BM_MASK;
            // Schneidet dort aus, wo src_alpha > 0 ist. Farbe bleibt unverändert.
            blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
                                                   SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
        }
        SDL_SetTextureBlendMode(resource->resource.texture, blendMode);
    }

    return ERR_OK;
}

static int SDLW_LoadFont(char *config, sdlwResource_t *resource) {
    char key[32] = {0};
    char type[32] = {0};
    char fileName[64] = {0};
    int size = 0;

    // Konfiguration lesen
    int count = sscanf(config, "%31s %31s %63s %d", key, type, fileName, &size);

    if (count != 4) { // Konfigurationsfehlerüberprüfung
        printf("Konfigurationsargumente für %s ungültig!\n", key);
        return ERR_FAIL;
    }

    if (size < 5 || size > 200) { // Bounds für Schriftgrösse überprüfen
        printf("Schriftgrösse für %s zu gross oder zu klein! [%d]\n", key, size);
        return ERR_FAIL;
    }

    resource->type = RESOURCETYPE_FONT;
    resource->resource.font = TTF_OpenFont(fileName, size);
    if (resource->resource.font)
        return ERR_OK;
    printf("TTF_OpenFont error! [%s]\n", TTF_GetError());
    return ERR_FAIL;
}

static int SDLW_LoadSound(char *config, sdlwResource_t *resource) {
    char key[32] = {0};
    char type[32] = {0};
    char fileName[64] = {0};
    char soundType[8] = {0};
    // Konfiguration lesen
    int count = sscanf(config, "%31s %31s %63s %7s", key, type, fileName, soundType);

    if (count != 4) { // Konfigurationsfehlerüberprüfung
        printf("Konfigurationsargumente für %s ungültig!\n", key);
        return ERR_FAIL;
    }

    if (strcmp(soundType, "effect") && strcmp(soundType, "music")) { // Bounds für Schriftgrösse überprüfen
        printf("Typ für %s ungueltig! [%s]\n", key, soundType);
        return ERR_FAIL;
    }

    int isMusic = strcmp(soundType, "music") == 0;

    if (isMusic) { // Laden von Hintergrundmusik
        resource->type = RESOURCETYPE_SOUND_MUSIC;
        resource->resource.bgMusic = Mix_LoadMUS(fileName);
        if (resource->resource.bgMusic)
            return ERR_OK;
        else
            printf("[%s] konnte nicht geladen werden!\n", key);
    } else { // Laden von Soundeffekten
        resource->type = RESOURCETYPE_SOUND_EFFECT;
        resource->resource.soundEffect = Mix_LoadWAV(fileName);
        if (resource->resource.soundEffect)
            return ERR_OK;
        else
            printf("[%s] konnte nicht geladen werden!\n", key);
    }

    printf("Mix_Load error! [%s]\n", Mix_GetError());
    return ERR_FAIL;
}

static int FreeSDLWResource(sdlwResource_t *resource) {
    if (resource->type & RESOURCETYPE_TEXTURE) { // Zerstörte Textur
        SDL_DestroyTexture(resource->resource.texture);
    } else if (resource->type & RESOURCETYPE_FONT) { // Zerstöre Schriftwart
        TTF_CloseFont(resource->resource.font);
    } else if (resource->type & RESOURCETYPE_SOUND_MUSIC) { // Zerstöre Sound
        Mix_FreeMusic(resource->resource.bgMusic);
    } else if (resource->type & RESOURCETYPE_SOUND_EFFECT) { // Zerstöre Soundeffekt
        Mix_FreeChunk(resource->resource.soundEffect);
    } else if (resource->type & RESOURCETYPE_SPRITE) { // Zerstöre Sprite
        free(resource->resource.sprite);
    } else if (resource->type & RESOURCETYPE_WORLD) { // Zerstöre Welt
        free(resource->resource.world);
    }
    free(resource); // Ressourcenunion zerstören
    return ERR_OK;
}
