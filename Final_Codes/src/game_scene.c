#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"

#include <math.h>

Player player; // Player
Player player2; // for multi
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

// Weapon
Weapon weapon_list[2];
Weapon weapon;
int coins_obtained;

static void init(void){

    initEnemy();
    
    map = create_map("Assets/map0.txt", 0);

    player = create_player("Assets/panda2.png", map.Spawn.x, map.Spawn.y, 1, 1000, 4); 
	player2=create_player("Assets/player.png", map.Spawn.x, map.Spawn.y, 2, 1000, 7);

    enemyList = createEnemyList();
    bulletList = createBulletList();

	coins_obtained=0;

    weapon_list[0]=create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10, RIFLE);
    weapon_list[1]=create_weapon("Assets/sniper.png", "Assets/orange_bullet.png", 72, 20, 50, SHOOTER);
	weapon=weapon_list[0]; // the initial weapon
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("Assets/audio/game_bgm.mp3");
}

static Point get_camera() {
	Point Camera=(Point){INF, INF};

	// Camera.x=player.coord.x-SCREEN_W/2;
	// Camera.y=player.coord.y-SCREEN_H/2;
	int player_weight=0, player2_weight=0;
	if (player.status!=PLAYER_DEAD) player_weight=1;
	if (player2.status!=PLAYER_DEAD) player2_weight=1;
	if (player_weight+player2_weight==0) return Camera;
	Camera.x=(player.coord.x*player_weight+player2.coord.x*player2_weight)/(player_weight+player2_weight)-SCREEN_W/2;
	Camera.y=(player.coord.y*player_weight+player2.coord.y*player2_weight)/(player_weight+player2_weight)-SCREEN_H/2;

	return Camera;
}

static void update(void){
    Point Camera=get_camera();

	static int previous_wheel_state=0;
    // Point Camera;

	// Camera.x=player.coord.x-SCREEN_W/2;
	// Camera.y=player.coord.y-SCREEN_H/2;

    updateEnemyList(enemyList, &map, &player, &player2);
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map, player2);

	int change;
	change=update_map(&map, player.coord, player.id, &coins_obtained);
	if (change==TO_SPEED) player.speed_effect_time=120;
	if (change==TO_INVINCIBLE) player.invincible_time=120;
    change=update_map(&map, player2.coord, player2.id, &coins_obtained);
	if (change==TO_SPEED) player2.speed_effect_time=120;
	if (change==TO_INVINCIBLE) player2.invincible_time=120;

    update_player(&player, &map, Camera);
	update_player(&player2, &map, Camera);

	if (previous_wheel_state!=mouseState.z) {
		weapon=weapon_list[(mouseState.z+INF)%INF%2];
	}

	previous_wheel_state=mouseState.z;
}

static void draw(void){
    Point Camera=get_camera();

	// losing and winning scene
	if (player.status==PLAYER_DEAD&&player2.status==PLAYER_DEAD) {
		al_draw_scaled_bitmap(al_load_bitmap("Assets/panda_lose.png"), 0, 0, 64, 64, SCREEN_W/2-128, SCREEN_H/2-128, 256, 256, false);
		return;
	}

	if (coins_obtained>=WINNING_COINS) {
		al_draw_scaled_bitmap(al_load_bitmap("Assets/panda_win.png"), 0, 0, 64, 64, SCREEN_W/2-128, SCREEN_H/2-128, 256, 256, false);
		return;
	}
    
    // Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);
	draw_player(&player2, Camera);
    draw_weapon(&weapon, player.coord, Camera);

	// the coin and heart
	al_draw_scaled_bitmap(al_load_bitmap("Assets/heart.png"), 0, 0, 32, 32, 0, 0, 64, 64, false);
	al_draw_scaled_bitmap(al_load_bitmap("Assets/heart.png"), 0, 0, 32, 32, 128, 0, 64, 64, false);
	al_draw_scaled_bitmap(al_load_bitmap("Assets/coin_icon.png"), 0, 0, 16, 16, 0, 64, 64, 64, false);

	#define string(str) char str[100]
	string(health_text); string(coin_text); string(health2_text);
	snprintf(health_text, 100, "%d", player.health);
	snprintf(health2_text, 100, "%d", player2.health);
	snprintf(coin_text, 100, "%d", coins_obtained);
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        64, 16,
		ALLEGRO_ALIGN_LEFT,
        health_text
    );
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        196, 16,
		ALLEGRO_ALIGN_LEFT,
        health2_text
    );
	al_draw_text(
		P2_FONT, al_map_rgb(255, 255, 255),
		64, 80,
		ALLEGRO_ALIGN_LEFT,
		coin_text
    );
}

static void destroy(void){
    delete_player(&player);
	delete_player(&player2);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}
