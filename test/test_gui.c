/**
 * @file test_gui.c
 * @author Leuenberger Niklaus (leuen4@bfh.ch)
 * @brief Tests für GUI-Modul sowie dessen GUI-Elemente
 * @version 0.1
 * @date 2021-06-11
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

#include "error.h"
#define SDL_MAIN_HANDLED
#include "gui.h"
#include "guiElements/button.h"
#include "guiElements/text.h"


/*
 * Tests
 * 
 */

/**
 * @brief GUI und Elemente können nicht mit NULL als Argument aufgerufen werden.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_cant_call_functions_with_null_argument(void **state) {
    (void) state;
    // GUI
    assert_int_equal(GUI_Init(NULL), ERR_NULLPARAMETER);
    assert_int_equal(GUI_Update(NULL, (gui_t*)1), ERR_NULLPARAMETER);
    assert_int_equal(GUI_Update((inputEvent_t*)1, NULL), ERR_NULLPARAMETER);
    assert_int_equal(GUI_Draw(NULL), ERR_NULLPARAMETER);
    assert_int_equal(GUI_AddElement(NULL, (gui_t*)1), ERR_NULLPARAMETER);
    assert_int_equal(GUI_AddElement((guiElement_t*)1, NULL), ERR_NULLPARAMETER);
    assert_int_equal(GUI_RemoveElement(NULL, (gui_t*)1), ERR_NULLPARAMETER);
    assert_int_equal(GUI_RemoveElement((guiElement_t*)1, NULL), ERR_NULLPARAMETER);
    // Button
    assert_int_equal(Button_Init(NULL, "1", "1"), ERR_NULLPARAMETER);
    assert_int_equal(Button_Init((button_t*)1, NULL, "1"), ERR_NULLPARAMETER);
    assert_int_equal(Button_Init((button_t*)1, "1", NULL), ERR_NULLPARAMETER);
    assert_int_equal(Button_Update(NULL, (button_t*)1), ERR_NULLPARAMETER);
    assert_int_equal(Button_Update((inputEvent_t*)1, NULL), ERR_NULLPARAMETER);
    assert_int_equal(Button_Draw(NULL), ERR_NULLPARAMETER);
    // Text
    assert_int_equal(Text_Init(NULL), ERR_NULLPARAMETER);
    assert_int_equal(Text_Update(NULL, (text_t*)1), ERR_NULLPARAMETER);
    assert_int_equal(Text_Update((inputEvent_t*)1, NULL), ERR_NULLPARAMETER);
    assert_int_equal(Text_Draw(NULL), ERR_NULLPARAMETER);
}

/**
 * @brief Gui Struktur kann initialisiert werden.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_can_be_initialized(void **state) {
    (void) state;
    gui_t gui = {0};
    assert_int_equal(GUI_Init(&gui), ERR_OK);
    assert_non_null(&gui.element);
    List_Clear(&gui.element);
}

/**
 * @brief Elemente können zum GUI hinzugefügt und entfernt werden.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_can_add_and_remove_elements(void **state) {
    (void) state;
    gui_t gui = {0};
    GUI_Init(&gui);
    guiElement_t element0 = {0}, element1 = {0};
    // Beide Elemente können hinzugefügt werden
    assert_int_equal(GUI_AddElement(&element0, &gui), ERR_OK);
    assert_int_equal(GUI_AddElement(&element1, &gui), ERR_OK);
    assert_int_equal(gui.element.elementCount, 2);
    // Beide Elemente können entfernt werden
    assert_int_equal(GUI_RemoveElement(&element0, &gui), ERR_OK);
    assert_int_equal(GUI_RemoveElement(&element1, &gui), ERR_OK);
    assert_int_equal(gui.element.elementCount, 0);
    // Ein erneutes Entfernen löst kein Fehler aus
    assert_int_equal(GUI_RemoveElement(&element0, &gui), ERR_OK);
    assert_int_equal(GUI_RemoveElement(&element1, &gui), ERR_OK);
    assert_int_equal(gui.element.elementCount, 0);
    List_Clear(&gui.element);
}

/**
 * @brief Button Struktur kann initialisiert werden.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_button_can_be_initialized(void **state) {
    (void) state;
    // Es wird eine Text-Textur mittels SDLW erstellt, prüfe auf Korrektheit.
    expect_function_call(SDLW_CreateTextTexture);
    will_return(SDLW_CreateTextTexture, ERR_OK);
    button_t button;
    assert_int_equal(Button_Init(&button, "fontXYZ", "Pomelo-Banane"), ERR_OK);
    // Lasse SDLW ein Fehler zurückgeben. Die Text-Textur kann nun nicht mehr
    // erstellt werden und entsprechend sollte ein Fehler zurückgegeben werden.
    expect_function_call(SDLW_CreateTextTexture);
    will_return(SDLW_CreateTextTexture, ERR_FAIL); // Aufruf ist fehlerhaft
    button_t buttonFail;
    assert_int_not_equal(Button_Init(&buttonFail, "fontXYZ", "Pomelo-Banane"), ERR_OK);
}

/**
 * @brief Mockup eines onClick Callbacks
 * 
 * @param id Id-Nummer des Buttons welcher gedrückt wurde
 * 
 */
