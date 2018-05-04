//[Practica 2.2 Ejercicio 4] Servidor Eco TCP

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
	
	//Creamos variables donde almacenaremos los datos del cliente
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

	//Hacemos el bind
	bind(sd,res->ai_addr, res->ai_addrlen);

	//Llamamos al listen (TCP)
	listen(sd, 15);
	
	//Limpiamos el puntero
	freeaddrinfo(res);

	int sd_src = accept(sd, &src_addr, &addr_len);

	getnameinfo(&src_addr, addr_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

	std::cout << "Conexión: " << host << ":" << serv << std::endl;

	//Gestión del envío y recepción de mensajes
	while (true) {
		char buffer[256]; //Creamos un contenedor del mensaje lo suficientemente grande.
		
		//Recibimos el mensaje y almacenamos su tamaño en "s"
		ssize_t s = recv(sd_src, buffer, 255, 0);
		
		//Llamar al cierre si se desconecta el cliente, eso ocurre cuando el núm bytes es 0 (s == 0)
		if (s == 0){
			std::cout << "Fin de la conexión. Cerrando." << std::endl;	//Notificamos cierre
			break; 								//rompemos el bucle
		 }		

  		//Mostrar información obtenida
  		std::cout << "Mensaje:" << buffer << std::endl; //buffer contiene en fin de línea
		
		//Devolvemos el mensaje (eco)
		send(sd_src, buffer, s, 0);
		
		

	}


	return 0;
}
