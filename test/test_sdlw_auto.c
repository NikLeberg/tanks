/**
 * @file test_sdlw_auto.c
 * @author Stoll Simon (stols4@bfh.ch)
 * @brief Automatische Tests für sdlWrapper-Modul
 * @version 0.1
 * @date 2021-04-09
 * 
 * @copyright Copyright (c) 2021 Stoll Simon
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

#include "sdlWrapper.h"
#include "error.h"


/*
 * Tests
 * 
 */

/**
 * @brief Überprüft die Initialisierung und die Erkennung ob SDL nicht initialisiert wurde.
 * 
 * @param state unbenutzt
 */
static void test_init_checks(void **state) {
    (void)state;
    // Windowsize zu gross oder zu klein
    assert_int_equal(SDLW_Init(0, 500), ERR_PARAMETER);
    assert_int_equal(SDLW_Init(2001, 500), ERR_PARAMETER);
    assert_int_equal(SDLW_Init(500, 0), ERR_PARAMETER);
    assert_int_equal(SDLW_Init(500, 2001), ERR_PARAMETER);
    // Init ok
    assert_int_equal(SDLW_Init(500, 500), ERR_OK);
    // Es kann nur einmal initialisiert werden
    assert_int_equal(SDLW_Init(500, 500), ERR_FAIL);
    assert_int_equal(SDLW_Render(), ERR_OK);
    // Check SDLW_Quit()
    assert_int_equal(SDLW_Quit(), ERR_OK);
    // Überprüfen ob erkannt wird, ob sdlw initialisiert ist
    assert_int_equal(SDLW_Clear((SDL_Color){0, 0, 0, 0}), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/test_sldw.cfg"), ERR_FAIL);
    int *v1;
    assert_int_equal(SDLW_GetResource("key", RESOURCETYPE_TEXTURE, (void **)&v1), ERR_FAIL);
    assert_int_equal(SDLW_DrawFilledRect((SDL_Rect){0, 0, 0, 0}, (SDL_Color){0, 0, 0, 0}), ERR_FAIL);
    assert_int_equal(SDLW_DrawTexture((sprite_t){0}), ERR_FAIL);
    assert_int_equal(SDLW_CreateTextTexture(NULL, NULL, (SDL_Color){0, 0, 0, 0}, NULL), ERR_FAIL);
    assert_int_equal(SDLW_Render(), ERR_FAIL);
    assert_int_equal(SDLW_PlayMusic("1khz"), ERR_FAIL);
    assert_int_equal(SDLW_PlaySoundEffect("1khz"), ERR_FAIL);
}

/**
 * @brief Überprüft ob erkannt wird, falls config Dateien korrekt sind oder nicht.
 * 
 * @param state unbenutzt
 */
static void test_config_files(void **state) {
    (void)state;
    SDLW_Init(500, 500);
    assert_int_equal(SDLW_LoadResources(NULL), ERR_NULLPARAMETER); // Kein string übergeben
    // Fehlerhafte Konfigurationsdateien. Fehlerbeschrieb sind in den einzelnen Konfigdateien beschrieben
    assert_int_equal(SDLW_LoadResources("assets/test/e_empty.cfg"), ERR_PARAMETER);
    assert_int_equal(SDLW_LoadResources("assets/test/e_notype.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_unknowntype.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_config.cfg"), ERR_PARAMETER);
    assert_int_equal(SDLW_LoadResources("assets/test/e_texture1.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_texture2.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_texture3.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_sprite.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_multisprite1.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_multisprite2.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_font1.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_font2.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_sound1.cfg"), ERR_FAIL);
    assert_int_equal(SDLW_LoadResources("assets/test/e_sound2.cfg"), ERR_FAIL);

    // Laden der Testdateien
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);
    SDLW_Quit();
}

/**
 * @brief Laden von Texturen funktioniert, und sie kann gezeichnet werden.
 * 
 * @param state unbenutzt
 */
static void test_sdlw_getTexture_and_draw(void **state) {
    (void)state;
    SDLW_Init(500, 500);
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);

    SDL_Texture *texture;
    // Null checks
    assert_int_equal(SDLW_GetResource(NULL, RESOURCETYPE_TEXTURE, (void **)&texture), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_TEXTURE, (void **)NULL), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_GetTexture(NULL, &texture), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_GetTexture("nswo", NULL), ERR_NULLPARAMETER);
    // Fehlerhafte id
    assert_int_equal(SDLW_GetResource("nonsense", RESOURCETYPE_TEXTURE, (void **)&texture), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("peep", RESOURCETYPE_TEXTURE, (void **)&texture), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_FONT, (void **)&texture), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_NONE, (void **)&texture), ERR_FAIL);
    assert_int_equal(SDLW_GetTexture("nonsense", &texture), ERR_FAIL);
    assert_int_equal(SDLW_GetTexture("peep", &texture), ERR_FAIL);
    // Fehlerfrei
    assert_int_equal(SDLW_GetTexture("nswo", &texture), ERR_OK);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_TEXTURE, (void **)&texture), ERR_OK);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_ANY, (void **)&texture), ERR_OK);

    assert_int_equal(SDLW_DrawTexture((sprite_t){0}), ERR_NULLPARAMETER);       // Null Check
    assert_int_equal(SDLW_DrawTexture((sprite_t){.texture = texture}), ERR_OK); // Fehlerfrei

    SDLW_Quit();
}

