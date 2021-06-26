/**
 * @file text.c
 * @author Hinnaoui Angelo (hinna2@bfh.ch)
 * @brief textInputs als GUI Element
 * @version 0.01
 * @date 2021-03-24
 *
 * @copyright Copyright (c) 2021 Hinnaoui Angelo
 *
 */


/*
 * Includes
 *
 */

#include "guiElements/text.h"
#include "sdlWrapper.h"
#include "scene.h"
#include "SDL_rect.h"
#include "error.h"

#include <stdio.h>


/*
 * Typdeklarationen
 *
 */

/* ... */


/*
 * Variablendeklarationen
 *
 */

static char *textSelectEffect = "textSelect"; //!< ID des textSelect Soundeffekt


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief UpdateText
 *
 * Aktualisiert die Text Textur, nach einer Änderung.
 *
 * @param[in] textInput Liste der Texteigenschaften.
 * @return ERR_OK oder ERR_NULLPARAMETER
 */

static int UpdateText(text_t *textInput);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int Text_Init(text_t *textInput) {
    if (!textInput) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }

    if (Sprite_CreateText(textInput->text, textInput->font, textInput->textColor, &textInput->textTextur) != ERR_OK) { // Es wird ein Text mit den gegebenen Eigenschaften erstellt.

        return ERR_FAIL;
    }

    int w, h; // Parameter für die Breite und Höhe

    if (SDL_QueryTexture(textInput->textTextur.texture, NULL, NULL, &w, &h) != 0) { // Abfrage der Attribute einer Textur

        return ERR_FAIL;
    }

    textInput->textTextur.position.x = textInput->textRectSize.x + w / 2 + 10; // Richtet die Textposition aus, damit der Text linksbündig dargestellt wird.
    textInput->textTextur.position.y = textInput->textRectSize.y + textInput->textRectSize.h / 2;

    return ERR_OK;
}

int Text_Update(inputEvent_t *inputEvents, text_t *textInput) {
    if (!inputEvents || !textInput) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    if (inputEvents->mouseButtons > 0) {                                              // Wird eine Eingabe erkannt,
        if (SDL_PointInRect(&inputEvents->mousePosition, &textInput->textRectSize)) { // Überprüf, ob sich der Mauszeiger innerhalb eines Rechtecks ​​befindet.
            if (textInput->state == 0) {                                              // Ist das Textfeld noch nicht ausgewählt,
                SDLW_PlaySoundEffect(textSelectEffect);                               // wird der dazugehoerige Sound Effekt abgespielt (textSelect)
            }
            textInput->state = 1; // das Textfeld wird aktiviert. (Auswahlrahmen)

        } else {

            textInput->state = 0; // Erfolgte die Mauseingabe ausserhalb des Textfeld, wird dass Feld deaktiviert.
        }
    }
    if (textInput->state > 0) { // ist das Textfeld aktiv, wird auf die zuletzt registrierte Tastatureingabe überprüft.

        if (inputEvents->lastKey == 8) { // Ist die Letzte Eingabe ein Backspace, (Wert 8 in ASCII)
            if (textInput->index > 0) {   // wird der zuletzt erhaltene Eintrag gelöscht bzw. auf NULL gesetzt.
                textInput->text[textInput->index - 1] = '\0';
                textInput->index--;
            }
            UpdateText(textInput);                                         // Aktualisiert die Textausgabe.
        } else if (inputEvents->lastKey != '\0') {                        // Wen eine Tastatureingabe registriert wird,
            if (textInput->index < 31) {                                   // und die maximale Texteingabe noch nicht erreicht wurde,
                textInput->text[textInput->index] = inputEvents->lastKey; // wird die Eingabe dem entsprechenden Text Array hinzugefügt.
                textInput->index++;
                textInput->text[textInput->index] = '\0'; // Der darauf folgende Eintrag, im Text Array, wird auf NULL gesetzt.
            }
            UpdateText(textInput); // Aktualisiert die Textausgabe.
        }
    }
    return ERR_OK;
}

int Text_Draw(text_t *textInput) {
    if (!textInput) { // Fehlerüberprüfung
        return ERR_NULLPARAMETER;
    }

    switch (textInput->state) {

    case 0:                                                               // Ist das Textfeld deaktiviert,
        SDLW_DrawFilledRect(textInput->textRectSize, textInput->textBgc); // wird die Eingabefläche und
        SDLW_DrawTexture(textInput->textTextur);                          // den darin enthaltenden Text gezeichnet.
        break;
    case 1:                                                                      // Ist das Textfeld aktiviert,
        SDLW_DrawFilledRect(textInput->textRectSize, textInput->highlightColor); // wird die Eingabefläche,

        SDLW_DrawFilledRect((SDL_Rect){// einen Auswahlrahmen und
                                       textInput->textRectSize.x + textInput->borderWidth,
                                       textInput->textRectSize.y + textInput->borderWidth,
                                       textInput->textRectSize.w - 2 * textInput->borderWidth,
                                       textInput->textRectSize.h - 2 * textInput->borderWidth},
                            textInput->textBgc);
        SDLW_DrawTexture(textInput->textTextur); // den darin enthaltenden Text gezeichnet.
        break;
    }
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int UpdateText(text_t *textInput) {
    if (!textInput) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }

    SDL_DestroyTexture(textInput->textTextur.texture); // Zerstöre die angegebene Textur.
    Sprite_CreateText(textInput->text, textInput->font, textInput->textColor, &textInput->textTextur);
    int w, h;
    SDL_QueryTexture(textInput->textTextur.texture, NULL, NULL, &w, &h);       // Abfrage der Attribute einer Textur
    textInput->textTextur.position.x = textInput->textRectSize.x + w / 2 + 10; // Richtet die Textposition aus, damit der Text linksbündig dargestellt wird.
    textInput->textTextur.position.y = textInput->textRectSize.y + textInput->textRectSize.h / 2;

    return ERR_OK;
}
