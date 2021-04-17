/**
 * @file mock_physics.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Mocks einiger Physics Funktionen
 * @version 0.1
 * @date 2021-04-17
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */


/*
 * Includes
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


/*
 * Mocks
 * 
 */

/**
 * @brief Mockup des echten \ref Physics_Update().
 * 
 * Ersetze das originale \ref Physics_Update() mit dieser Funktion.
 * Somit kann isoliert getestet werden.
 * 
 * @param[in] entityList Liste aller Entitäten
 * 
 * @return immer 0
 */
int Physics_Update(void *entityList) {
    (void)entityList;
    return 0;
}
