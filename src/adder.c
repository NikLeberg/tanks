/**
 * @file adder.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Addition von Ganzzahlen (Vorlage)
 * @version 0.2
 * @date 2021-03-05
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 * Dieses Modul dient als Vorlage für andere Module.
 * Im eventuellen Release ist dies zu entfernen.
 * 
 */


/*
 * Includes
 * 
 */

#include "adder.h"


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

int add1(int a, int b) {
    return a + b;
}

int add2(addition_t *addition) {
    if (!addition) {
        return 0;
    }
    addition->result = addition->a + addition->b;
    return addition->result;
}


/*
 * Implementation Privater Funktionen
 * 
 */

/* ... */
