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
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

// Weapon
Weapon weapon; 
int coins_obtained;

static void init(void){

    initEnemy();
    
    map = create_map("Assets/map0.txt", 0);

    player = create_player("Assets/panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    bulletList = createBulletList();

	coins_obtained=0;

    weapon = create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10);
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("Assets/audio/game_bgm.mp3");
}

static void update(void){
    /*
        [TODO Homework]
        
        Change the scene if winning/losing to win/lose scene
    */

    Point Camera;

	Camera.x=player.coord.x-SCREEN_W/2;
	Camera.y=player.coord.y-SCREEN_H/2;

    update_player(&player, &map);
    updateEnemyList(enemyList, &map, &player);
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map);
    update_map(&map, player.coord, &coins_obtained);

}

static void draw(void){
    Point Camera;

	Camera.x=player.coord.x-SCREEN_W/2;
	Camera.y=player.coord.y-SCREEN_H/2;

	if (player.status==PLAYER_DEAD) {
		al_draw_scaled_bitmap(al_load_bitmap("Assets/panda_lose.png"), 0, 0, 64, 64, SCREEN_W/2-128, SCREEN_H/2-128, 256, 256, false);
		return;
	}

	if (coins_obtained>=3) {
		al_draw_scaled_bitmap(al_load_bitmap("Assets/panda_win.png"), 0, 0, 64, 64, SCREEN_W/2-128, SCREEN_H/2-128, 256, 256, false);
		return;
	}
    
    // Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);
    draw_weapon(&weapon, player.coord, Camera);

	al_draw_scaled_bitmap(al_load_bitmap("Assets/heart.png"), 0, 0, 32, 32, 0, 0, 64, 64, false);
	al_draw_scaled_bitmap(al_load_bitmap("Assets/coin_icon.png"), 0, 0, 16, 16, 0, 64, 64, 64, false);

	#define string(str) char str[100]
	string(health_text); string(coin_text);
	snprintf(health_text, 100, "%d", player.health);
	snprintf(coin_text, 100, "%d", coins_obtained);
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        64, 16,
		ALLEGRO_ALIGN_LEFT,
        health_text
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