/**
 * @brief Laden von Schriftarten funktioniert, und ein Text kann damit erstellt werden.
 * 
 * @param state unbenutzt
 */
static void test_sdlw_getFont_and_create(void **state) {
    (void)state;
    SDLW_Init(500, 500);
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);

    TTF_Font *font;
    SDL_Texture *texture;

    // Fehlerhafte id
    assert_int_equal(SDLW_GetResource("nonsense", RESOURCETYPE_FONT, (void **)&font), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_FONT, (void **)&font), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("osans25", RESOURCETYPE_TEXTURE, (void **)&font), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("osans25", RESOURCETYPE_TEXTURE, (void **)&font), ERR_FAIL);
    // Fehlerfrei
    assert_int_equal(SDLW_GetResource("osans25", RESOURCETYPE_FONT, (void **)&font), ERR_OK);
    assert_int_equal(SDLW_GetResource("osans25", RESOURCETYPE_ANY, (void **)&font), ERR_OK);
    // Null Check
    assert_int_equal(SDLW_CreateTextTexture(NULL, "osans25", (SDL_Color){0}, &texture), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_CreateTextTexture("Test", NULL, (SDL_Color){0}, &texture), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_CreateTextTexture("Test", "osans25", (SDL_Color){0}, NULL), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_CreateTextTexture("Test", "nonsense", (SDL_Color){0}, &texture), ERR_PARAMETER);
    // OK
    assert_int_equal(SDLW_CreateTextTexture("Test", "osans25", (SDL_Color){0}, &texture), ERR_OK);

    SDLW_Quit();
}

/**
 * @brief Laden von Sounds funktioniert, und sie können abgespielt werden.
 * 
 * @param state unbenutzt
 */
static void test_sdlw_getSound_and_play(void **state) {
    (void)state;
    SDLW_Init(500, 500);
    assert_int_equal(SDLW_LoadResources("assets/test/config.cfg"), ERR_OK);

    Mix_Music *music;

    // Fehlerhafte id
    assert_int_equal(SDLW_GetResource("nonsense", RESOURCETYPE_SOUND_MUSIC, (void **)&music), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_SOUND_MUSIC, (void **)&music), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("jingle", RESOURCETYPE_TEXTURE, (void **)&music), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("jingle", RESOURCETYPE_TEXTURE, (void **)&music), ERR_FAIL);
    // Fehlerfrei
    assert_int_equal(SDLW_GetResource("jingle", RESOURCETYPE_SOUND_MUSIC, (void **)&music), ERR_OK);
    assert_int_equal(SDLW_GetResource("jingle", RESOURCETYPE_SOUND_MUSIC, (void **)&music), ERR_OK);

    Mix_Chunk *chunk;

    // Fehlerhafte id
    assert_int_equal(SDLW_GetResource("nonsense", RESOURCETYPE_SOUND_EFFECT, (void **)&chunk), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("nswo", RESOURCETYPE_SOUND_EFFECT, (void **)&chunk), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("peep", RESOURCETYPE_TEXTURE, (void **)&chunk), ERR_FAIL);
    assert_int_equal(SDLW_GetResource("peep", RESOURCETYPE_TEXTURE, (void **)&chunk), ERR_FAIL);
    // Fehlerfrei
    assert_int_equal(SDLW_GetResource("peep", RESOURCETYPE_SOUND_EFFECT, (void **)&chunk), ERR_OK);
    assert_int_equal(SDLW_GetResource("peep", RESOURCETYPE_SOUND_EFFECT, (void **)&chunk), ERR_OK);

    // Null Check
    assert_int_equal(SDLW_PlayMusic(NULL), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_PlayMusic("nonsense"), ERR_FAIL);
    assert_int_equal(SDLW_PlaySoundEffect(NULL), ERR_NULLPARAMETER);
    assert_int_equal(SDLW_PlaySoundEffect("nonsense"), ERR_FAIL);

    // OK
    assert_int_equal(SDLW_PlayMusic("jingle"), ERR_OK);
    assert_int_equal(SDLW_PlaySoundEffect("peep"), ERR_OK);

    SDLW_Quit();
}

/**
 * @brief Testprogramm
 * 
 * @return int Anzahl fehlgeschlagener Tests
 */
int main(void) {
    const struct CMUnitTest sdlwAutoTest[] = {
        cmocka_unit_test(test_init_checks),
        cmocka_unit_test(test_config_files),
        cmocka_unit_test(test_sdlw_getTexture_and_draw),
        cmocka_unit_test(test_sdlw_getFont_and_create),
        cmocka_unit_test(test_sdlw_getSound_and_play)
    };
    return cmocka_run_group_tests(sdlwAutoTest, NULL, NULL);
}
