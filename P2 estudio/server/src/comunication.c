#include "comunication.h"

//LINKS REFERENCIAS
//https://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
//https://pubs.opengroup.org/onlinepubs/009695399/functions/send.html

int server_receive_instruction(int client_socket){
    // Se obtiene solamente el ID del mensaje
    int instruction = 0;
    recv(client_socket, &instruction, 1, 0);
    return instruction;
}

int* server_receive_stdpayload(Player** players_array, int player){
  
    int socket = players_array[player]->socket;

    // Se obtiene el largo del payload
    int len = 0;
    recv(client_socket, &len, 1, 0);

    // Se obtiene el payload
    int* payload = malloc(len * sizeof(int));
    int received = recv(socket, payload, len, 0);
    // Se retorna
    return payload;
}

void server_receive_setting(Player** players_array, int player)
{

    int socket = players_array[player]->socket;

    // Se obtiene el largo del payload
    int len = 0;
    recv(socket, &len, 1, 0);

    if (len <= 4)
    {
        // ERROR: faltó información en el mensaje
        return 1;
    }

    // Se obtiene el payload
    
    int farmers = 0;
    recv(socket, &farmers, 1, 0);
    int miners = 0;
    recv(socket, &miners, 1, 0);
    int engineers = 0;
    recv(socket, &engineers, 1, 0);
    int warriors = 0;
    recv(socket, &warriors, 1, 0);

    int villagers = farmers + miners + engineers + warriors;

    char * name = malloc(len - 4);
    int received = recv(socket, name, len - 4, 0);

    if (farmers >= 0 && miners >= 0 && engineers >= 0 && warriors >= 0 &&  villagers == 9)
    {
        // Configuramos al jugador
        set_player(players_array[player], name, farmers, miners, engineers, warriors, 0, 0, 0, 1, 1, 1, 1, 1);
        return 0;
    }
    else
    {
        // ERROR: Los datos entregados son inválidos
        return 2;
    }


}

void server_send_stdmessage(Player** players_array, int player, int pkg_id, int size, int* message);
  
    int payloadSize = size + 1;

    // Se arma el paquete
    int msg[1+1+payloadSize];
    msg[0] = pkg_id;
    msg[1] = payloadSize;

    for (int i = 2; i < size + 2; i++)
    {
        msg[i] = message[i];
    }

    // Se envía el paquete
    send(players_array[player]->socket, msg, 2 + payloadSize, 0);
}
