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
#include "error.h"


/*
 * Tests
 * 
 */

/**
 * @brief Testet das Initialisieren von Listen
 * 
 * @param state unbenutzt
 */
static void init_and_check_initialized_members(void **state) {
    (void)state;
    list_t list;
    assert_int_equal(List_Init(&list), ERR_OK);
    //Listenmember werden richtig initialisiert
    assert_int_equal(list.elementCount, 0);
    assert_null(list.listHead);
    assert_null(list.dataAutoFree);
}

/**
 * @brief Testet das Erstellen und Zerstören von listen
 * 
 * @param state unbenutzt
 */
static void create_then_destroy_list(void **state) {
    (void)state;
    list_t *list = NULL;
    assert_int_equal(List_Create(&list), ERR_OK);
    assert_non_null(list); //Liste wurde erstellt
    assert_int_equal(List_Destroy(&list), ERR_OK);
    assert_null(list); //Liste wurde zerstört
}

/**
 * @brief Testet ob die Liste Element hinzufügen und entfernen kann
 * 
 * @param state unbenutzt
 */
static void check_list_add_and_remove(void **state) {
    (void)state;
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
 * 
 * @param state unbenutzt
 */
static void check_clear(void **state) {
    (void)state;
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
 * 
 * @param state unbenutzt
 */
static void check_adding_order(void **state) {
    (void)state;
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

/**
 * @brief Funktionsaufruf zum simulieren von \ref fnPntrDataCallback.
 *
 * Wird für das Suchen und Vergleichen gebraucht.
 * 
 * @param data Datenpointer der Liste
 * 
 * @return gemäss will_return() von CMocka
 */
static int onCallbackData(void *data) {
    check_expected_ptr(data);
    function_called();
    return mock_type(int);
}

/**
 * @brief Funktionsaufruf zum simulieren von \ref fnPntrDataCallbackArg
 * 
 * Wird für das Suchen und Vergleichen gebraucht.
 * 
 * @param data Datenpointer der Liste
 * @param userData übergebene Benutzerdaten
 * 
 * @return gemäss will_return() von CMocka
 */
static int onCallbackDataArg(void *data, void *userData) {
    check_expected_ptr(data);
    check_expected_ptr(userData);
    function_called();
    return mock_type(int);
}

/**
 * @brief Testet ob die Schleife alle Elemente in der richtigen Reihenfolge aufruft.
 * 
 * Zudem wird getestet ob die Schleife bei einem Fehler abbricht.
 * 
 * @param state unbenutzt
 */
static void check_foreach_loop(void **state) {
    (void)state;
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
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, ERR_OK);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, ERR_OK);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v3);
        will_return(onCallbackData, ERR_OK);
        assert_int_equal(List_Foreach(&list, onCallbackData), ERR_OK);

        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v1);
        expect_value(onCallbackDataArg, userData, &arg);
        will_return(onCallbackDataArg, ERR_OK);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v2);
        expect_value(onCallbackDataArg, userData, &arg);
        will_return(onCallbackDataArg, ERR_OK);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v3);
        expect_value(onCallbackDataArg, userData, &arg);
        will_return(onCallbackDataArg, ERR_OK);
        assert_int_equal(List_ForeachArg(&list, onCallbackDataArg, &arg), ERR_OK);
    }
    //Schleifendurchlauf mit Fehler ERR_FAIL
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, ERR_OK);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, ERR_FAIL);
        assert_int_equal(List_Foreach(&list, onCallbackData), ERR_FAIL);

        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v1);
        expect_value(onCallbackDataArg, userData, &arg);
        will_return(onCallbackDataArg, ERR_OK);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v2);
        expect_value(onCallbackDataArg, userData, &arg);
        will_return(onCallbackDataArg, ERR_FAIL);
        assert_int_equal(List_ForeachArg(&list, onCallbackDataArg, &arg), ERR_FAIL);
    }

    List_Clear(&list);
}

static int searchNumber; //!< Das gesuchte Element für check_search()

/**
 * @brief Testet ob die Listensuche das richtige Element ausgibt
 * 
 * @param state unbenutzt
 */
static void check_search(void **state) {
    (void)state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;

    int *searchResult;
    searchNumber = 1;

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);

    //Suche nach einem nicht existierenden Element
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, 0);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, 0);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v3);
        will_return(onCallbackData, 0);
        assert_int_equal(List_Search(&list, onCallbackData, (void **)&searchResult), ERR_OK);
        assert_null(searchResult);

        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v1);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 0);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v2);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 0);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v3);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 0);
        assert_int_equal(List_SearchArg(&list, onCallbackDataArg, &searchNumber, (void **)&searchResult), ERR_OK);
        assert_null(searchResult);
    }
    // Suche nach dem dritten Element
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, 0);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, 0);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v3);
        will_return(onCallbackData, 1);
        assert_int_equal(List_Search(&list, onCallbackData, (void **)&searchResult), ERR_OK);
        assert_ptr_equal(searchResult, &v3);

        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v1);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 0);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v2);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 0);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v3);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 1);
        assert_int_equal(List_SearchArg(&list, onCallbackDataArg, &searchNumber, (void **)&searchResult), ERR_OK);
        assert_ptr_equal(searchResult, &v3);
    }
    // Suche nach dem zweiten Element
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, 0);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, 1);
        assert_int_equal(List_Search(&list, onCallbackData, (void **)&searchResult), ERR_OK);
        assert_ptr_equal(searchResult, &v2);

        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v1);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 0);
        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v2);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 1);
        assert_int_equal(List_SearchArg(&list, onCallbackDataArg, &searchNumber, (void **)&searchResult), ERR_OK);
        assert_ptr_equal(searchResult, &v2);
    }
    // Suche nach dem ersten Element
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, 1);
        assert_int_equal(List_Search(&list, onCallbackData, (void **)&searchResult), ERR_OK);
        assert_ptr_equal(searchResult, &v1);

        expect_function_call(onCallbackDataArg);
        expect_value(onCallbackDataArg, data, &v1);
        expect_value(onCallbackDataArg, userData, &searchNumber);
        will_return(onCallbackDataArg, 1);
        assert_int_equal(List_SearchArg(&list, onCallbackDataArg, &searchNumber, (void **)&searchResult), ERR_OK);
        assert_ptr_equal(searchResult, &v1);
    }

    List_Clear(&list);
}

