#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button backButton;
static Slider bgm_slider, sfx_slider;

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
	bgm_slider=slider_create(SCREEN_W/2-200, 100, 400, 10, BGM_VOLUME, 200, "Assets/slider.png", "Assets/yellow_bullet.png", BGM);
	sfx_slider=slider_create(SCREEN_W/2-200, 200, 400, 10, SFX_VOLUME, 200, "Assets/slider.png", "Assets/yellow_bullet.png", SFX);
}

static void update(void) {

    update_button(&backButton);
	if (!sfx_slider.is_activate) update_slider(&bgm_slider);
	if (!bgm_slider.is_activate) update_slider(&sfx_slider);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }

}

static void draw(void) {
    // button
    draw_button(backButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

	// slider 
	draw_slider(bgm_slider);
	draw_slider(sfx_slider);
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        75, 90,
        ALLEGRO_ALIGN_LEFT,
        "BGM"
    );
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        75, 190,
        ALLEGRO_ALIGN_LEFT,
        "SFX"
    );

	#define string(str) char str[5]
	string(BGM_text); string(SFX_text);
	snprintf(BGM_text, 100, "%d", (int)(100*(1.f*(bgm_slider.note_x-bgm_slider.bar_x)/bgm_slider.bar_w)));
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        SCREEN_W-100, 90,
		ALLEGRO_ALIGN_LEFT,
        BGM_text
    );
	snprintf(SFX_text, 100, "%d", (int)(100*(1.f*(sfx_slider.note_x-sfx_slider.bar_x)/sfx_slider.bar_w)));
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        SCREEN_W-100, 190,
		ALLEGRO_ALIGN_LEFT,
        SFX_text
    );
}

static void destroy(void) {
    destroy_button(&backButton);
	destroy_slider(&bgm_slider);
	destroy_slider(&sfx_slider);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
