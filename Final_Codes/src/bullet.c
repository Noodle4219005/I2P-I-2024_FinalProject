#include "bullet.h"
#include "map.h"

#include <math.h>

Bullet create_bullet(char* bullet_path, PointFloat coord, float angle, float speed, float damage, BULLET_TYPE bullet_type){
    Bullet bullet;
    bullet.coord = coord;
    bullet.angle = angle;
    bullet.speed = speed;
    bullet.damage = damage;
	bullet.type=bullet_type;
    bullet.image = al_load_bitmap(bullet_path);
    // For better repositioning
    bullet.coord.x += (float)(TILE_SIZE/2) * cos(bullet.angle);
    bullet.coord.y += (float)(TILE_SIZE/2) * sin(bullet.angle);
    
    return bullet;
}

// Return true if the bullet collides, so it will be deleted from the list
bool update_bullet(Bullet * bullet, enemyNode * enemyList, Map * map){
	bullet->coord.x += bullet->speed * cos(bullet->angle);
	bullet->coord.y += bullet->speed * sin(bullet->angle);
    
	int tile_y = (int)(bullet->coord.x / TILE_SIZE);
	int tile_x = (int)(bullet->coord.y / TILE_SIZE);
	if (tile_x<0||tile_x>=map->row||
		tile_y<0||tile_y>=map->col||
		map->map[tile_x][tile_y]==WALL||map->map[tile_x][tile_y]==DOOR_CLOSE) {
		return true;
	}
    
    // Check if the bullet collide with the enemies by simple iterating

	for (; enemyList!=NULL; enemyList=enemyList->next) {
		int enemy_x=enemyList->enemy.coord.x;
		int enemy_y=enemyList->enemy.coord.y;
		if (enemy_x<=bullet->coord.x&&bullet->coord.x<=enemy_x+TILE_SIZE&&
			enemy_y<=bullet->coord.y&&bullet->coord.y<=enemy_y+TILE_SIZE) {
			if (enemyList->enemy.status==ALIVE) {
				hitEnemy(&enemyList->enemy, bullet->damage, bullet->angle);
				return true;
			}
		}
	}

    return false;
}

void draw_bullet(Bullet * bullet, Point camera){
    float scale = TILE_SIZE / 16;
    // al_draw_filled_circle(bullet->coord.x - camera.x, bullet->coord.y - camera.y, scale, al_map_rgb(255, 255, 0));
    al_draw_bitmap(bullet->image, bullet->coord.x - camera.x - 16, bullet->coord.y - camera.y - 16, 0);
}

void destroy_bullet(Bullet * bullet){
    // In case you are using immage asset for the bullet, free it here
    al_destroy_bitmap(bullet->image);
}

/*
    LINKED LIST IMPLEMENTATION FOR BULLET
    THERE ARE :
        (1) CREATE      : INITIALIZING DUMMY HEAD
        (2) INSERT      : INSERT NEW NODE OF BULLET
        (3) UPDATE      : ITERATE EVERYTHING AND UPDATE THE BULLET
        (4) DRAW        : ITERATE EVERYTHING AND DRAW THE BULLET
        (5) DESTROY     : DESTROY THE LINKED LIST
 */

BulletNode * createBulletList(void){
    BulletNode * dummyhead = (BulletNode *) malloc(sizeof(BulletNode));
    dummyhead->next = NULL;
    return dummyhead;
}

void insertBulletList(BulletNode * dummyhead, Bullet bullet){
    BulletNode * tmp = (BulletNode *) malloc(sizeof(BulletNode));
    tmp->bullet = bullet;
    tmp->next = dummyhead->next;
    dummyhead->next = tmp;
}

void updateBulletList(BulletNode * dummyhead, enemyNode * enemyList, Map * map, Player p2){
    BulletNode * cur = dummyhead->next;
    BulletNode * prev = dummyhead;
    
    while(cur != NULL){
        bool shouldDelete = update_bullet(&cur->bullet, enemyList, map);
        if(shouldDelete){
            prev->next = cur->next;
            destroy_bullet(&cur->bullet);
            free(cur);
            cur = prev;
        }
        prev = cur;
        cur = cur->next;
    }

	enemyNode* buf=enemyList;
	if (p2.status==PLAYER_DEAD||p2.status==PLAYER_DYING) return;
	for (int i=0; i<p2.fireball_count; ++i) {
		int fireball_x=p2.coord.x+TILE_SIZE/2+cos(p2.fireball_angle/360.f*6.28+6.28/p2.fireball_count*i)*p2.fireball_range;
		int fireball_y=p2.coord.y+TILE_SIZE/2+sin(p2.fireball_angle/360.f*6.28+6.28/p2.fireball_count*i)*p2.fireball_range;
		for (enemyList=buf; enemyList!=NULL; enemyList=enemyList->next) {
			int enemy_x=enemyList->enemy.coord.x;
			int enemy_y=enemyList->enemy.coord.y;
			if (enemy_x<=fireball_x&&fireball_x<=enemy_x+TILE_SIZE&&
				enemy_y<=fireball_y&&fireball_y<=enemy_y+TILE_SIZE) {
				if (enemyList->enemy.status==ALIVE)
					hitEnemy(&enemyList->enemy, p2.fireball_damage, p2.fireball_angle/360.f*6.28);
			}
		}
	}
}

void drawBulletList(BulletNode * dummyhead, Point camera){
    BulletNode * cur = dummyhead->next;
    
    while(cur != NULL){
        draw_bullet(&cur->bullet, camera);
        cur = cur->next;
    }
}

void destroyBulletList(BulletNode * dummyhead){
	BulletNode* del = dummyhead;
    dummyhead = dummyhead->next;
    free(del); // No Images
    while(dummyhead != NULL){
        BulletNode * del = dummyhead;
        dummyhead = dummyhead->next;
        destroy_bullet(&del->bullet);
        free(del);
    }
}
