/**
 * @file scene.c
 * @author Weber Jan (webej14@bfh.ch)
 * @brief Aktualisiert Module und übergibt InputEvents
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Weber Jan
 * 
 */


/*
 * Includes
 * 
 */

#include "scene.h"
#include "entityHandler.h"


/*
 * Typdeklarationen
 * 
 */

/* ... */


/*
 * Variablendeklarationen
 * 
 */

int gameloop;           //!< Globale Variable, 1=Programm läuft, 0=Programm wird beendet
SceneID currentSceneID; //!< Globale Variable, enthält die aktuell aktive Szene ID


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief konvertiert SDL InputEvents Union ins Struct inputEvent_t
 * 
 * @param[in] inputEvent Benutzereingabe
 * @param[out] convertedInputEvent Speicherort für konvertierte Benutzereingabe
 */
static void convertInputEvent(SDL_Event *inputEvent, inputEvent_t *convertedInputEvent);

/**
 * @brief Identifiziert die eingegebene Taste und übersetzt ins Raster
 * 
 * Die Vorgaben für die Werte der entsprechenden Tasten
 * kommen vom struct inputEvent_t im scene.h.
 * Die Werte werde durch den Pointer übergeben.
 * 
 * @param[in] inputEvent Benutzereingabe
 * @param[out] convertedInputEvent konvertierte Benutzereingabe 
 */
static void identifieKey(SDL_Event *inputEvent, inputEvent_t *convertedInputEvent);

/**
 * @brief Identifiziert eingegebene ASCII Zeichen
 * 
 * @param[in] inputEvent Benutzereingabe
 * @param[out] convertedInputEvent konvertierte Benutzereingabe 
 */
static void identifieChar(SDL_Event *inputEvent, inputEvent_t *convertedInputEvent);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */
/****************************************************************************/
int Scene_Init(player_t playerA, player_t playerB, inputEvent_t input,
               gui_t *sceneStartUp, sceneNamePlayer_t *sceneNamePlayer,
               sceneMainMenu_t *sceneMainMenu, gui_t *sceneControls,
               sceneInGame_t *sceneInGame, sceneChooseWorld_t *sceneChooseWorld,
               gui_t *sceneVictory, gui_t *sceneErrFail) {
    if (input.currentPlayer == NULL) {
        return ERR_PARAMETER;
    }
    if (ERR_OK != sceneStartup_Init(sceneStartUp) ||
        ERR_OK != sceneMainMenu_Init(sceneMainMenu) ||
        ERR_OK != sceneNamePlayer_Init(sceneNamePlayer) ||
        ERR_OK != sceneControls_Init(sceneControls) ||
        ERR_OK != sceneChooseWorld_Init(sceneChooseWorld) ||
        ERR_OK != sceneInGame_Init(sceneInGame, playerA, playerB, input) ||
        ERR_OK != sceneVictory_Init(sceneVictory) ||
        ERR_OK != sceneErrFail_Init(sceneErrFail)) {
        return ERR_FAIL;
    }
    return ERR_OK;
}
/****************************************************************************/
int Scene_Update(SDL_Event *event, inputEvent_t *inputEvent, gui_t *scene) {
	// Erstelle neuen leeren InputEvent, behalte aber den aktuellen Spieler
	inputEvent_t newInputEvent = {.currentPlayer = inputEvent->currentPlayer};
	*inputEvent = newInputEvent;
	// Frage alle Events von SDL ab und behandle QUIT, KEYDOWN und TEXTINPUT
    while (SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) {
            gameloop = 0;
        }
        convertInputEvent(event, inputEvent);
    }
    inputEvent->mouseButtons = SDL_GetMouseState(&inputEvent->mousePosition.x, &inputEvent->mousePosition.y);
	// Gebe die Events den einzelnen Modulen weiter
    EntityHandler_Update(inputEvent);
    SDLW_Clear(COLORBACKGROUND);
    if (currentSceneID == SCENE_INGAME) {
        Scene_DrawGame(scene);
    } else {
        Scene_Draw(scene);
    }
    GUI_Update(inputEvent, scene);
    SDLW_Render();
    return ERR_OK;
}
/****************************************************************************/
int Scene_DrawGame(gui_t *scene) {
    if (World_DrawBackground() != ERR_OK ||
        GUI_Draw(scene) != ERR_OK ||
        World_DrawForeground() != ERR_OK ||
        EntityHandler_Draw() != ERR_OK) {
        return ERR_FAIL;
    }
    return ERR_OK;
}
/****************************************************************************/
int Scene_Draw(gui_t *scene) {
    if (GUI_Draw(scene) != ERR_OK) {
        return ERR_FAIL;
    }
    return ERR_OK;
}
/****************************************************************************/
guiElement_t Scene_ButtonPrototyp() {
    return (guiElement_t){
		.type = 0,
		.elementData.button = (button_t){
			.id = 0,
			.buttonColor = (SDL_Color){255, 255, 255, 255},
			.highlightColor = (SDL_Color){255, 165, 0, 255},
			.textColor = (SDL_Color){0, 0, 0, 255},
			.buttonSize = (SDL_Rect){50, 50, 200, 100},
			.onClickCallback = NULL, .borderWidth = 2
		}
	};
}
/****************************************************************************/
guiElement_t Scene_TextInputPrototyp() {
    return (guiElement_t){
		.type = TYPE_TEXT,
		.elementData.textInput = (text_t){
			.textBgc = (SDL_Color){255, 255, 255, 255},
			.highlightColor = (SDL_Color){255, 165, 0, 255},
			.textColor = (SDL_Color){0, 0, 0, 255},
			.textRectSize = (SDL_Rect){50, 300, 200, 100},
			.borderWidth = 2,
			.text = {'E', 'n', 't', 'r', 'y', '\0'},
			.font = "osans25"
		}
	};
}
/****************************************************************************/
/*
 * Implementation Privater Funktionen
 * 
 */
