//[Practica 2.2 Ejercicio 2] Servidor horario
//Programa cliente que tendra por parámetros la dirección y el puerto del servidor además del comando

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
	/*
	std::cout << "argc: " << argc << std::endl; //4
	std::cout << "argv[0]: " << argv[0] << std::endl; //./client
	std::cout << "argv[1]: " << argv[1] << std::endl; //127.0.0.1
	std::cout << "argv[2]: " << argv[2] << std::endl; //3000
	std::cout << "argv[3]: " << argv[3] << std::endl; //t
	*/
	
	// ---------------------
	// 1. Inicializar socket
	// ---------------------
	struct addrinfo hints; //socket
	struct addrinfo* res;  //puntero al socket

	memset((void*) &hints,'\0', sizeof(struct addrinfo)); 	//inicializamos socket
	hints.ai_family = AF_UNSPEC;         		      	//ipV4 o ipV6 soportado
	hints.ai_socktype = SOCK_DGRAM; 			//UDP SOCK_DGRAM, TCP SOCK_STREAM 


	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}
	
	//sd es el socket descriptor
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sd,res->ai_addr, res->ai_addrlen); //UDP


	//Creamos variables donde almacenaremos los datos del servidor
	struct sockaddr server;		
	socklen_t server_len = sizeof(struct sockaddr);
	char host[NI_MAXHOST]; //IP
	char serv [NI_MAXSERV]; //Puerto
	//char buffer[2]; //Creamos un contenedor del mensaje lo suficientemente grande. Como es un comando

	//buffer[0] = argv[3];
	//buffer[1] = '\0';	

	//Generamos el mensaje
	sendto(sd, argv[3], 2, 0, (struct sockaddr*) &server, server_len);

}

