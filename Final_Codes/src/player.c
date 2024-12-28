#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"

#include <math.h>

static bool isCollision(Player* player, Map* map, Point cam);

Player create_player(char * path, int row, int col, int id, int health, int speed){
    Player player;
    memset(&player, 0, sizeof(player));
    
    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    player.speed = speed;
    player.health = health;
	player.id=id;
	player.fireball=NULL;
	player.fireball_range=TILE_SIZE*2;
	player.fireball_angle=0;
	player.fireball_damage=2;
	player.fireball_count=4;

    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
	player.fireball=al_load_bitmap("./Assets/fireball.png");
	if (!player.fireball) {
        game_abort("Error Load Bitmap: fireball");
	}
    
    return player;
}

void update_player(Player * player, Map* map, Point cam){
	// if (player->status==PLAYER_DEAD) printf("player id: %d is dead\n", player->id);
    Point original = player->coord;
    
    // Knockback effect
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map, cam)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map, cam)){
            player->coord = next;
        }
    }
    
    /*
        Player Movement
        Adjust the movement by player->speed
	*/

	if (player->id==1) {
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
	}
	else {
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
		player->fireball_angle=(player->fireball_angle+3)%360;
	}

    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map, cam)){
		player->coord.x=original.x;
		player->coord.y=original.y;
        // player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }

	player->coord.x=max(player->coord.x, cam.x);
	player->coord.x=min(player->coord.x, cam.x+SCREEN_W-TILE_SIZE);
	player->coord.y=max(player->coord.y, cam.y);
	player->coord.y=min(player->coord.y, cam.y+SCREEN_H-TILE_SIZE);
    
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
	if (player->status==PLAYER_DEAD) {
		offset_x=4*32;
		offset_y=64;
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
	
	ALLEGRO_COLOR tint=al_map_rgb(255, 255, 255);

	if (player->status==PLAYER_IDLE) {
		offset_x=0;
		offset_y=0;
	}
	if (player->status==PLAYER_WALKING) {
		offset_x=(player->animation_tick/8)%10*48;
		offset_y=0;
	}
	if (player->status==PLAYER_DYING) {
		offset_x=0;
		offset_y=0;
		int offset_c=(player->animation_tick)*255/25;
		offset_c=min(offset_c, 255);
		tint=al_map_rgb(255, 255-offset_c, 255-offset_c);
		if (player->animation_tick>25) player->status=PLAYER_DEAD;
	}
	if (player->status==PLAYER_DEAD) {
		offset_x=0;
		offset_y=0;
		tint=al_map_rgb(255, 0, 0);
	}
    
    al_draw_tinted_scaled_bitmap(player->image, tint,
        offset_x, offset_y, 48, 48, // source image x, y, width, height
        dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
        flag // Flip or not
    );

	if (player->status==PLAYER_DEAD||player->status==PLAYER_DYING) return;

	for (int i=0; i<player->fireball_count; ++i) {
		int fireball_x=dx+TILE_SIZE/2+cos(player->fireball_angle/360.f*6.28+6.28/player->fireball_count*i)*player->fireball_range;
		int fireball_y=dy+TILE_SIZE/2+sin(player->fireball_angle/360.f*6.28+6.28/player->fireball_count*i)*player->fireball_range;
		al_draw_tinted_scaled_bitmap(player->fireball, al_map_rgb(255, 255, 255),
			0, 0, 16, 16, // source image x, y, width, height
			fireball_x-16, fireball_y-16, 32, 32, // destiny x, y, width, height
			false // Flip or not
		);

	}
    
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

static bool isCollision(Player* player, Map* map, Point cam){

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE) / TILE_SIZE >= map->row)
        return true;

	Point player_boundary[4];
	player_boundary[0]=(Point){player->coord.x, player->coord.y};
	player_boundary[1]=(Point){player->coord.x+TILE_SIZE, player->coord.y};
	player_boundary[2]=(Point){player->coord.x, player->coord.y+TILE_SIZE};
	player_boundary[3]=(Point){player->coord.x+TILE_SIZE, player->coord.y+TILE_SIZE};

	for (int i=0; i<4; ++i) {
		if (!isWalkable(map->map[player_boundary[i].y/TILE_SIZE][player_boundary[i].x/TILE_SIZE])) return true;
		if (player_boundary[i].x>cam.x+SCREEN_W||player_boundary[i].x<cam.x) return true;
		if (player_boundary[i].y>cam.y+SCREEN_H||player_boundary[i].y<cam.y) return true;
	}

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
		player->health=max(player->health, 0);
    }
}
