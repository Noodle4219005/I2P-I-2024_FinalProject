#ifndef map_h
#define map_h

#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utility.h"

// IF YOU WANT TO INCREASE YOUR MAP SIZE OR ENEMY SIZE MAKE SURE TO INCREASE DEFINE VARIABLE BELOW IF IT EXCEED THE MAXIMUM
#define MAX_MAP_ROW 100
#define MAX_MAP_COL 100
#define MAX_ENEMY_SPAWN 100

/*
    Map Tiles type
    Hint : Some type might need a background, to configure it try to modify "get_map_offset(Map * map)"
*/
typedef enum _BLOCK_TYPE{
    FLOOR,
    WALL,
    DOOR_CLOSE,
	DOOR_OPEN,
    HOLE,
    COIN,
	BUTTON,
	INVINCIBLE,
	SPEEDUP,
    NOTHING
} BLOCK_TYPE;

typedef enum _COIN_STATUS {
    APPEAR,
    DISAPPEARING,
    DISAPPEAR
} COIN_STATUS;

// Map Struct
typedef struct Map_{
    uint8_t ** map; // Map array
	uint8_t** coin_status;
	uint8_t** coin_disappearing_animation_tick;
    Point ** offset_assets; // Assets

	int coin_animation_tick;
	int coin_counter;

	int button_animation_tick;
	int door_animation_tick;

	bool is_button_pressed;
    
    int row, col;
    
    // Map assets
    ALLEGRO_BITMAP* assets;
    ALLEGRO_BITMAP* coin_assets;
    ALLEGRO_BITMAP* button_assets;
    ALLEGRO_BITMAP* door_assets;

    // Coin Properties
    ALLEGRO_SAMPLE* coin_audio;
    
    // Spawn Coordinate
    Point Spawn;
    Point EnemySpawn[MAX_ENEMY_SPAWN];
    char EnemyCode[MAX_ENEMY_SPAWN];
    uint8_t EnemySpawnSize;

} Map;

enum _CHANGE_STATUS {
	CHANGE_NOTHING,
	TO_SPEED,
	TO_INVINCIBLE
} CHANGE_STATUS;

/*
    MAP FUNCTION
    Feel free to add more if you have some idea or interaction with the map
 */
Map create_map(char * path, uint8_t type); // Create a map based on given file path
void draw_map(Map * map, Point cam); // Draw the map
uint8_t update_map(Map* map, Point player_coord, int player_id, int* total_coins);
void destroy_map(Map * map); // Destroy map

bool isWalkable(BLOCK_TYPE block);

#endif /* map_h */
