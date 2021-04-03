/**
 * @file test_list.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Tests für list-Modul
 * @version 0.1
 * @date 2021-03-26
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

#include "list.h"
#include <stdlib.h>

#include "error.h"


/*
 * Tests
 * 
 */

/**
 * @brief Testet das Initialisieren von Listen
 */
static void init_and_check_initialized_members(void **state) {
    (void) state;
    list_t list;
    assert_int_equal(List_Init(&list), ERR_OK);
    //Listenmember werden richtig initialisiert
    assert_int_equal(list.elementCount, 0);
    assert_null(list.listHead);
    assert_null(list.dataAutoFree);
}

/**
 * @brief Testet das Erstellen und Zerstören von listen
 */
static void create_then_destroy_list(void **state) {
    (void) state;
    list_t *list = NULL;
    assert_int_equal(List_Create(&list), ERR_OK);
    assert_non_null(list); //Liste wurde erstellt
    assert_int_equal(List_Destroy(&list), ERR_OK);
    assert_null(list); //Liste wurde zerstört
}

/**
 * @brief Testet ob die Liste Element hinzufügen und entfernen kann
 */
static void check_list_add_and_remove(void **state) {
    (void) state;
    list_t list;
    int v1 = 1;
    int v2 = 1;
    List_Init(&list);

    assert_int_equal(List_Add(&list, &v1), ERR_OK); //Erstes Element
    assert_non_null(list.listHead);
    assert_int_equal(list.elementCount, 1);

    assert_int_equal(List_Add(&list, &v1), ERR_OK); //Element Anzahl
    assert_non_null(list.listHead);
    assert_int_equal(list.elementCount, 2);

    assert_int_equal(List_Remove(&list, NULL), ERR_OK); //Null entfernen, existiert nicht
    assert_non_null(list.listHead);
    assert_int_equal(list.elementCount, 2);

    assert_int_equal(List_Remove(&list, &v2), ERR_OK); //Nicht existierendes Element entfernen
    assert_non_null(list.listHead);
    assert_int_equal(list.elementCount, 2);

    assert_int_equal(List_Remove(&list, &v1), ERR_OK); //Element Entfernen
    assert_non_null(list.listHead);
    assert_int_equal(list.elementCount, 1);

    assert_int_equal(List_Remove(&list, &v1), ERR_OK); //Letztes Element entfernen
    assert_null(list.listHead);
    assert_int_equal(list.elementCount, 0);
}

/**
 * @brief Testet ob alle Elemente der Liste entfernt werden kann
 */
static void check_clear(void **state) {
    (void) state;
    list_t list;
    int v1 = 1;
    List_Init(&list);

    List_Add(&list, &v1);
    List_Add(&list, &v1);
    List_Add(&list, &v1);

    //Löschen aller Elemente in der liste
    assert_int_equal(List_Clear(&list), ERR_OK);
    assert_null(list.listHead);
    assert_int_equal(list.elementCount, 0);
}

/**
 * @brief Testet ob die Reihenfolge der hinzugefügten Elemente den Spezifikationen entspricht
 */
static void check_adding_order(void **state) {
    (void) state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;

    List_Init(&list);

    List_Add(&list, &v1); // Erstes Element
    assert_ptr_equal(list.listHead->data, &v1);

    List_Add(&list, &v2); // Zweites Element vor dem ersten
    assert_ptr_equal(list.listHead->nextElement->data, &v1);
    assert_ptr_equal(list.listHead->data, &v2);

    List_Add(&list, &v3); // Drittes Element vor dem ersten, erstes Element am Schluss
    assert_ptr_equal(list.listHead->nextElement->nextElement->data, &v1);
    assert_ptr_equal(list.listHead->nextElement->data, &v2);
    assert_ptr_equal(list.listHead->data, &v3);

    List_Clear(&list);
}

