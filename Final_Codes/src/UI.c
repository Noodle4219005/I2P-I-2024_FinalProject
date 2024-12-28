#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <string.h>
#include "UI.h"
#include "utility.h"
#include "game.h"

static bool mouse_in_rect(Point mouse, RecArea rect);

Button button_create(int x, int y, int w, int h, const char* default_image_path, const char* hovered_image_path) {

	Button button;
	memset(&button, 0, sizeof(Button));

	button.default_img = al_load_bitmap(default_image_path);
	button.hovered_img = al_load_bitmap(hovered_image_path);

	if (!button.default_img) {
        game_log("failed loading button image %s", default_image_path);
	}
    if (!button.hovered_img) {
        game_log("failed loading button image %s", hovered_image_path);
    }

	button.x = x;
	button.y = y;
	button.w = w;
	button.h = h;

	button.hovered = false;

	return button;
}

Slider slider_create(int x, int y, int w, int h, float note_proportion, int note_size, const char* bar_iamge_path, const char* note_image_path, int sound_type) {

	Slider slider;
	memset(&slider, 0, sizeof(Slider));

	slider.bar_img=al_load_bitmap(bar_iamge_path);
	slider.note_img=al_load_bitmap(note_image_path);

	if (!slider.bar_img) {
        game_log("failed loading slider image %s", bar_iamge_path);
	}
    if (!slider.note_img) {
        game_log("failed loading slider image %s", note_image_path);
    }

	slider.bar_x=x;
	slider.bar_y=y;
	slider.bar_w=w;
	slider.bar_h=h;
	slider.note_x=x+w*note_proportion;
	slider.note_y=y;
	slider.note_size=note_size;

	slider.is_activate=false;
	slider.hovered = false;

	slider.Sound_Type=sound_type;

	return slider;

}


void draw_button(Button button) {
	ALLEGRO_BITMAP* _img = button.hovered ? button.hovered_img : button.default_img;
	al_draw_scaled_bitmap(
		_img,
		0, 0,
		al_get_bitmap_width(_img), al_get_bitmap_height(_img),
		button.x, button.y,
		button.w, button.h, 0
	);
}

void draw_slider(Slider slider) {
	al_draw_scaled_bitmap(
		slider.bar_img,
		0, 0,
		al_get_bitmap_width(slider.bar_img), al_get_bitmap_height(slider.bar_img),
		slider.bar_x, slider.bar_y,
		slider.bar_w, slider.bar_h, 0
	);

	al_draw_scaled_bitmap(
		slider.note_img,
		0, 0,
		al_get_bitmap_width(slider.note_img), al_get_bitmap_height(slider.note_img),
		slider.note_x-slider.note_size/2, slider.note_y-slider.note_size/2,
		slider.note_size, slider.note_size, 0
	);

}

void update_button(Button* button) {
	Point mouse = { mouseState.x, mouseState.y };
	RecArea rect = { button->x, button->y, button->w, button->h };
	button->hovered=mouse_in_rect(mouse, rect);
}

void update_slider(Slider* slider) {
	Point mouse = { mouseState.x, mouseState.y };
	RecArea rect = { slider->bar_x, slider->bar_y, slider->bar_w, slider->bar_h };

	// whether the mouse is pressed
	if (mouse_in_rect(mouse, rect)&&(mouseState.buttons&1)) slider->is_activate=true;
	else if (slider->is_activate&&(mouseState.buttons&1)) slider->is_activate=true;
	else slider->is_activate=false;

	if (slider->is_activate) {
		slider->note_x=max(slider->bar_x, min(slider->bar_x+slider->bar_w, mouseState.x));
	}
	if (slider->Sound_Type==BGM) BGM_VOLUME=1.f*(slider->note_x-slider->bar_x)/slider->bar_w;
	if (slider->Sound_Type==SFX) SFX_VOLUME=1.f*(slider->note_x-slider->bar_x)/slider->bar_w;
}

void destroy_button(Button* button) {
	al_destroy_bitmap(button->default_img);
	al_destroy_bitmap(button->hovered_img);
}

void destroy_slider(Slider* slider ) {
	al_destroy_bitmap(slider->bar_img);
	al_destroy_bitmap(slider->note_img);

}

static bool mouse_in_rect(Point mouse, RecArea rect) {
	if (rect.x<=mouse.x&&mouse.x<=rect.x+rect.w
		&&rect.y<=mouse.y&&mouse.y<=rect.y+rect.h)
		return true;

	return false;
}
