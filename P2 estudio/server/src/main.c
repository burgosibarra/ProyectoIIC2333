#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "comunication.h"
#include "conection.h"
#include "players.h"


int main(int argc, char *argv[]){
    // Se define una IP y un puerto
    char * IP = "0.0.0.0";
    int PORT = 8080;

    // Se crea el servidor y se obtienen los sockets de ambos clientes.
    pthread_t thread;
    Lock lock = {0};
    Player** players_info = prepare_sockets_and_get_clients(IP, PORT, &thread, &lock);

    // Le enviamos al primer cliente un mensaje de bienvenida
    //char * welcome = "Bienvenido Cliente 1!!";
    //server_send_message(players_info->socket_c1, 1, welcome);
    char array[1];
    array[0] = 1;
    server_send_stdmessage(players_info, 0, 0, 1, &array[0]);

    int my_attention = 0;
    while (1)
    {
        int socket = players_info[my_attention]->socket;
        server_send_stdmessage(players_info, my_attention, 15, 0, (char*) NULL);
        // Se obtiene el paquete del cliente my_attention
        int msg_code = server_receive_instruction(socket);

        if (players_info[my_attention]->status == 0 && msg_code != 0)
        {
            // Nunca deberíamos llegar aquí
            // ERROR: no se puede enviar una instrucción que no sea de inicializacion
        }

        if (my_attention != 0 && msg_code == 1)
        {
            // Nunca deberíamos llegar aquí
            // ERROR: no se puede iniciar el juego sin ser el líder
        }

        if (msg_code == 0)
        {
            // Recepción
            int return_value = server_receive_setting(players_info, my_attention);

            if (return_value == 0)
            {
                // Enviamos una respuesta
                //server_send_message(sockets_array[my_attention], 1, response);

                // Avanzamos al siguiente cliente
                my_attention = (my_attention + 1) % 4;
            }
            else 
            {
                // Enviamos una respuesta, pedimos nuevamente la información
                //server_send_message(sockets_array[my_attention], 1, response);
            }

        }

        else if (msg_code == 1)
        {
            acquire(&lock);
            int count = 0;
            for (int player = 0; player < 4; player++)
            {
                if (players_info[player]->status == 0)
                {
                    count++;
                    char message[1];
                    message[0] = 1;
                    server_send_stdmessage(players_info, 0, 2, 1, &message[0]);
                    break;
                }
                if (players_info[player]->status == 1)
                {
                    count++;
                }
            }
            if (count > 1)
            {
                // cancelar el thread corriendo
                // Que comience el juego
                pthread_cancel(thread);
                char message[1];
                message[0] = 0;
                server_send_stdmessage(players_info, 0, 2, 1, &message[0]);
            }
            else
            {
                // Solo hay 1 jugador
                char message[1];
                message[0] = 2;
                server_send_stdmessage(players_info, 0, 2, 1, &message[0]);
            }
            release(&lock);

        }

        else if (msg_code == 2)
        {
            char* message = malloc((12 + 4 * 50));
            message[0] = players_info[my_attention] -> farmers;
            message[1] = players_info[my_attention] -> miners;
            message[2] = players_info[my_attention] -> engineers;
            message[3] = players_info[my_attention] -> warriors;
            message[4] = players_info[my_attention] -> gold;
            message[5] = players_info[my_attention] -> food;
            message[6] = players_info[my_attention] -> science;
            message[7] = players_info[my_attention] -> farmers_level;
            message[8] = players_info[my_attention] -> miners_level;
            message[9] = players_info[my_attention] -> engineers_level;
            message[10] = players_info[my_attention] -> attack_level;
            message[11] = players_info[my_attention] -> defense_level;
            server_send_stdmessage(players_info, my_attention, 3 ,1 , &message[0]);
            // Enviar información al jugador
        }

        else if (msg_code == 3)
        {
            char* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = create_villager(players_info[my_attention], (int) payload[0]);
            char message3[1];
            message3[0] = return_value;
            server_send_stdmessage(players_info, my_attention, 3, 1, &message3[0]);
        }
        else if (msg_code == 4)
        {
            char* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = level_up(players_info[my_attention], (int) payload[0]);
            char message4[1];
            message4[0] = return_value;
            server_send_stdmessage(players_info, my_attention, 4, 1, &message4[0]);
        }

        else if (msg_code == 5) //Atacar
        {
            char* message5 = malloc((12 + 4 * 50));
            message5[0] = players_info[my_attention] -> farmers;
            message5[1] = players_info[my_attention] -> miners;
            message5[2] = players_info[my_attention] -> engineers;
            message5[3] = players_info[my_attention] -> warriors;
            message5[4] = players_info[my_attention] -> gold;
            message5[5] = players_info[my_attention] -> food;
            message5[6] = players_info[my_attention] -> science;
            message5[7] = players_info[my_attention] -> farmers_level;
            message5[8] = players_info[my_attention] -> miners_level;
            message5[9] = players_info[my_attention] -> engineers_level;
            message5[10] = players_info[my_attention] -> attack_level;
            message5[11] = players_info[my_attention] -> defense_level;
            for (int i = 0; i < 4; i++)
            {
                if (players_info[i] -> status == 1)
                {
                    for (int j = 0; j < 50; j++)
                    {
                        message5[12 + 50 * i + j] = players_info[i]->name[j];
                    }
                }
            }
            server_send_stdmessage(players_info, my_attention, 2, 12 + 4 * 50, message5);
            free(message5);

            int msg_code = server_receive_instruction(socket);
            if (msg_code != 5)
            {
                printf("Nunca deberiamos llegar aquí");
            }
            char* payload = server_receive_stdpayload(players_info, my_attention);
            
            int defender = ((uint8_t) payload[0]) - 1;
            int return_value;
            if (my_attention == defender || defender >= 4 || defender < 0)
            {
                return_value = 2;
            }
            else
            {
                return_value = attack(players_info[my_attention], players_info[defender]);
            }
            char message5_2[1];
            message5_2[0]= return_value;
            server_send_stdmessage(players_info, my_attention, 5, 1, &message5_2[0]);

            if (return_value != 2)
            {
                char* message5_3 = malloc(51);
                message5_3[0] = return_value;
                for (int j = 0; j < 50; j++)
                {
                    message5_3[1 + j] = players_info[my_attention]->name[j];
                }
                server_send_stdmessage(players_info, defender, 6, 51, message5_3);
                free(message5_3);

                char* message5_4 = malloc(1 + 2 * 50);
                message5_4[0] = return_value;
                for (int j = 0; j < 50; j++)
                {                 
                    message5_4[1 + j] = players_info[my_attention]->name[j];
                    message5_4[51 + j] = players_info[defender]->name[j];
                }

                for (int i = 0; i < 4; i++)
                {
                    if (players_info[i] -> status == 1)
                    {
                        server_send_stdmessage(players_info, i, 7, 1 + 2 * 50, message5_4);
                    }
                }
                free(message5_4);

            }

            free(payload);

        }

        else if (msg_code == 6)
        {
            char* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = spy(players_info[my_attention], players_info[(int) payload[0]]);
            if (return_value == 0)
            {
                // Informar del éxito
            }
            else
            {
                // Informar del fracaso
            }
        }

        else if (msg_code == 7)
        {
            char* payload = server_receive_stdpayload(players_info, my_attention);
            char return_value = (char) steal(players_info[my_attention], players_info[(int) payload[0]], (int) payload[1]);
            server_send_stdmessage(players_info, my_attention, 9, 2, &return_value);

            if (return_value == 0)
            {
                char* message_players = malloc(50); // nombre más un 1 o 0               
                message_players = players_info[(int) payload[0]]->name;
                for (int i = 0; i < 4; i++)
                {
                    if (i != my_attention) // se envía el mensaje 11 a todos menos al ladrón
                    {
                        server_send_stdmessage(players_info, i, 11, 51, &message_players[0]);
                    }
                }
                free(message_players);

                char* robbed_message = malloc(2); // 1 char más 1 int
                robbed_message[0] = (char) payload[1];
                int amount = (int) floor((players_info[payload[0]]->food/0.9)*0.1);
                robbed_message[1] = (uint8_t) amount;
                server_send_stdmessage(players_info, (int) payload[0], 10, 2, &robbed_message[0]);
                free(robbed_message);
            }
        }

        else if (msg_code == 8)
        {
            my_attention = (my_attention + 1) % 4;
        }

        else if (msg_code == 9)
        {

        }

        else if (msg_code == 10)
        {

        }

        else if (msg_code == 255) //Termina tu turno
        {
            my_attention = (my_attention + 1) % 4;
        }

    printf("------------------\n");

    }

    for (int i = 0; i < 4; i++)
    {
        player_destroy(players_info[i]);
    }

    free(players_info);

    return 0;
}
