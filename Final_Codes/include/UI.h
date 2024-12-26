#ifndef UI_H
#define UI_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <string.h>
#include "utility.h"

typedef struct _Button {
	int x, y, w, h;

	ALLEGRO_BITMAP* default_img;
	ALLEGRO_BITMAP* hovered_img;

	bool hovered;
}Button;

typedef struct _Slider {
	int bar_x, bar_y, bar_w, bar_h;
	int note_x, note_y;
	int note_size;

	ALLEGRO_BITMAP* bar_img;
	ALLEGRO_BITMAP* note_img;

	bool is_activate;
	bool hovered;

}Slider;

Button button_create(int x, int y, int w, int h, const char* default_image_path, const char* hovered_image_path);
void draw_button(Button button);
void update_button(Button* button);
void destroy_button(Button* button);

Slider slider_create(int x, int y, int w, int h, float note_propotion, int note_size, const char* bar_iamge_path, const char* note_image_path);
void draw_slider(Slider slider);
void update_slider(Slider* slider);
void destroy_slider(Slider* slider);

#endif