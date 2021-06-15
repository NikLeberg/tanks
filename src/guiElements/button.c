/**
 * @file button.c
 * @author Hinnaoui Angelo (hinna2@bfh.ch)
 * @brief Buttons als GUI Element
 * @version 0.01
 * @date 2021-03-30
 *
 * @copyright Copyright (c) 2021 Hinnaoui Angelo
 *
 */


/*
 * Includes
 *
 */

#include "guiElements/button.h"
#include "sdlWrapper.h"
#include "scene.h"
#include "error.h"

#include <stdio.h>


/*
 * Variablendeklarationen
 *
 */

static char *clickEffect = "click";     //!< ID des click Soundeffekt
static char *selectEffect = "select";   //!< ID des select Soundeffekt
static char *releaseEffect = "release"; //!< ID des release Soundeffekt


/*
 * Private Funktionsprototypen
 * 
 */

/* ... */


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Button_Init(button_t *button, char *font, char *text) {
    if (!button || !font || !text) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }

    if (Sprite_CreateText(text, font, button->textColor, &button->textTextur) != ERR_OK) { // Die erhaltenen Parameter werden an die Struktur des zu erstellenden Taster uebergeben.

        printf("Sprite ungueltig! Sprite_CreateText()\n");

        return ERR_FAIL;
    }

    button->textTextur.position.x = button->buttonSize.x + button->buttonSize.w / 2; // Der Tastenmittelpunkt, wird zu den Koordinaten für den Text bestimmt.
    button->textTextur.position.y = button->buttonSize.y + button->buttonSize.h / 2;

    return ERR_OK;
}

int Button_Update(inputEvent_t *inputEvents, button_t *button) {
    if (!inputEvents || !button) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    if (SDL_PointInRect(&inputEvents->mousePosition, &button->buttonSize)) { // Ist der Mauszeiger im Bereich des Tastenfeld,
        if (button->state == 0) {                                            // ist die Maustaste nicht gedrueckt,
            SDLW_PlaySoundEffect(selectEffect);                              // wird der dazugehoerige Sound Effekt abgespielt (select)
            button->state = 1;                                               // der Tastenzustand wird auf 1 gesetzt. (Auswahlrahmen)
        }
        if (inputEvents->mouseButtons > 0) {       // Wird die gewaehlte Maustaste gedrueckt,
            if (button->state == 1) {              // ist der Tastenzustand gesetzt bzw. die Maus im Tastenbereich,
                SDLW_PlaySoundEffect(clickEffect); // wird der dazugehoerige Sound Effekt abgespielt. (click)
            }
            button->state = 2; // Tastenzustand wird auf 2 gesetzt (Taste ist gedrueckt)

        } else {
            if (button->state == 2) {                    // Ist die Maustaste nicht gedrueckt aber der Tastenzustand auf 2,
                SDLW_PlaySoundEffect(releaseEffect);     // wird der dazugehoerige Sound Effekt abgespielt (release)
                if (button->onClickCallback != NULL) {   // ueberprueft ob die Funktion onClickCallback exisitiert
                    button->onClickCallback(button->id); // wen ja wird die Funktion aufgerufen.
                }
            }
            button->state = 1; // Ist die Maus nach dem Click noch immer im Tastenbereich, wird der Zustand 1 (select),
        }
    } else {
        button->state = 0; // ansonsten wird der Tasterzustand zurueckgesetzt.
    }
    return ERR_OK;
}

int Button_Draw(button_t *button) {
    if (!button) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    switch (button->state) { // je nach Tastenzustand,

    case 0:
        SDLW_DrawFilledRect(button->buttonSize, button->buttonColor); // ist die Taste nicht gedrueckt wird die Taste mit der vorgegebenen groesse und Farbe gezeichnet.
        SDLW_DrawTexture(button->textTextur);
        break;
    case 1:

    case 2:
        SDLW_DrawFilledRect(button->buttonSize, button->highlightColor); // ist die Taste gedrueckt wird zusätzlich, anhand der Tastengroesse, ein Auswahlrahmen darüber gezeichnet.s

        SDLW_DrawFilledRect((SDL_Rect){
                                button->buttonSize.x + button->borderWidth,
                                button->buttonSize.y + button->borderWidth,
                                button->buttonSize.w - 2 * button->borderWidth,
                                button->buttonSize.h - 2 * button->borderWidth},
                            button->buttonColor);
        SDLW_DrawTexture(button->textTextur);
        break;
    }


    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
