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

void collect_resources(Player* player) //Se suman las cantidades de recursos necesarios
{
    player-> gold += player -> miners * player -> miners_level * 2;
    player -> food += player -> farmers * player -> farmers_level * 2;
    player -> science += player -> engineers * player -> engineers_level;
}

void create_villager(Player* player, int option)// Se compra un aldeano, falta enviar mensajes de q no se pueden hacer las cosas
{
    if (option == 1) //Se crea un agricultor
    {
        if (player -> food >= 10)
        {
            player -> food -= 10; // Se descuenta recurso actual
            player -> farmer +=1; //Se añade un agricultor
        }
        else
        {
            printf("No tienes comida suficiente para crear un agricultor");
        }
    }
    else if (option == 2) //Se crea minero
    {
        if (player -> food >= 10 && player -> gold >=5)
        {
            player -> food -= 10;
            player -> gold -= 5;
            player -> miners += 1;
        }
        else 
        {
            printf("No tienes recursos suficientes para crear un minero")
        }

    }
    else if (option == 3) //Se crea ingeniero
    {
        if (player -> food >= 20 && player -> gold >= 10)
        {
            player -> food -= 20;
            player -> gold -= 10;
            player -> engineers +=1;
        }
        else 
        {
            printf("No tienes recursos suficientes para crear un ingeniero")
        }
    }
    else if (option == 4) //Se crea guerrero
    {
        if (player -> food >= 10 && player -> gold >= 10)
        {
            player -> food -= 10;
            player -> gold -= 10;
            player -> warriors +=1;
        }
        else 
        {
            printf("No tienes recursos suficientes para crear un guerrero")
        }
    }
}

void level_up(Player* player, int option)
{
    int* options = player -> farmers_level, player -> miners_level, player -> engineers_level, player -> attack_level, player -> defense_level; 
    int upgrade = options[option];
    if (upgrade < 5)
    {
        if (player -> food >= 10* upgrade && player -> gold >= 10 * upgrade && player -> science >= 10* upgrade)
        {
            if (option == 1)
            {
                player -> farmers_level +=1;
                player -> food -= 10 * upgrade;
                player -> gold -= 10 * upgrade;
                player -> science -= 10 * upgrade;
            }
            if (option == 2)
            {
                player -> miners_level +=1;
                player -> food -= 10 * upgrade;
                player -> gold -= 10 * upgrade;
                player -> science -= 10 * upgrade;
            }
            if (option == 3)
            {
                player -> engineers_level +=1;
                player -> food -= 10 * upgrade;
                player -> gold -= 10 * upgrade;
                player -> science -= 10 * upgrade;
            }
            if (option == 4)
            {
                player -> attack_level +=1;
                player -> food -= 10 * upgrade;
                player -> gold -= 10 * upgrade;
                player -> science -= 10 * upgrade;
            }
            if (option == 5)
            {
                player -> defense_level +=1;
                player -> food -= 10 * upgrade;
                player -> gold -= 10 * upgrade;
                player -> science -= 10 * upgrade;
            }
        }
    }
    else 
    {
        printf("No se tienen los recursos para realizar esta acción");
    }
}

void player_destroy(Player* player)
{
    free(player);
}