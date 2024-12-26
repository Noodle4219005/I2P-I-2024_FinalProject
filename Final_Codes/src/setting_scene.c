#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button backButton;
static Slider volumn_slider;

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
	volumn_slider=slider_create(SCREEN_W/2-200, 100, 400, 10, BGM_VOLUME, 200, "Assets/slider.png", "Assets/yellow_bullet.png");
}

static void update(void) {

    update_button(&backButton);
	update_slider(&volumn_slider);

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
	draw_slider(volumn_slider);
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        75, 90,
        ALLEGRO_ALIGN_LEFT,
        "BGM"
    );

	#define string(str) char str[5]
	string(BGM_text); string(SFX_text);
	snprintf(BGM_text, 100, "%d", (int)(100*(1.f*(volumn_slider.note_x-volumn_slider.bar_x)/volumn_slider.bar_w)));
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        SCREEN_W-100, 90,
		ALLEGRO_ALIGN_LEFT,
        BGM_text
    );
}

static void destroy(void) {
    destroy_button(&backButton);
	destroy_slider(&volumn_slider);
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