void onClick(int id) {
    check_expected(id);
    function_called();
    return;
}

/**
 * @brief Beim Update des Buttons wird bei einem Klick der Callback aufgerufen.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_button_update_calls_callback_on_click(void **state) {
    (void) state;
    expect_function_call_any(SDLW_CreateTextTexture);
    will_return_always(SDLW_CreateTextTexture, ERR_OK);
    expect_function_call_any(SDLW_PlaySoundEffect);
    will_return_always(SDLW_PlaySoundEffect, ERR_OK);
    button_t button = {
        .id = 1,
        .onClickCallback = onClick,
        .buttonSize = {.w = 1, .h = 1} // An Position 0,0 mit 1 Pixel an Fläche
    };
    assert_int_equal(Button_Init(&button, "fontXYZ", "Pomelo-Banane"), ERR_OK);
    inputEvent_t input = {0}; // Maus ist an Position 0,0
    // Ohne Klick wird kein Klick-Callback aufgerufen
    assert_int_equal(Button_Update(&input, &button), ERR_OK);
    // Mit gedrückt gehaltener Maus wird auch noch kein Callback ausgelöst
    input.mouseButtons = SDL_BUTTON_LEFT;
    assert_int_equal(Button_Update(&input, &button), ERR_OK);
    // Beim loslassen wird Callback mit richtiger Id aufgerufen
    expect_function_call(onClick);
    expect_value(onClick, id, button.id);
    input.mouseButtons = 0;
    assert_int_equal(Button_Update(&input, &button), ERR_OK);
}

/**
 * @brief Beim Zeichnen wird mit SDLW der Button gezeichnet.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_button_draw_calls_sdlw(void **state) {
    (void) state;
    expect_function_call_any(SDLW_CreateTextTexture);
    will_return_always(SDLW_CreateTextTexture, ERR_OK);
    button_t button = {
        .buttonSize = {.w = 1, .h = 1} // An Position 0,0 mit 1 Pixel an Fläche
    };
    assert_int_equal(Button_Init(&button, "fontXYZ", "Pomelo-Banane"), ERR_OK);
    // Erwarte das Zeichnen vom Rahmen
    // Die Text-Textur wird nicht gezeichnet, da dieser aufrug gemockt wurde.
    expect_function_call(SDLW_DrawFilledRect);
    will_return(SDLW_DrawFilledRect, ERR_OK);
    assert_int_equal(Button_Draw(&button), ERR_OK);
}

/**
 * @brief Text Struktur kann initialisiert werden.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_text_can_be_initialized(void **state) {
    (void) state;
    // Es wird eine Text-Textur mittels SDLW erstellt, prüfe auf Korrektheit.
    expect_function_call(SDLW_CreateTextTexture);
    will_return(SDLW_CreateTextTexture, ERR_OK); // Aufruf ist erfolgreich
    text_t text;
    assert_int_equal(Text_Init(&text), ERR_OK);
    // Lasse SDLW ein Fehler zurückgeben. Die Text-Textur kann nun nicht mehr
    // erstellt werden und entsprechend sollte ein Fehler zurückgegeben werden.
    expect_function_call(SDLW_CreateTextTexture);
    will_return(SDLW_CreateTextTexture, ERR_FAIL); // Aufruf ist fehlerhaft
    text_t textFail;
    assert_int_not_equal(Text_Init(&textFail), ERR_OK);
}

/**
 * @brief Beim Update lässt sich der Text des Textfeldes verändern.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_text_update_changes_text(void **state) {
    (void) state;
    expect_function_call_any(SDLW_PlaySoundEffect);
    will_return_always(SDLW_PlaySoundEffect, ERR_OK);
    expect_function_call_any(SDLW_CreateTextTexture);
    will_return_always(SDLW_CreateTextTexture, ERR_OK);
    text_t text = {
        .text = {"Pomelo-Banane"},
        .textRectSize = {.w = 1, .h = 1} // An Position 0,0 mit 1 Pixel an Fläche
    };
    inputEvent_t input = {0}; // Maus ist an Position 0,0
    // Ohne Klick ins Textfeld kann noch kein Text verändert werden
    input.currentChar = 'H';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_string_equal(text.text, "Pomelo-Banane");
    // Nach einem Klick lässt sich das Feld aber verändern
    input.mouseButtons = SDL_BUTTON_LEFT;
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    // Der Platzhaltertext sollte vollständig ersetzt worden sein
    assert_string_equal(text.text, "H");
    // Weitere Änderungen sich möglich auch ohne Mausklick
    input.mouseButtons = 0;
    input.currentChar = 'a';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_string_equal(text.text, "Ha");
    input.currentChar = 'l';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_string_equal(text.text, "Hall");
    input.currentChar = 'o';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_string_equal(text.text, "Hallo");
    // Text lässt sich auch wieder löschen
    input.currentChar = '\b';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_string_equal(text.text, "Hall");
    // Klick ausserhalb des Textfeldes deaktiviert den Editier-Modus
    input.mouseButtons = SDL_BUTTON_LEFT;
    input.mousePosition = (SDL_Point){.x = 10, .y = 10};
    input.currentChar = '\0';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    input.currentChar = 'o';
    assert_int_equal(Text_Update(&input, &text), ERR_OK);
    assert_string_equal(text.text, "Hall"); // 'o' wurde nicht hinzugefügt
}

/**
 * @brief Beim Zeichnen wird mit SDLW das Textfeld gezeichnet.
 * 
 * @param state unbenutzt
 * 
 */
