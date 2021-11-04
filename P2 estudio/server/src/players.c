#include <stdio.h>
#include <stdlib.h>
#include "players.h"


Player* player_init(char* name, int farmers, int miners, int engineers, int warriors,
                    int gold, int food, int science, int farmers_level, int miners_level,
                    int engineers_level, int attack_level, int defense_level)
{
    Player* player = malloc(sizeof(Player));
    player -> name = name;
    player -> farmers = farmers;
    player -> miners = miners;
    player -> engineers = engineers;
    player -> warriors = warriors;
    player -> gold = gold;
    player -> food = food;
    player -> science = science;
    player -> farmers_level = farmers_level;
    player -> miners_level = miners_level;
    player -> engineers_level = engineers_level;
    player -> attack_level = attack_level;
    player -> defense_level = defense_level;
    return player;
}

void player_destroy(Player* player)
{
    free(player);
}