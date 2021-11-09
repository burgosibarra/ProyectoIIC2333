#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct player
{
    char* name;
    int socket;
    int status;
    int farmers;
    int miners;
    int engineers;
    int warriors;
    int gold;
    int food;
    int science;
    int farmers_level;
    int miners_level;
    int engineers_level;
    int attack_level;
    int defense_level;
};


typedef struct player Player;

Player* player_init(int socket);
void set_player(Player* player, char* name, int farmers, int miners, int engineers, int warriors,
                int gold, int food, int science, int farmers_level, int miners_level,
                int engineers_level, int attack_level, int defense_level);

void spy(Player* player, Player* player_spied);
void level_up(Player* player, int option);
void collect_resources(Player* player);
void create_villager(Player* player, int option);
void player_destroy(Player* player);
int attack(Player* attacker, Player* defender);
void steal(Player* player, Player* player_robbed, int resource);
