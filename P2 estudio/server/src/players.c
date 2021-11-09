#include "players.h"


Player* player_init(int socket)
{
    Player* player = malloc(sizeof(Player));
    player -> socket = socket;
    player -> ready = -1;
    return player;
}

void set_player(Player* player, char* name, int farmers, int miners, int engineers,
                int warriors, int gold, int food, int science, int farmers_level, 
                int miners_level, int engineers_level, int attack_level, int defense_level)
{
    player -> ready = 1;
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
}

void collect_resources(Player* player) //Se suman las cantidades de recursos necesarios
{
    player -> gold += player -> miners * player -> miners_level * 2;
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
            player -> farmers +=1; //Se añade un agricultor
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
            printf("No tienes recursos suficientes para crear un minero");
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
            printf("No tienes recursos suficientes para crear un ingeniero");
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
            printf("No tienes recursos suficientes para crear un guerrero");
        }
    }
}

void level_up(Player* player, int option)
{
    int* options[5];
    options[0] = player -> farmers_level;
    options[1] =  player -> miners_level;
    options[2] = player -> engineers_level;
    options[3] = player -> attack_level;
    options[4] = player -> defense_level; 
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

// 2.5.4 atacar
int attack(Player* attacker, Player* defender)
{
    int att_force = attacker->warriors*attacker->attack_level;
    int def_force = defender->warriors*defender->defense_level*2;
    if (att_force > def_force)
    {
        attacker->food = attacker->food + defender->food;
        attacker->gold = attacker->gold + defender->gold;
        attacker->science = attacker->science + defender->science;
        defender->food = 0;
        defender->gold = 0;
        defender->science = 0;
        return 1;
    }else if (att_force < def_force)
    {
        attacker->warriors = floor(attacker->warriors*0.5);
        return 0;
    }
    
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

// 2.5.6 Robar
void steal(Player* player, Player* player_robbed, int resource)
{
    player->science = player->science - 10;
    if (resource == 0)
    {
        player->food = player_robbed->food*0.1;
        player_robbed->food = player_robbed->food*0.9;
    }
    else if (resource == 1)
    {
        player->gold = player_robbed->gold*0.1;
        player_robbed->gold = player_robbed->gold*0.9;
    }
}