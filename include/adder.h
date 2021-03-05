/**
 * @file adder.h
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Addition von Ganzzahlen
 * @version 0.1
 * @date 2021-03-05
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */

#pragma once


/*
 * Includes
 * 
 */

/* ... */


/*
 * Typdeklarationen
 * 
 */

/**
 * @brief Beispielstruktur
 * 
 */
typedef struct {
    int a; //!< Erste Zahl
    int b; //!< Zweite Zahl
    int result; //!< Ergebnis der Berechnung mittels add2()
} addition_t;


/*
 * Variablendeklarationen
 * 
 */

/* ... */


/*
 * Öffentliche Funktionen
 * 
 */

/**
 * @brief Addiere zwei Ganzzahlen
 * 
 * @param a Zahl a
 * @param b Zahl b
 * @return int a + b
 */
int add1(int a, int b);

/**
 * @brief Addiere zwei Ganzzahlen aus Struktur
 * 
 * @param addition[in,out] Pointer auf Struktur der zu addierenden Zahlen
 * @return int a + b
 */
int add2(addition_t *addition);
