/**
 * @file adder.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Addition von Ganzzahlen
 * @version 0.1
 * @date 2021-03-05
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
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
