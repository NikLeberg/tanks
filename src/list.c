/**
 * @file list.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Single linked list with automatic free function
 * @version 1.0
 * @date 2021-03-26
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

#include "list.h"
#include "error.h"

/*
 * Implementation öffentlicher Funktionen
 *
 */

int List_Create(list_t **list) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungueltig! List_Create()\n");
        return ERR_NULLPARAMETER;
    }

    (*list) = malloc(sizeof(list_t)); // Erstellen einer neuen Liste
    if (!*list) {                     // Fehlerüberprüfung
        printf("Liste konnte nicht alloziert werden! List_Create()\n");
        return ERR_MEMORY;
    }

    List_Init((*list)); //Initialisierung der erstellten Liste
    return ERR_OK;
}

int List_Init(list_t *list) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungueltig! List_Init()\n");
        return ERR_NULLPARAMETER;
    }

    // Setzen der Initialwerte
    list->dataAutoFree = NULL;
    list->elementCount = 0;
    list->listHead = NULL;
    return ERR_OK;
}

int List_Destroy(list_t **list) {
    if (!list || !(*list)) { // Fehlerüberprüfung
        printf("Liste ungueltig! List_Destroy()\n");
        return ERR_NULLPARAMETER;
    }

    List_Clear(*list); // Deallozierung aller Elemente
    free(*list);
    *list = NULL;
    return ERR_OK;
}

int List_Add(list_t *list, void *data) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungueltig! List_Add()\n");
        return ERR_NULLPARAMETER;
    }

    listElement_t *element = malloc(sizeof(listElement_t)); // Erstellen eines neuen Elementes
    if (!element) {                                         // Fehlerüberprüfung
        printf("Element konnte nicht alloziert werden! List_Add()\n");
        return ERR_MEMORY;
    }

    // Einfügen des neuen Elementes am Anfang der Liste
    element->data = data;
    element->nextElement = list->listHead;
    list->listHead = element;
    list->elementCount++;
    return ERR_OK;
}

int List_Remove(list_t *list, void *data) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungültig! List_Remove()\n");
        return ERR_NULLPARAMETER;
    }

    listElement_t *element = list->listHead;
    listElement_t *prevElement = NULL;

    // Suche für das Element zum Entfernen
    while (element) {
        if (element->data == data) {
            int errCode = 0;
            if (list->dataAutoFree) // Ruft den Dekonstruktor des Elementes auf
                errCode = list->dataAutoFree(element->data);

            if (errCode)
                return errCode;

            if (prevElement) // Ist das jetztige Element am Anfang
                prevElement->nextElement = element->nextElement;
            else
                list->listHead = element->nextElement;

            list->elementCount--;
            free(element);
            break;
        }

        // Suche beim nächsten Element
        prevElement = element;
        element = element->nextElement;
    }
    return ERR_OK;
}

int List_Clear(list_t *list) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungültig! List_Clear()\n");
        return ERR_NULLPARAMETER;
    }


    while (list->listHead) { // Entfernen aller Elemente
        int errCode = List_Remove(list, list->listHead->data);
        if (errCode)
            return errCode;
    }

    // Setzen der Initialwerte
    list->elementCount = 0;
    list->listHead = NULL;
    return 0;
}

int List_Foreach(list_t *list, fnPntrDataCallback callback) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    if (!callback) { // Fehlerüberprüfung
        printf("Callback ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    listElement_t *element = list->listHead;

    while (element) {                           // Schleife über jedes Element
        int errorVal = callback(element->data); // Callback Aufruf
        if (errorVal)                           // Bei Error wird das Foreach abgebrochen
            return errorVal;
        element = element->nextElement;
    }
    return ERR_OK;
}

int List_ForeachArg(list_t *list, fnPntrDataCallbackArg callback, void *userData) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    if (!callback) { // Fehlerüberprüfung
        printf("Callback ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    listElement_t *element = list->listHead;

    while (element) {                                     // Schleife über jedes Element
        int errorVal = callback(element->data, userData); // Callback Aufruf
        if (errorVal)                                     // Bei Error wird das Foreach abgebrochen
            return errorVal;
        element = element->nextElement;
    }
    return ERR_OK;
}

int List_Search(list_t *list, fnPntrDataCallback searchCmpFn, void **data) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungültig! List_Search()\n");
        return ERR_NULLPARAMETER;
    }

    if (!searchCmpFn) { // Fehlerüberprüfung
        printf("Searchfunktion ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    if (!data) { // Fehlerüberprüfung
        printf("Datenziel ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    listElement_t *element = list->listHead;
    (*data) = NULL;                       // Initialisieren des Rückgabewertes
    while (element) {                     // Schleife über jedes Element
        if (searchCmpFn(element->data)) { // Überprüfung ob die Suchbedingung erfüllt ist
            (*data) = element->data;
            break;
        }
        element = element->nextElement;
    }
    return ERR_OK;
}

int List_SearchArg(list_t *list, fnPntrDataCallbackArg searchCmpFn, void *userData, void **data) {
    if (!list) { // Fehlerüberprüfung
        printf("Liste ungültig! List_Search()\n");
        return ERR_NULLPARAMETER;
    }

    if (!searchCmpFn) { // Fehlerüberprüfung
        printf("Searchfunktion ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    if (!data) { // Fehlerüberprüfung
        printf("Datenziel ungültig! List_Foreach()\n");
        return ERR_NULLPARAMETER;
    }

    listElement_t *element = list->listHead;
    (*data) = NULL;                                 // Initialisieren des Rückgabewertes
    while (element) {                               // Schleife über jedes Element
        if (searchCmpFn(element->data, userData)) { // Überprüfung ob die Suchbedingung erfüllt ist
            (*data) = element->data;
            break;
        }
        element = element->nextElement;
    }
    return ERR_OK;
}
