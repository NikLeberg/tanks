/**
 * @file gui.c
 * @author Hinnaoui Angelo (hinna2@bfh.ch)
 * @brief GUI c
 * @version 0.01
 * @date 2021-03-19
 *
 * @copyright Copyright (c) 2021 Hinnaoui Angelo
 *
 */


/*
 * Includes
 *
 */

#include "SDL_ttf.h"
#include "sdlWrapper.h"
#include "list.h"
#include "error.h"
#include "gui.h"

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

/* ... */


/*
 * Private Funktionsprototypen
 * 
 */

/**
 * @brief DrawElement
 *
 * Je nach Typ des erhaltenen gui Element, wird es der entsprechenden Funktion übergeben.
 *
 * @param[in] element
 * @return ERR_OK oder ERR_NULLPARAMETER
 */

static int DrawElement(guiElement_t *element);

/**
 * @brief UpdateElement
 *
 * Je nach Typ des zu aktualisierenden Element, wird es der entsprechenden Funktion übergeben.
 *
 * @param[in] element
 * @param[in] inputEvents
 * @return ERR_OK oder ERR_NULLPARAMETER
 */

static int UpdateElement(guiElement_t *element, inputEvent_t *inputEvents);


/*
 * Implementation Öffentlicher Funktionen
 * 
 */

int GUI_Init(gui_t *gui) {
    if (!gui) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }

    if (List_Init(&gui->element) != ERR_OK) { // Verifizierung
        return ERR_FAIL;
    }
    return ERR_OK;
}

int GUI_Update(inputEvent_t *inputEvents, gui_t *gui) {
    if (!inputEvents || !gui) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }

    List_ForeachArg(&gui->element, (fnPntrDataCallbackArg)UpdateElement, inputEvents);
    return ERR_OK;
}

int GUI_Draw(gui_t *gui) {
    if (!gui) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }

    List_Foreach(&gui->element, (fnPntrDataCallback)DrawElement);

    (void)gui;
    return ERR_OK;
}

int GUI_AddElement(guiElement_t *element, gui_t *gui) {
    if (!element || !gui) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    if (List_Add(&gui->element, element) != ERR_OK) { // Ist das Hinzufügen von gui Elemente fehlgeschlagen wird eine Fehlermeldung ausgegeben.
        printf("Hinzufügen des Elements fehlgeschlagen");
    }
    return ERR_OK;
}

int GUI_RemoveElement(guiElement_t *element, gui_t *gui) {
    if (!element || !gui) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    List_Remove(&gui->element, element);
    return ERR_OK;
}


/*
 * Implementation Privater Funktionen
 * 
 */

static int DrawElement(guiElement_t *element) {
    if (!element) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    switch (element->type) {
    case TYPE_BUTTON:
        Button_Draw(&element->elementData.button);
        break;
    case TYPE_TEXT:
        Text_Draw(&element->elementData.textInput);
        break;
    }
    return ERR_OK;
}

static int UpdateElement(guiElement_t *element, inputEvent_t *inputEvents) {
    if (!element || !inputEvents) { // Fehlerüberprüfung

        return ERR_NULLPARAMETER;
    }
    switch (element->type) {
    case TYPE_BUTTON:
        Button_Update(inputEvents, &element->elementData.button);
        break;
    case TYPE_TEXT:
        Text_Update(inputEvents, &element->elementData.textInput);
        break;
    }
    return ERR_OK;
}
