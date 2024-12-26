#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"

#include <math.h>

static bool isCollision(Player* player, Map* map);

Player create_player(char * path, int row, int col, int id){
    Player player;
    memset(&player, 0, sizeof(player));
    
    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    player.speed = 4;
    player.health = 1000;
	player.id=id;
    
    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
    
    return player;
}

void update_player(Player * player, Map* map){

	if (player->id==2) {
		update_player2(player, map);
		return;
	}

    Point original = player->coord;
    
    // Knockback effect
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }
    
    /*
        Player Movement
        Adjust the movement by player->speed
	*/

	if (keyState[ALLEGRO_KEY_W]) {
		player->coord.y-=player->speed;
		player->direction=UP;
	}
	if (keyState[ALLEGRO_KEY_S]) {
		player->coord.y+=player->speed;
		player->direction=DOWN;
	}
	if (keyState[ALLEGRO_KEY_A]) {
		player->coord.x-=player->speed;
		player->direction=LEFT;
	}
	if (keyState[ALLEGRO_KEY_D]) {
		player->coord.x+=player->speed;
		player->direction=RIGHT;
	}

    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){
		player->coord.y=original.y;
        // player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    if(isCollision(player, map)){
		player->coord.x=original.x;
        // player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }

	if (player->status==PLAYER_DYING||player->status==PLAYER_DEAD) {
		player->coord.x=original.x;
		player->coord.y=original.y;
	}
	else if (player->coord.x==original.x&&player->coord.y==original.y) {
		player->status=PLAYER_IDLE;
	}
	else {
		player->status=PLAYER_WALKING;
	}

	player->animation_tick++;
}

void update_player2 (Player* player, Map* map){ 

    Point original = player->coord;
    
    // Knockback effect
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }

	if (keyState[ALLEGRO_KEY_UP]) {
		player->coord.y-=player->speed;
		player->direction=UP;
	}
	if (keyState[ALLEGRO_KEY_DOWN]) {
		player->coord.y+=player->speed;
		player->direction=DOWN;
	}
	if (keyState[ALLEGRO_KEY_LEFT]) {
		player->coord.x-=player->speed;
		player->direction=LEFT;
	}
	if (keyState[ALLEGRO_KEY_RIGHT]) {
		player->coord.x+=player->speed;
		player->direction=RIGHT;
	}

    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){
		player->coord.y=original.y;
        // player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    if(isCollision(player, map)){
		player->coord.x=original.x;
        // player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }

	if (player->status==PLAYER_DYING||player->status==PLAYER_DEAD) {
		player->coord.x=original.x;
		player->coord.y=original.y;
	}
	else if (player->coord.x==original.x&&player->coord.y==original.y) {
		player->status=PLAYER_IDLE;
	}
	else {
		player->status=PLAYER_WALKING;
	}

	player->animation_tick++;
}

void draw_player(Player * player, Point cam){
	if (player->id==2) {
		draw_player2(player, cam);
		return;
	}

    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    // Change the flag to flip character
	int flag = player->direction == RIGHT ? 1 : 0;

	int offset_x=0;
	int offset_y=0;

	if (player->status==PLAYER_IDLE) {
		offset_x=(player->animation_tick/8)%2*32;
		offset_y=0;
	}
	if (player->status==PLAYER_WALKING) {
		offset_x=(player->animation_tick/8)%4*32;
		offset_y=32;
	}
	if (player->status==PLAYER_DYING) {
		offset_x=(player->animation_tick/8)%4*32;
		offset_y=64;
		if (player->animation_tick>32) player->status=PLAYER_DEAD;
	}
    
    al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
        offset_x, offset_y, 32, 32, // source image x, y, width, height
        dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
        flag // Flip or not
    );

    
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
    
}

void draw_player2(Player * player, Point cam){
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    // Change the flag to flip character
	int flag = player->direction == RIGHT ? 1 : 0;

	int offset_x=0;
	int offset_y=0;

	if (player->status==PLAYER_IDLE) {
		offset_x=0;
		offset_y=0;
	}
	if (player->status==PLAYER_WALKING) {
		offset_x=(player->animation_tick/8)%10*48;
		offset_y=0;
	}
	if (player->status==PLAYER_DYING) {
		offset_x=(player->animation_tick/8)%4*32;
		offset_y=64;
		if (player->animation_tick>32) player->status=PLAYER_DEAD;
	}
    
    al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
        offset_x, offset_y, 48, 48, // source image x, y, width, height
        dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
        flag // Flip or not
    );

    
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
    
}

void delete_player(Player * player){
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map){

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE) / TILE_SIZE >= map->row)
        return true;

	Point player_boundary_top_left=(Point){player->coord.x, player->coord.y};
	Point player_boundary_top_right=(Point){player->coord.x+TILE_SIZE, player->coord.y};
	Point player_boundary_bottom_left=(Point){player->coord.x, player->coord.y+TILE_SIZE};
	Point player_boundary_bottom_right=(Point){player->coord.x+TILE_SIZE, player->coord.y+TILE_SIZE};

	if (!isWalkable(map->map[player_boundary_top_left.y/TILE_SIZE][player_boundary_top_left.x/TILE_SIZE])) return true;
	if (!isWalkable(map->map[player_boundary_top_right.y/TILE_SIZE][player_boundary_top_right.x/TILE_SIZE])) return true;
	if (!isWalkable(map->map[player_boundary_bottom_left.y/TILE_SIZE][player_boundary_bottom_left.x/TILE_SIZE])) return true;
	if (!isWalkable(map->map[player_boundary_bottom_right.y/TILE_SIZE][player_boundary_bottom_right.x/TILE_SIZE])) return true;
    
    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage){
    if(player->knockback_CD == 0){
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);

		player->knockback_angle = angle;
		player->knockback_CD = 32;

		player->health-=damage;
		if(player->health<=0&&player->status!=PLAYER_DEAD&&player->status!=PLAYER_DYING) {
			player->health=0;
			player->status=PLAYER_DYING;
			player->animation_tick=0;
		}

        /*
            [TODO Homework]
            
            Game Logic when the player get hit or die


            player->health = ...
            if(player->health <= 0)

                player->health = 0;
                player->status = ...

                ...
            }
        */
    }
}
