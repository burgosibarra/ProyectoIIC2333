#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"
#include "players.h"


int main(int argc, char *argv[]){
    // Se define una IP y un puerto
    char * IP = "0.0.0.0";
    int PORT = 8080;

    // Se crea el servidor y se obtienen los sockets de ambos clientes.
    Player** players_info = prepare_sockets_and_get_clients(IP, PORT);

    // Le enviamos al primer cliente un mensaje de bienvenida
    char * welcome = "Bienvenido Cliente 1!!";
    server_send_message(players_info->socket_c1, 1, welcome);


    int my_attention = 0;
    while (1)
    {
        int socket = players_info[my_attention]->socket;
        // Se obtiene el paquete del cliente my_attention
        int msg_code = server_receive_instruction(socket);

        if (players_info[my_attention]->ready == 0 && msg_code != 0)
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
            for (int player = 0; i < 4; i++)
            {
                if (players_info[player]->ready == 0)
                {
                    // Responder con que no todos están conectaods
                }
            }
            // Que comience el juego
        }

        else if (msg_code == 2)
        {
            // Enviar información al jugador
        }

        else if (msg_code == 3)
        {
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = create_villager(players_info[my_attention], payload[0]);
            if (return_value == 0)
            {
                // Informar del éxito
            }
            else
            {
                // Informar del fracaso
            }
        }

        else if (msg_code == 4)
        {
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = level_up(players_info[my_attention], payload[0]);
            if (return_value == 0)
            {
                // Informar del éxito
            }
            else
            {
                // Informar del fracaso
            }
        }

        else if (msg_code == 5)
        {
            int* payload = server_receive_stdpayload(players_info, my_attention);
            int return_value = attack(players_info[my_attention], players_info[payload[0]]);
            if (return_value == 0)
            {
                // Informar del éxito
            }
            else
            {
                // Informar del fracaso
            }
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