/****************************************************************************/
static void convertInputEvent(SDL_Event *inputEvent, inputEvent_t *convertedInputEvent) {
    switch (inputEvent->type) {
    case SDL_TEXTINPUT:
        identifieChar(inputEvent, convertedInputEvent);
        break;
    case SDL_KEYDOWN:
        convertedInputEvent->lastKey = inputEvent->key.keysym.sym;
        identifieKey(inputEvent, convertedInputEvent);
        break;
    }
}
/****************************************************************************/
static void identifieKey(SDL_Event *inputEvent, inputEvent_t *convertedInputEvent) {
    if (inputEvent->key.keysym.sym == SDLK_w) {
        convertedInputEvent->axisWASD.y = -1;
    } else if (inputEvent->key.keysym.sym == SDLK_a) {
        convertedInputEvent->axisWASD.x = -1;
    } else if (inputEvent->key.keysym.sym == SDLK_s) {
        convertedInputEvent->axisWASD.y = 1;
    } else if (inputEvent->key.keysym.sym == SDLK_d) {
        convertedInputEvent->axisWASD.x = 1;
    } else if (inputEvent->key.keysym.sym == SDLK_UP) {
        convertedInputEvent->axisArrow.y = -1;
    } else if (inputEvent->key.keysym.sym == SDLK_LEFT) {
        convertedInputEvent->axisArrow.x = -1;
    } else if (inputEvent->key.keysym.sym == SDLK_DOWN) {
        convertedInputEvent->axisArrow.y = 1;
    } else if (inputEvent->key.keysym.sym == SDLK_RIGHT) {
        convertedInputEvent->axisArrow.x = 1;
    } else if (inputEvent->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
        convertedInputEvent->currentChar = '\b';
    }
}
/****************************************************************************/
static void identifieChar(SDL_Event *inputEvent, inputEvent_t *convertedInputEvent) {
    if (inputEvent->text.text[1] == '\0') {
        convertedInputEvent->currentChar = inputEvent->text.text[0];
    }
}