#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <strings.h>
#include "conection.h"
#include "comunication.h"

char * get_input(){
  char * response = malloc(50);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}




char* current_villager(int i, int villager) {
  
  if (i == villager)
  {
    return ">";
  } else {
    return " ";
  }
}

void flujo(int server_socket) {
	printf(" ________________________\n");
	printf("|                        |\n");
  printf("| Bienvenid@s a IkaRuz   |\n");
	printf("|     ~₍ ´• ˕ •` ₎~      |\n");
	printf("|________________________|\n\n");
  printf("Ingrese su nombre: \n>> ");
  char * name = get_input();
  int villagers[4] = {0, 0, 0, 0};
  int n_villagers = 0;
  int i = 0;
	printf("Hola %s \n", name);
  while (n_villagers < 9)
  {
		printf("%s, reparte a tus 9 aldeanos\n", name);
    printf(" ________________________  \n");
    printf("|                    |   | \n");
    printf("| Aldean@s restantes | %i | \n| %s Agricultor@s     | %i |\n| %s Miner@s          | %i |\n| %s Ingenier@s       | %i |\n| %s Guerrer@s        | %i |\n", 9 - n_villagers, current_villager(i, 0), villagers[0], current_villager(i, 1), villagers[1], current_villager(i, 2), villagers[2], current_villager(i, 3), villagers[3]);
		printf("|____________________|___| \n\n");
    printf("Si deseas volver un paso atrás inserta X \n\n");

    switch (i)
    {
      case 0:
        printf("¿Cuántos agricultores quieres?\n>> ");
        villagers[0] = getchar() - '0';
        getchar();
        break;
        
      case 1:
        printf("¿Cuántos mineros quieres?  \n>> ");
        villagers[1] = getchar() - '0';
        getchar();
        break;
        
      case 2:
        printf("¿Cuántos ingenieros quieres?  \n>> ");
        villagers[2] = getchar() - '0';
        getchar();
        break;

      case 3:
        printf("¿Cuántos guerreros quieres? \n>> ");
        villagers[3] = getchar() - '0';
        getchar();
        break;
    }
    if (villagers[i] == 88 - 48 || villagers[i] == 120 - 48) 
    {
      villagers[i] = 0;
      
      if (i > 0) {
        n_villagers = n_villagers - villagers[i - 1];
        villagers[i - 1] = 0;
        i--;

      }
    }
    else {
			if (villagers[0] + villagers[1] + villagers[2] + villagers[3] > 9){
				villagers[i] = 0;
				printf("Superaste el número de aldeanos permitido. \n");
			} else {

				n_villagers = villagers[0] + villagers[1] + villagers[2] + villagers[3];

        if (i == 3 && n_villagers < 9)
        {
          printf("Aún quedan %i aldeanos para asignar. \n", 9 - n_villagers);
        }
        else {
				  i++;
        }
    	}
    }
}

char* initial_msg = malloc(54);
for (int i = 0; i < 4; i++)
{
  initial_msg[i] = villagers[i];
}
int finished = 0;
for (int i = 4; i < 54; i++)
{
  if (finished)
  {
    initial_msg[i] = '\0';
  }
  else
  {
    if (name[i-4] == '\0')
    {
      finished = 1;
    }
    initial_msg[i] = name[i-4];
  }
}
client_send_message(server_socket, 0, 54, &initial_msg[0]);
free(initial_msg);

printf(" ________________________  \n");
printf("|                    |   | \n");
printf("| Aldean@s restantes | %i | \n|   Agricultor@s     | %i |\n|   Miner@s          | %i |\n|   Ingenier@s       | %i |\n|   Guerrer@s        | %i |\n", 9 - n_villagers, villagers[0], villagers[1], villagers[2], villagers[3]);
printf("|____________________|___| \n\n");

// printf("¿Qué deseas hacer %s?\n1. Mostrar información \n2. Crear Aldeano\n3. Subir de nivel\n4. Atacar\n5. Espiar\n6. Robar\n7. Pasar\n8. Negociar\n9. Rendirse\n>> ", name);
int option = 0; // getchar() - '0';
// getchar(); 

while (1) {
  
  int msg_server = client_receive_id(server_socket);
  char* payload_server = client_receive_payload(server_socket);

  if (msg_server == 0 && (int) payload_server[0] == 1)
  {
    free(payload_server);
    int started = 0;
    while (started == 0)
    {
      printf("Presiona cualquier tecla para comenzar el juego \n>> ");
      getchar() - '0';
      client_send_message(server_socket, 1, 0, (void*) NULL);
      msg_server = client_receive_id(server_socket);
      if (msg_server != 1)
      {
        printf("Cliente/MAIN157: Nunca deberíamos llegar aquí \n");
      }
      char* msg_0 = client_receive_payload(server_socket);
      if ((int) msg_0[0] == 0)
      {
        started = 1;
      }
      else if ((int) msg_0[0] == 1)
      {
        printf("Alguien no está listo todavía \n");
      }
      else if (((int) msg_0[0] == 2))
      {
        printf("Estas SOLOOO \n");
      }
      free(msg_0);
    }
  }

  if (msg_server == 6)
  {
    int status = payload_server[0];
    if (status == 0)
    {
      printf("%s Te atacó y ganaste\n", &payload_server[1]);
    }else if (status == 1)
    {
      printf("%s Te atacó y perdiste\n", &payload_server[1]);
    }
    free(payload_server);
  }

  if (msg_server == 7)
  {
    int status = payload_server[0];
    if (status == 0)
    {
      printf("%s Atacó a %s y perdió\n", &payload_server[1], &payload_server[51]);
    }else if (status == 1)
    {
      printf("%s Atacó a %s y ganó\n", &payload_server[1], &payload_server[51]);
    }
    free(payload_server);
  }

  if (msg_server == 10)
  {
    int resource = (int) payload_server[0];
    int amount = (int) payload_server[1];
    if (resource == 0)
    {
      printf("The han robado %i de comida \n", amount);
    }else
    {
      printf("The han robado %i de oro \n", amount);
    }
    free(payload_server);
  }
  if (msg_server == 11)
  {
    printf("%s fue robado\n", payload_server);
    free(payload_server);
  }
  if (msg_server == 12)
  {
    int status = payload_server[0];
    if (status == 0)
    {
      printf("%s ha perdido\n", &payload_server[1]);
    }else if (status == 1)
    {
      printf("%s se ha retirado\n", &payload_server[1]);
    }else if (status == 2)
    {
      printf("%s ha ganado\n", &payload_server[1]);
      free(name);
      free(payload_server);
      break;
    }

    free(payload_server);
  }

  if (msg_server == 13)
  {
    free(payload_server);
    printf("HAS GANADO WIIII!!!\n");
  }

  if (msg_server == 15)
  {
    free(payload_server);
    printf("¿Qué deseas hacer %s?\n1. Mostrar información \n2. Crear Aldeano\n3. Subir de nivel\n4. Atacar\n5. Espiar\n6. Robar\n7. Pasar\n8. Negociar\n9. Rendirse\n>> ", name);
    int option = getchar() - '0';
    getchar();

    switch (option)
    {
        case 1: // Mostrar información
          printf("\nInformación de %s\n", name);
          client_send_message(server_socket, 2, 0, (char*) NULL);
          msg_server = client_receive_id(server_socket);
          if (msg_server != 2)
          {
            printf("CLIENT/MAIN212: Nunca deberíamos llegar aquí \n");
          }
          payload_server = client_receive_payload(server_socket);
          printf("Tienes estos aldeanos en cada rol:\n");
          printf("Granjeros: %i\n", (uint8_t) payload_server[0]);
          printf("Mineros: %i\n", (uint8_t) payload_server[1]);
          printf("Ingenieros: %i\n", (uint8_t) payload_server[2]);
          printf("Guerreros: %i\n", (uint8_t) payload_server[3]);
          printf("Tienes estos recursos:\n");
          printf(" > Oro: %i\n", (uint8_t) payload_server[4]);
          printf(" > Comida: %i\n", (uint8_t) payload_server[5]);
          printf(" > Ciencia: %i\n", (uint8_t) payload_server[6]);
          printf("Estos son los niveles de tus unidades: \n");
          printf(" > Granjeros: %i\n", (uint8_t) payload_server[7]);
          printf(" > Mineros: %i\n", (uint8_t) payload_server[8]);
          printf(" > Ingenieros: %i\n", (uint8_t) payload_server[9]);
          printf(" > Ataque: %i\n", (uint8_t) payload_server[10]);
          printf(" > Defensa: %i\n\n", (uint8_t) payload_server[11]);
          free(payload_server);
          break;
          
        case 2: // Crear Aldeano
          printf("\n¿Qué aldeano quieres?\n1. Agricultor/a\n2. Minero/a\n3. Ingeniero/a\n4. Guerrero/a\n>> ");
          int villager = getchar() - '0';
          getchar();
          char message2_1[1];
          message2_1[0] = villager;
          client_send_message(server_socket, 3, 1, &message2_1[0]);
          msg_server = client_receive_id(server_socket);
          if (msg_server != 3)
          {
            printf("CLIENTE/MAIN236: Nunca deberíamos llegar aquí \n");
          }
          payload_server = client_receive_payload(server_socket);
          int create_villager_result = (uint8_t) payload_server[0];

          if (create_villager_result == 0)
          {
            printf("Has creado un aldeano\n");
          }
          else if (create_villager_result == 1)
          {
            printf("No tienes los recursos suficientes\n");
          }
          free(payload_server);
          break;
          
        case 3: // Subir de nivel
          printf("\n¿Qué quieres subir de nivel?\n");
          printf("1. Agricultor@s\n2. Miner@s\n3. Ingenier@s\n4. Ataque\n5. Defensa\n>> ");
          int level = getchar() - '0';
          getchar();
          char message3_1[1];
          message3_1[0] = level;
          client_send_message(server_socket, 4, 1, &message3_1[0]);

          msg_server = client_receive_id(server_socket);
          if (msg_server != 4)
          {
            printf("CLIENTE/MAIN264: Nunca deberíamos llegar aquí \n");
          }
          payload_server = client_receive_payload(server_socket);
          int level_up_result = (uint8_t) payload_server[0];

          if (level_up_result == 0)
          {
            printf("Has subido de nivel con éxito\n");
          }
          else if (level_up_result == 1)
          {
            printf("No tienes los recursos suficientes\n");
          }
          else if (level_up_result == 2)
          {
            printf("Ya eres nivel 5\n");
          }
          free(payload_server);
          break;

        case 4: // Atacar
          client_send_message(server_socket, 5, 0, (char*) NULL);
          msg_server = client_receive_id(server_socket);
          if (msg_server != 2)
          {
            printf("CLIENTE/MAIN289: Nunca deberíamos llegar aquí \n");
          }
        
          payload_server = client_receive_payload(server_socket);

          char* name2 = malloc(50);
          for (int i = 0; i <= 3; i++)
          {
            for (int j = 0; j < 50; j++)
            { 
              name2[j] = payload_server[12 + (50 * i) + j];
            }
            printf("[%i] %s \n", i + 1, name2);
          }
          free(payload_server);

          printf("\n¿A quién quieres atacar? \n>> ");
          int attack = getchar() - '0';
          getchar();
          attack = (char) attack;
          client_send_message(server_socket, 5, 1, (char*) &attack);

          msg_server = client_receive_id(server_socket);
          if (msg_server != 5)
          {
            printf("CLIENTE/MAIN314: Nunca deberíamos llegar aquí \n");
          }
          payload_server = client_receive_payload(server_socket);

          int attack_result = (uint8_t) payload_server[0];
          
          if (attack_result == 1)
          {
            printf("Ganaste!\n");
          }
          else if (attack_result == 0)
          {
            printf("Perdiste!\n");
          }
          else if (attack_result == 2)
          {
            printf("Diste una opción inválida:c\n");
          }
          free(payload_server);
          free(name2);
          break;

        case 5: // Espiar
          client_send_message(server_socket, 6, 0, (char*) NULL);

          msg_server = client_receive_id(server_socket);
          if (msg_server != 2)
          {
            printf("CLIENTE/MAIN289: Nunca deberíamos llegar aquí \n");
          }
        
          payload_server = client_receive_payload(server_socket);

          char* name4 = malloc(50);
          for (int i = 0; i <= 3; i++)
          {
            for (int j = 0; j < 50; j++)
            { 
              name4[j] = payload_server[12 + (50 * i) + j];
            }
            printf("[%i] %s \n", i + 1, name4);
          }
          free(payload_server);

          printf("\n¿A quién quieres espiar? \n>> ");
          int spy = getchar() - '0';
          getchar();

          client_send_message(server_socket, 6, 1, (char*) &spy);

          msg_server = client_receive_id(server_socket);
          if (msg_server != 8)
          {
            printf("CLIENTE/MAIN359: Nunca deberíamos llegar aquí \n");
          }

          payload_server = client_receive_payload(server_socket);

          if (payload_server[0] == 0)
          {
            printf("La aldea a la que estas espiando tiene la siguiente información:\n");        
            printf("Guerreros: %i\n", payload_server[1]);
            printf("Ataque: %i\n", payload_server[2]);
            printf("Defensa: %i\n", payload_server[3]);
          }
          else if (payload_server[0] == 1)
          {
            printf("No tienes los recursos necesarios \n");
          }
          else
          {
            printf("Has dado una opción inválida\n");
          }
          
          free(payload_server);
          free(name4);
          break;

        case 6: // Robar
            client_send_message(server_socket, 7, 0, (char*) NULL);
            msg_server = client_receive_id(server_socket);
            if (msg_server != 2)
            {
              printf("CLIENTE/MAIN289: Nunca deberíamos llegar aquí \n");
            }
          
            payload_server = client_receive_payload(server_socket);

            char* name3 = malloc(50);
            for (int i = 0; i <= 3; i++)
            {
              for (int j = 0; j < 50; j++)
              { 
                name3[j] = payload_server[12 + (50 * i) + j];
              }
              printf("[%i] %s \n", i + 1, name3);
            }
            free(payload_server);

            printf("\n¿A quién le quieres robar? \n>> ");
            int robar = getchar() - '0';
            getchar();

            printf("\n¿Qué quieres robar? \n");
            printf("\n1. Comida");
            printf("\n2. Oro\n");
            int resource = getchar() - '0';
            getchar();
            resource--;
            char steal_message[2];
            steal_message[0] = robar;
            steal_message[1] = resource;
            client_send_message(server_socket, 7, 2, &steal_message[0]);

            msg_server = client_receive_id(server_socket);
            if (msg_server != 9)
            {
              printf("CLIENTE/MAIN359: Nunca deberíamos llegar aquí \n");
            }

            payload_server = client_receive_payload(server_socket);
            int steal_pkg_msg = (int) payload_server[0];
            if (steal_pkg_msg==0)
            {
              printf("Robo realizado\n");
            }else if (steal_pkg_msg==1)
            {
              printf("No tienes suficientes recursos\n");
            }else
            {
              printf("Opción Inválida\n");
            }
            free(payload_server);
            free(name3);
            break;
        
        case 7: // Pasar
          printf("Turno finalizado! \n");
          client_send_message(server_socket, 8, 0, (void*) NULL);
          break;

        case 8: // Negociar
          printf("\n¿Con quién quieres negociar? \n>> ");
          printf("¿Qué quieres negociar? \n>> ");
          break;
        
        case 9: // Rendirse
          printf("Saliste del juego! \n ");
          client_send_message(server_socket, 9, 0, (void*) NULL);
          // Enviar aviso al servidor de que se rindió el juego.
          break;
    }
  }
}

}

int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char* IP;
  int PORT;
  if (argc != 5)
  {
      printf("El uso es ./client -i <ip_address> -p <port>\n");
      return 0;
  }
  else if (strcmp(argv[1], "-i") == 0 && strcmp(argv[3], "-p") == 0)
  {
    IP = argv[2];
    PORT = atoi(argv[4]);
  }
  else if (strcmp(argv[1], "-p") == 0 && strcmp(argv[3], "-i") == 0)
  {
    IP = argv[4];
    PORT = atoi(argv[2]);
  }
  else
  {
    printf("El uso es ./client -i <ip_address> -p <port>\n");
    return 0;
  }
  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  flujo(server_socket);

  // Se cierra el socket
  close(server_socket);
  //free(IP);

  return 0;
}