static void gui_text_draw_calls_sdlw(void **state) {
    (void) state;
    expect_function_call_any(SDLW_CreateTextTexture);
    will_return_always(SDLW_CreateTextTexture, ERR_OK);
    text_t text = {
        .textRectSize = {.w = 1, .h = 1} // An Position 0,0 mit 1 Pixel an Fläche
    };
    assert_int_equal(Text_Init(&text), ERR_OK);
    // Erwarte das Zeichnen vom Rahmen
    // Die Text-Textur wird nicht gezeichnet, da dieser aufrug gemockt wurde.
    expect_function_call(SDLW_DrawFilledRect);
    will_return(SDLW_DrawFilledRect, ERR_OK);
    assert_int_equal(Text_Draw(&text), ERR_OK);
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest gui[] = {
        cmocka_unit_test(gui_cant_call_functions_with_null_argument),
        cmocka_unit_test(gui_can_be_initialized),
        cmocka_unit_test(gui_can_add_and_remove_elements),
        cmocka_unit_test(gui_button_can_be_initialized),
        cmocka_unit_test(gui_button_update_calls_callback_on_click),
        cmocka_unit_test(gui_button_draw_calls_sdlw),
        cmocka_unit_test(gui_text_can_be_initialized),
        cmocka_unit_test(gui_text_update_changes_text),
        cmocka_unit_test(gui_text_draw_calls_sdlw),
    };
    return cmocka_run_group_tests(gui, NULL, NULL);
}