/**
 * @brief Testet ob das \ref list_t.dataAutoFree richtig aufgerufen wird.
 * 
 * @param state unbenutzt
 */
static void check_dataAutoFree(void **state) {
    (void)state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);
    list.dataAutoFree = onCallbackData;
    // Clear hat funktioniert
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, ERR_OK);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, ERR_OK);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v3);
        will_return(onCallbackData, ERR_OK);
        assert_int_equal(List_Clear(&list), ERR_OK);
    }

    List_Init(&list);
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);
    list.dataAutoFree = onCallbackData;
    // Clear hat Fehler
    {
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v1);
        will_return(onCallbackData, ERR_OK);
        expect_function_call(onCallbackData);
        expect_value(onCallbackData, data, &v2);
        will_return(onCallbackData, ERR_FAIL);
        assert_int_equal(List_Clear(&list), ERR_FAIL);
    }

    list.dataAutoFree = NULL;
    List_Clear(&list);
}

/**
 * @brief Überprüft ob alle Nullabfragen vorhanden sind
 * 
 * @param state unbenutzt
 */
static void check_null_catch(void **state) {
    (void)state;
    int v1 = 0;
    int *v2 = &v1;
    int *v3 = NULL;
    list_t list;
    List_Init(&list);
    // Grundfunktionen
    assert_int_equal(List_Init(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Add(NULL, &v1), ERR_NULLPARAMETER);
    assert_int_equal(List_Remove(NULL, &v1), ERR_NULLPARAMETER);
    assert_int_equal(List_Clear(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Create(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Destroy(NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Destroy((list_t **)&v3), ERR_NULLPARAMETER);
    // Einfache Hilfsfunktionen
    assert_int_equal(List_Foreach(NULL, onCallbackData), ERR_NULLPARAMETER);
    assert_int_equal(List_Foreach(&list, NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_Search(NULL, onCallbackData, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_Search(&list, NULL, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_Search(&list, onCallbackData, NULL), ERR_NULLPARAMETER);
    // Hilfsfunktionen mit Benutzerdaten
    assert_int_equal(List_ForeachArg(NULL, onCallbackDataArg, NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_ForeachArg(&list, NULL, NULL), ERR_NULLPARAMETER);
    assert_int_equal(List_SearchArg(NULL, onCallbackDataArg, NULL, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_SearchArg(&list, NULL, NULL, (void **)&v2), ERR_NULLPARAMETER);
    assert_int_equal(List_SearchArg(&list, onCallbackDataArg, NULL, NULL), ERR_NULLPARAMETER);
}

/**
 * @brief Mock-Callback für nachfolgenden Test
 * Wird für \ref lists_can_delete_elements_inside_for_loop benötigt. Löscht
 * jedes Element der Liste währendem über die Liste iteriert wird.
 * 
 * @param data Pointer auf Listenelement
 * 
 * @return gemäss List_Remove
 */
static int forCallbackDeleteAll(void *data) {
    function_called();
    return List_Remove(mock_type(list_t*), data);
}

/**
 * @brief Mock-Callback für nachfolgenden Test (mit Argument)
 * Wird für \ref lists_can_delete_elements_inside_for_loop benötigt. Löscht
 * jedes Element der Liste währendem über die Liste iteriert wird.
 * 
 * @param data Pointer auf Listenelement
 * @param userData unbenutzt
 * 
 * @return gemäss List_Remove
 */
static int forCallbackDeleteAllArg(void *data, void *userData) {
    (void)userData;
    function_called();
    return List_Remove(mock_type(list_t*), data);
}

/**
 * @brief Testet ob während dem Interieren mit den For-Funktionen Elemente aus
 * der Liste gelöscht werden dürfen.
 *  
 * @param state unbenutzt
 */
static void lists_can_delete_elements_inside_for_loop(void **state) {
    (void)state;
    list_t list;
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;
    List_Init(&list);
    // Standard For
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);
    will_return_always(forCallbackDeleteAll, &list);
    expect_function_calls(forCallbackDeleteAll, 3);
    assert_int_equal(List_Foreach(&list, forCallbackDeleteAll), ERR_OK);
    // For mit Argument (caste Callback auf Funktion ohne Argument)
    List_Add(&list, &v3);
    List_Add(&list, &v2);
    List_Add(&list, &v1);
    will_return_always(forCallbackDeleteAllArg, &list);
    expect_function_calls(forCallbackDeleteAllArg, 3);
    assert_int_equal(List_ForeachArg(&list, forCallbackDeleteAllArg, NULL), ERR_OK);
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
        cmocka_unit_test(check_null_catch),
        cmocka_unit_test(lists_can_delete_elements_inside_for_loop),
    };
    return cmocka_run_group_tests(list, NULL, NULL);
}