static int loopCheckNumber; //!< Wert zum überprüfen ob foreach_loop_check mit dem richtigen Element aufgerufen wurde
static int loopCheckReturn; //!< Returnwert von foreach_loop_check
/**
 * @brief Überprüft ob das jetztige Element hätte aufgerufen werden sollen.
 * Gibt je nach \ref loopCheckReturn einen Fehlercode zurück
 */
static int foreach_loop_check(void *data) {
    int *check = (int *)data;
    assert_int_equal(*check, loopCheckNumber);
    loopCheckNumber++;
    return loopCheckReturn;
}
/**
 * @brief Überprüft ob das jetztige Element hätte aufgerufen werden sollen.
 * Gibt je nach \ref loopCheckReturn einen Fehlercode zurück
 */
static int foreach_loop_checkArg(void *data, void *userData) {
    int *check = (int *)data;
    int *increment = (int *)userData;
    assert_int_equal(*check, (*increment));
    (*increment) += 1;
    return loopCheckReturn;
}

/**
 * @brief Testet ob die Schleife alle Elemente in der richtigen Reihenfolge aufruft.
 * Zudem wird getestet ob die Schleife bei einem Fehler abbricht.
 */
static void check_foreach_loop(void **state) {
    (void) state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;

    int arg = 1;

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);

    //Fehlerfreien Schleifendurchlauf
    loopCheckNumber = 1;
    loopCheckReturn = 0;
    assert_int_equal(List_Foreach(&list, foreach_loop_check), ERR_OK);
    assert_int_equal(loopCheckNumber, 4); // Alle Elemente wurden aufgerufen
    arg = 1;
    assert_int_equal(List_ForeachArg(&list, foreach_loop_checkArg, &arg), ERR_OK);
    assert_int_equal(loopCheckNumber, 4); // Alle Elemente wurden aufgerufen

    //Schleifendurchlauf mit Fehler ERR_FAIL
    loopCheckReturn = ERR_FAIL;
    loopCheckNumber = 1;
    assert_int_equal(List_Foreach(&list, foreach_loop_check), ERR_FAIL);
    assert_int_equal(loopCheckNumber, 2); // Nur das erste Element wurde aufgerufen
    arg = 1;
    assert_int_equal(List_ForeachArg(&list, foreach_loop_checkArg, &arg), ERR_FAIL);
    assert_int_equal(loopCheckNumber, 2); // Nur das erste Element wurde aufgerufen

    List_Clear(&list);
}

static int searchNumber; //!< Das gesuchte Element für check_search()
/**
 * @brief Die Funktion zum Vergleichen des gesuchten Elements für check_search()
 */
static int search_compare(void *data) {
    int *intData = (int *)data;
    return (*intData) == searchNumber;
}
/**
 * @brief Die Funktion zum Vergleichen des gesuchten Elements für check_search() mit Argument
 */
static int search_compareArg(void *data, void *userData) {
    int *intData = (int *)data;
    int *cmp = (int *)userData;
    return (*intData) == (*cmp);
}
/**
 * @brief Testet ob die Listensuche das richtige Element ausgibt
 */
static void check_search(void **state) {
    (void) state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;

    int *searchResult;

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);

    //Suche nach einem nicht existierenden Element
    searchNumber = 0;
    assert_int_equal(List_Search(&list, search_compare, (void **)&searchResult), ERR_OK);
    assert_null(searchResult);

    //Suche nach dem ersten Element
    searchNumber = 1;
    assert_int_equal(List_Search(&list, search_compare, (void **)&searchResult), ERR_OK);
    assert_ptr_equal(searchResult, &v1);
    assert_int_equal(List_SearchArg(&list, search_compareArg, &searchNumber, (void **)&searchResult), ERR_OK);
    assert_ptr_equal(searchResult, &v1);

    //Suche nach dem zweiten Element
    searchNumber = 2;
    assert_int_equal(List_Search(&list, search_compare, (void **)&searchResult), ERR_OK);
    assert_ptr_equal(searchResult, &v2);
    assert_int_equal(List_SearchArg(&list, search_compareArg, &searchNumber, (void **)&searchResult), ERR_OK);
    assert_ptr_equal(searchResult, &v2);

    //Suche nach dem letzten Element
    searchNumber = 3;
    assert_int_equal(List_Search(&list, search_compare, (void **)&searchResult), ERR_OK);
    assert_ptr_equal(searchResult, &v3);
    assert_int_equal(List_SearchArg(&list, search_compareArg, &searchNumber, (void **)&searchResult), ERR_OK);
    assert_ptr_equal(searchResult, &v3);
}

