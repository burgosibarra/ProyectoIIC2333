
struct player
{
    char* name;
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
Player* player_init(char* name, int farmers, int miners, int engineers, int warriors,
                    int gold, int food, int science, int farmers_level, int miners_level,
                    int engineers_level, int attack_level, int defense_level);


// 2.5.5 Espiar
void spy(Player* player, Player* player_spied);