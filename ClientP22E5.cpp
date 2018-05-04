//[Practica 2.2 Ejercicio 5] Cliente del Servidor Eco TCP

//Red
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>

//Tiempo
#include <time.h>
#include <sys/time.h>
#include <features.h>

//setuid
#include <unistd.h>

//io
#include <stdio.h> 
#include <iostream>
#include <string.h>

int main(int argc, char** argv){
	//Debug (comando llamada ./"nombreApp" 127.0.0.1 3000 t
	std::cout << "argc: " << argc << std::endl; //4
	std::cout << "argv[0]: " << argv[0] << std::endl; //./client
	std::cout << "argv[1]: " << argv[1] << std::endl; //127.0.0.1
	std::cout << "argv[2]: " << argv[2] << std::endl; //3000
	std::cout << "argv[3]: " << argv[3] << std::endl; //t
	
	/////////////////////////////
	/// TCP
	////////////////////////////
	// ---------------------
	// 1. Inicializar socket
	// ---------------------

	struct addrinfo hints; //socket
	struct addrinfo* res;  //puntero al socket

	memset((void*) &hints,'\0', sizeof(struct addrinfo)); 	//inicializamos socket
	hints.ai_family = AF_UNSPEC;         		      	//ipV4 o ipV6 soportado
	hints.ai_socktype = SOCK_STREAM; 			//UDP SOCK_DGRAM, TCP SOCK_STREAM 
	
	//Creamos variables donde almacenaremos los datos del servidor
	struct sockaddr src_addr;		
	socklen_t addr_len = sizeof(struct sockaddr);
	char host[NI_MAXHOST]; //IP
	char serv [NI_MAXSERV]; //Puerto


	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}
	
	//Socket
	int sd = socket(res->ai_family, res->ai_socktype, 0);	

	//Limpiamos el puntero
	freeaddrinfo(res);

	//Connect del cliente
	int sd_src = connect(sd, &src_addr, addr_len);

	getnameinfo(&src_addr, addr_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

	std::cout << "Conectando a: " << host << ":" << serv << std::endl;

	//Mandamos mensaje inicial
	send(sd_src, argv[3], 80, 0);		
	
	//Creamos un contenedor del mensaje lo suficientemente grande.
	char buffer[256]; 

	//Gestión del envío y recepción de mensajes
	while (buffer != "Q") {
		
		
		//Recibimos el mensaje y almacenamos su tamaño en "s"
		ssize_t s = recv(sd_src, buffer, 255, 0);
		
		//Llamar al cierre si se desconecta el cliente, eso ocurre cuando el núm bytes es 0 (s == 0)
		if (s == 0 || buffer[0] == 'Q'){
			std::cout << "Fin de la conexión. Cerrando." << std::endl;	//Notificamos cierre
			close(sd_src);							//Cerramos socket
			break; 								//rompemos el bucle
		 }	

		//Actualizamos el mensaje
		std::cin >> buffer;
		
		//Mandamos el mensaje
		send(sd_src, buffer, s, 0);
		

	}


	return 0;
}
