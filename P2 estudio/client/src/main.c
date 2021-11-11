#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

char * get_input(){
  char * response = malloc(20);
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
        printf("Si deseas volver un paso atrás inserta -1 \n\n");

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

    if (villagers[i] == -1) 
    {
      villagers[i] = 0;
      
      if (i > 0) {
        
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

printf(" ________________________  \n");
printf("|                    |   | \n");
printf("| Aldean@s restantes | %i | \n|   Agricultor@s     | %i |\n|   Miner@s          | %i |\n|   Ingenier@s       | %i |\n|   Guerrer@s        | %i |\n", 9 - n_villagers, villagers[0], villagers[1], villagers[2], villagers[3]);
printf("|____________________|___| \n\n");

// printf("¿Qué deseas hacer %s?\n1. Mostrar información \n2. Crear Aldeano\n3. Subir de nivel\n4. Atacar\n5. Espiar\n6. Robar\n7. Pasar\n8. Negociar\n9. Rendirse\n>> ", name);
int option = 0 // getchar() - '0';
// getchar(); 

while (option != 7 && option != 9) {
  //manejar usuaroios

  // while ()

  printf("¿Qué deseas hacer %s?\n1. Mostrar información \n2. Crear Aldeano\n3. Subir de nivel\n4. Atacar\n5. Espiar\n6. Robar\n7. Pasar\n8. Negociar\n9. Rendirse\n>> ", name);
  int option = getchar() - '0';
  getchar();

	switch (option)
    {
      case 1: // Mostrar información
        printf("\nInformación de %s\n ", name);
        break;
        
      case 2: // Crear Aldeano
				printf("\n¿Qué aldeano quieres?\n1. Agricultor/a\n2. Minero/a\n3. Ingeniero/a\n4. Guerrero/a\n>> ");
        int villager = getchar() - '0';
        getchar();
        break;
        
      case 3: // Subir de nivel
        printf("\n¿Qué quieres subir de nivel?\n");
				printf("1. Agricultor@s\n2. Miner@s\n3. Ingenier@s\n4. Ataque\n5. Defensa\n>> ");
        int level = getchar() - '0';
        getchar(); // OJO ¿Para que es?
        char message[1];
        message[0] = level;
        client_send_message(server_socket, 4, 1, &message);

        int msg_code = client_receive_id(server_socket);
        if (msg_code != 4)
        {
          printf("Nunca deberíamos llegar aquí")
        }
        char* message = client_receive_payload(server_socket);
        int level_up_result = (uint8_t) message[0];

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
        
        free(message);
        break;

      case 4: // Atacar
        client_send_message(server_socket, 5, 0, (char*) NULL);
        int msg_code = client_receive_id(server_socket);
        if (msg_code != 0)
        {
          printf("Nunca deberíamos llegar aquí")
        }
       
        char* message = client_receive_payload(server_socket);

        char name[50];
        for (i = 0; i <= 3; i++)
        {
          for (j = 0; j < 50; j++)
          { 
            name[j] = message[12 + (50 * i) + j];
          }
          printf("[%i] %s \n", i + 1, name);
        }
        free(message);

        printf("\n¿A quién quieres atacar? \n>> ");
        int attack = getchar() - '0';
        getchar();
        attack = (char) attack;
        client_send_message(server_socket, 5, 1, (char*) &attack)

        int msg_code = client_receive_id(server_socket);
        if (msg_code != 5)
        {
          printf("Nunca deberíamos llegar aquí")
        }
        char* message = client_receive_payload(server_socket);

        int attack_result = (uint8_t) message[0];
        
        if (attack_result == 0)
        {
          printf("Ganaste!\n");
        }
        else if (attack_result == 1)
        {
          printf("Perdiste!\n");
        }
        else if (attack_result == 2)
        {
          printf("Diste una opción inválida:c\n");
        }

        free(message);
        

        break;

			case 5: // Espiar
        printf("\n¿A quién quieres espiar? \n>> ");
        int spy = getchar() - '0';
        getchar();
        break;

			case 6: // Robar
        printf("\n¿A quién le quieres robar? \n>> ");
        int robar = getchar() - '0';
        getchar();
        break;
			
			case 7: // Pasar
        printf("\n¿Cuántos guerreros quieres? \n>> ");
        int pass = getchar() - '0';
        getchar();
        break;

			case 8: // Negociar
        printf("\n¿Con quién quieres negociar? \n>> ");
				printf("¿Qué quieres negociar? \n>> ");
        break;
			
			case 9: // Rendirse
        printf("Saliste del juego! \n ");
        // Enviar aviso al servidor de que se rindió el juego.
        break;
    }

	// printf("\n¿Qué deseas hacer %s?\n1. Mostrar información \n2. Crear Aldeano\n3. Subir de nivel\n4. Atacar\n5. Espiar\n6. Robar\n7. Pasar\n8. Negociar\n9. Rendirse\n>> ", name);
	// option = getchar() - '0';
	// getchar(); 
}

}

int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "0.0.0.0";
  int PORT = 8080; //Puede ser 22 u 80

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  flujo(server_socket);

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}