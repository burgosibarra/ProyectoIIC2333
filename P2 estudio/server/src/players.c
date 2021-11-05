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


// 2.5.5 Espiar
void spy(Player* player, Player* player_spied) 
{
    player -> gold -= 30;
    printf("Spying on the player %s \n", player_spied -> name);
    printf("Number of warriors: %i \n", player_spied -> warriors);
    printf("Attack Level: %i \n", player_spied -> attack_level);
    printf("Defense Level: %i \n", player_spied -> defense_level);
}