/**
 * @file mock_heap.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Mocks dynamischer Speicher Funktionen
 * @version 0.1
 * @date 2021-04-10
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 */


// Ubuntu der GitLab-Pipeline mag es nicht wenn man malloc / free neu definiert.
#ifndef CI_TEST


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
 * @brief Mock-Ersatz für originales malloc()
 * 
 * Verwendet das von CMocka gelieferte System zur Prüfung von
 * Speicherallozierung. Wird ein Block alloziert welcher später nicht per free()
 * befreit wird, wird dies als Speicherleck gemeldet.
 * 
 * @param size Grösse des Speicherblocks
 * 
 * @return Pointer auf Speicherblock
 */
void *malloc(const size_t size) {
    return test_malloc(size);
}

/**
 * @brief Mock-Ersatz für originales calloc()
 * 
 * Verwendet das von CMocka gelieferte System zur Prüfung von
 * Speicherallozierung. Wird ein Block alloziert welcher später nicht per free()
 * befreit wird, wird dies als Speicherleck gemeldet.
 * 
 * @param num Anzahl zu allozierender Speicherblöcke
 * @param size Grösse eines Speicherblocks
 * 
 * @return Pointer auf Speicherblock
 */
void *calloc(const size_t num, const size_t size) {
    return test_calloc(num, size);
}

/**
 * @brief Mock-Ersatz für originales realloc()
 * 
 * Verwendet das von CMocka gelieferte System zur Prüfung von
 * Speicherallozierung. Wird ein Block alloziert welcher später nicht per free()
 * befreit wird, wird dies als Speicherleck gemeldet.
 * 
 * @param ptr Pointer auf Speicherblock
 * @param size neue Grösse des Speicherblocks
 * 
 * @return Pointer auf Speicherblock
 */
void *realloc(void *ptr, size_t size);

/**
 * @brief Mock-Ersatz für originales free()
 * 
 * Verwendet das von CMocka gelieferte System zur Prüfung von
 * Speicherallozierung. Wird ein Block mit malloc() alloziert welcher später
 * nicht hiermit befreit wird, wird dies als Speicherleck gemeldet.
 * 
 * @param ptr Pointer auf Speicherblock
 */
void free(void *const ptr) {
    test_free(ptr);
}

#else

/**
 * @brief Dummy Typedef
 * Durch aktiviertem -pedantic Flag des Compilers ist eine .c Datei ohne Inhalt
 * nicht zulässig. Unterdrücke hiermit die Warnung.
 */
typedef int mutePedanticWarning;

#endif