static int freeCheckNumber; //!< Wert um zu überprüfen ob dataAutoFree() mit dem Richtigen Element aufgerufen wurde
static int freeCheckReturn; //!< Rückgabewert von dataAutoFree(), Überprüfung ob das clear richtig abgebrochen wurde
/**
 * @brief Überprüft ob von check_dataAutoFree() das richtige Element aufgerufen wurde
 */
static int dataAutoFree(void *data) {
    int *intData = (int *)data;
    assert_int_equal(*intData, freeCheckNumber);
    freeCheckNumber++;
    return freeCheckReturn;
}
/**
 * @brief Testet ob das \ref list_t.dataAutoFree richtig aufgerufen wird.
 */
static void check_dataAutoFree(void **state) {
    (void) state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);

    list.dataAutoFree = dataAutoFree;
    freeCheckNumber = 1;
    freeCheckReturn = ERR_OK;
    assert_int_equal(List_Clear(&list), ERR_OK);
    assert_int_equal(freeCheckNumber, 4); // Von allen Elementen wurde dataAutoFree aufgerufen

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);

    list.dataAutoFree = dataAutoFree;
    freeCheckNumber = 1;
    freeCheckReturn = ERR_FAIL;
    assert_int_equal(List_Clear(&list), ERR_FAIL);
    assert_int_equal(freeCheckNumber, 2); // Clear wurde nach dem ersten Element abgebrochen

    freeCheckReturn = 0;
    list.dataAutoFree = NULL;
    List_Clear(&list);
}

/**
 * @brief Überprüft ob alle Nullabfragen vorhanden sind
 */
static void check_null_catch(void **state) {
    (void) state;
    int v1 = 0;
    int *v2 = &v1;
    int *v3 = NULL;
    list_t list;
    List_Init(&list);
    assert_int_equal(List_Init(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Add(NULL, &v1), ERR_NULLPARAMETER);
    assert_int_equal(List_Remove(NULL, &v1), ERR_NULLPARAMETER);
    assert_int_equal(List_Clear(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Create(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Destroy(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Destroy((list_t **)&v3), ERR_NULLPARAMETER);
    assert_int_equal(List_Foreach(NULL, dataAutoFree), ERR_NULLPARAMETER);
    assert_int_equal(List_Foreach(&list, NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Search(NULL, dataAutoFree, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_Search(&list, NULL, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_Search(&list, dataAutoFree, NULL), ERR_NULLPARAMETER);

    assert_int_equal(List_ForeachArg(NULL, search_compareArg, NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_ForeachArg(&list, NULL, NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_SearchArg(NULL, search_compareArg, NULL, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_SearchArg(&list, NULL, NULL, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_SearchArg(&list, search_compareArg, NULL, NULL), ERR_NULLPARAMETER);
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest list[] = {
        cmocka_unit_test(init_and_check_initialized_members),
        cmocka_unit_test(create_then_destroy_list),
        cmocka_unit_test(check_list_add_and_remove),
        cmocka_unit_test(check_clear),
        cmocka_unit_test(check_adding_order),
        cmocka_unit_test(check_foreach_loop),
        cmocka_unit_test(check_search),
        cmocka_unit_test(check_dataAutoFree),
        cmocka_unit_test(check_null_catch)};
    return cmocka_run_group_tests(list, NULL, NULL);
}
