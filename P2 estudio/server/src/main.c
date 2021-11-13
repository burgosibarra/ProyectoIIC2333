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
    char array = 1;
    server_send_message(players_info, 0, 0, 1, &array);

    int my_attention = 0;
    while (1)
    {
        int socket = players_info[my_attention]->socket;
        server_send_message(players_info, my_attention, 15, 0, (char*) NULL);
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
            for (int player = 0; i < 4; i++)
            {
                if (players_info[player]->status == 0)
                {
                    count++;
                    char message[1];
                    message[0] = 1;
                    server_send_stdmessage(players_info, 0, 2, 1, &message);
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
                server_send_stdmessage(players_info, 0, 2, 1, &message);
            }
            else
            {
                // Solo hay 1 jugador
                char message[1];
                message[0] = 2;
                server_send_stdmessage(players_info, 0, 2, 1, &message);
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
            server_send_stdmessage(players_info, my_attention, 3 ,1 , &message);
            // Enviar información al jugador
        }

        else if (msg_code == 3)
        {
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = create_villager(players_info[my_attention], payload[0]);
            char message[1];
            message[0] = return_value;
            server_send_stdmessage(players_info, my_attention, 3, 1, &message);
            free(message);
        }

        else if (msg_code == 4)
        {
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = level_up(players_info[my_attention], payload[0]);
            char message[1];
            message[0] = return_value;
            server_send_stdmessage(players_info, my_attention, 4, 1, &message);
        }

        else if (msg_code == 5) //Atacar
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
            for (int i = 0; i < 4; i++)
            {
                if (players_info[i] -> status == 1)
                {
                    for (int j = 0; j < 50; j++)
                    {
                        message[12 + 50 * i + j] = players_info[i]->name[j];
                    }
                }
            }
            server_send_stdmessage(players_info, my_attention, 2, 12 + 4 * 50, message);
            free(message);

            int msg_code = server_receive_instruction(socket);
            if (msg_code != 5)
            {
                printf("Nunca deberiamos llegar aquí");
            }
            char* payload = server_receive_stdpayload(players_info, my_attention);
            
            int defender = ((uint8_t) payload[0]) - 1;
            int return_value;
            if (attacker == defender || defender >= 4 || defender < 0)
            {
                return_value = 2;
            }
            else
            {
                return_value = attack(players_info[my_attention], players_info[defender]);
            }
            char message = return_value;
            server_send_stdmessage(players_info, my_attention, 5, 1, &message);

            if (return_value != 2)
            {
                char* message = malloc(51);
                message[0] = return_value;
                for (int j = 0; j < 50; j++)
                {
                    message[1 + j] = players_info[my_attention]->name[j];
                }
                server_send_stdmessage(players_info, defender, 6, 51, message);
                free(message);

                char* message = malloc(1 + 2 * 50);
                message[0] = return_value;
                for (int j = 0; j < 50; j++)
                {
                    message[1 + j] = players_info[my_attention]->name[j];
                    message[51 + j] = players_info[defender]->name[j];
                }

                for (int i = 0; i < 4; i++)
                {
                    if (players_info[i] -> status == 1)
                    {
                        server_send_stdmessage(players_info, i, 7, 1 + 2 * 50, message);
                    }
                }
                free(message);

            }

            free(payload);

        }

        else if (msg_code == 6)
        {
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = spy(players_info[my_attention], players_info[payload[0]]);
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
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = steal(players_info[my_attention], players_info[payload[0]], payload[1]);
            if (return_value == 0)
            {
                // Informar del éxito
            }
            else
            {
                // Informar del fracaso
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
