#include "conection.h"

//LINKS REFERENCIAS:
//https://www.man7.org/linux/man-pages/man2/socket.2.html
//https://man7.org/linux/man-pages/man7/socket.7.html
//https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
//https://man7.org/linux/man-pages/man2/setsockopt.2.html
//https://linux.die.net/man/3/htons
//https://linux.die.net/man/3/inet_aton
//https://www.howtogeek.com/225487/what-is-the-difference-between-127.0.0.1-and-0.0.0.0/
//https://www.man7.org/linux/man-pages/man2/bind.2.html
//https://www.man7.org/linux/man-pages/man2/accept.2.html

void* wait_connections(void * ARGS)
{
    struct args* arguments = (struct args*) ARGS;

    struct sockaddr_in client2_addr;
    struct sockaddr_in client3_addr;
    struct sockaddr_in client4_addr;

    int server_socket = arguments->server_socket;
    socklen_t* addr_pointer = arguments->addr_pointer;
    connect_player(arguments->players[1], accept(server_socket, (struct sockaddr *)&client2_addr, addr_pointer));
    connect_player(arguments->players[2], accept(server_socket, (struct sockaddr *)&client3_addr, addr_pointer));
    connect_player(arguments->players[3], accept(server_socket, (struct sockaddr *)&client4_addr, addr_pointer));
}

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port, pthread_t* threads){
    // Se define la estructura para almacenar info del socket del servidor al momento de su creación
    struct sockaddr_in server_addr;

    // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
    // AF_INET -> IPv4 Internet protocols
    // SOCK_STREAM -> Provides sequenced, reliable, two-way, connection-based byte streams.  An out-of-band data transmission mechanism may be supported.
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
    int opt = 1;
    int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    // Se guardan el puerto e IP en la estructura antes definida
    memset(&server_addr, 0, sizeof(server_addr)); //Llena con 0s tantas veces 
    server_addr.sin_family = AF_INET;
    inet_aton(IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    // Se le asigna al socket del servidor un puerto y una IP donde escuchar
    int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Se coloca el socket en modo listening
    int ret3 = listen(server_socket, 1);

    // Se definen las estructuras para almacenar info sobre los sockets de los clientes
    struct sockaddr_in client1_addr;
    socklen_t addr_size = sizeof(client1_addr);


    // OJO esto obliga a recibir 4 jugadores, no menos
    // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
    Player** players = malloc(4 * sizeof(Player*));
    players[0] = player_init();
    players[1] = player_init();
    players[2] = player_init();
    players[3] = player_init();

    // Se aceptan a los primeros 2 clientes que lleguen. "accept" retorna el n° de otro socket asignado para la comunicación
    connect_player(players[0], accept(server_socket, (struct sockaddr *)&client1_addr, &addr_size));
    
    Args arguments;
    arguments.players = players;
    arguments.server_socket = server_socket;
    arguments.addr_pointer = &addr_size;
    
    int rc;
    rc = pthread_create(threads, NULL, wait_connections, (void *) &arguments);
    if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }


    return players;
}
