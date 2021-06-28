/**
 * @file test_adder.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Tests für adder-Modul
 * @version 0.1
 * @date 2021-03-01
 * 
 * @copyright Copyright (c) 2021 Leuenberger Niklaus
 * 
 * Dieses Modul dient lediglich als Vorlage für andere Module.
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

#include "adder.h"


/*
 * Tests
 * 
 */

/**
 * @brief 1 + 1 muss 2 ergeben
 * 
 * @param state unbenutzt
 */
static void one_added_to_one_equals_two(void **state) {
    (void) state;
    assert_int_equal(add1(1, 1), 2);
    return;
}

/**
 * @brief 5 + (-5) muss 0 ergeben
 * 
 * @param state unbenutzt
 */
static void five_added_to_negative_five_equals_zero(void **state) {
    (void) state;
    assert_int_equal(add1(5, -5), 0);
    return;
}

/**
 * @brief Strukturvariante, 1 + 1 muss 2 ergeben
 * 
 * @param state unbenutzt
 */
static void struct_one_added_to_one_equals_two(void **state) {
    (void) state;
#ifdef CI_TEST // Beispiel wie ein Test fürs GitLab-CI deaktiviert werden kann
    skip();
#endif
    addition_t addition = {1, 1, 0};
    assert_int_equal(add2(&addition), 2);
    assert_int_equal(addition.result, 2);
    return;
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest adder[] = {
        cmocka_unit_test(one_added_to_one_equals_two),
        cmocka_unit_test(five_added_to_negative_five_equals_zero),
        cmocka_unit_test(struct_one_added_to_one_equals_two)
    };
    return cmocka_run_group_tests(adder, NULL, NULL);
}
