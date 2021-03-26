/**
 * @file entityHandler.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Verwaltet alle Entitäten
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */


/*
 * Includes
 * 
 */

#include "error.h"
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

int EntityHandler_Update(inputEvent_t *inputEvents) {
    (void) inputEvents;
    return ERR_FAIL;
}

int EntityHandler_Draw() {
    return ERR_FAIL;
}

int EntityHandler_AddEntity(entity_t *entity) {
    (void) entity;
    return ERR_FAIL;
}

int EntityHandler_RemoveEntity(entity_t *entity) {
    (void) entity;
    return ERR_FAIL;
}

int EntityHandler_RemoveAll() {
    return ERR_FAIL;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
