#ifndef player_h
#define player_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utility.h"
#include "map.h"

typedef enum {
    PLAYER_IDLE, PLAYER_WALKING, PLAYER_DYING, PLAYER_DEAD
} PLAYER_STATUS;

typedef struct _Player{
    Point coord; // coordinate of the player
    int speed; // TODO: CHANGE SPEED SCALED TO THE SIZE OF THE TILES
	int direction;
    int health;
	int id;
    
    ALLEGRO_BITMAP* image;
    uint8_t animation_tick; // For animation
    ALLEGRO_BITMAP* fireball; // for the second player
    
	int fireball_angle; // in degree
	int fireball_range;
	int fireball_damage;
	int fireball_count;
    float knockback_angle;
    uint8_t knockback_CD;

	int speed_effect_time;
	int invincible_time;
	int invincible_color;

    PLAYER_STATUS status;
} Player;

Player create_player(char* path, int row, int col, int id, int health, int speed);
void update_player(Player * player, Map * map, Point cam);
void draw_player(Player * player, Point cam);
void draw_player2(Player* player, Point cam);
void delete_player(Player * player);
void hitPlayer(Player * player, Point enemy_coord, int damage);

#endif /* player_h */
