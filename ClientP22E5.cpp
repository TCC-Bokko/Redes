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
	/*
	std::cout << "argc: " << argc << std::endl; //4
	std::cout << "argv[0]: " << argv[0] << std::endl; //./client
	std::cout << "argv[1]: " << argv[1] << std::endl; //127.0.0.1
	std::cout << "argv[2]: " << argv[2] << std::endl; //3000
	std::cout << "argv[3]: " << argv[3] << std::endl; //t
	*/


	/////////////////////////////
	/// TCP
	////////////////////////////
	// ---------------------
	// 1. Inicializar socket
	// ---------------------

	struct addrinfo hints; //socket
	struct addrinfo* res;  //puntero al socket

	memset((void*) &hints,'\0', sizeof(struct addrinfo)); 	//inicializamos socket
	hints.ai_family = AF_UNSPEC;    	     		      	//ipV4 o ipV6 soportado
	hints.ai_flags = AI_PASSIVE;							//Cliente
	hints.ai_socktype = SOCK_STREAM; 						//UDP SOCK_DGRAM, TCP SOCK_STREAM 


	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}
	
	//Obtener descriptor del Socket
	int sd = socket(res->ai_family, res->ai_socktype, 0);

	if(sd == -1){
		std::cout << "Error socket(): " <<  gai_strerror(sd) << std::endl;
		return -1;
	}

	//Connect del cliente
	int sd_src = connect(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);

	if(sd_src != 0){
		std::cout << "Error connect(): " << gai_strerror(sd_src) << std::endl;
		return -1;
	}

	//Creamos un contenedor del mensaje lo suficientemente grande.
	char buffer[256]; 

	//Mandamos mensaje inicial
	send(sd_src, argv[3], 80, 0);
	//Recibimos el eco inicial
	recv(sd_src, buffer, 256, 0);
	//Imprimimos mensaje inicial
	std::cout << buffer << "\n";		

	//Gestión del envío y recepción de mensajes
	while (buffer != "Q") {
		//Actualizamos el mensaje
		std::cin >> buffer;
		
		//Mandamos el mensaje al servidor
		ssize_t bytesSent = send(sd_src, buffer, 256, 0);
		
		//Recibimos el eco y almacenamos su tamaño en "s"
		ssize_t bytesRecv = recv(sd_src, buffer, 256, 0);
		
		//Llamar al cierre si se desconecta el cliente, eso ocurre cuando el núm bytes es 0 (s == 0)
		if (bytesRecv == 0 || buffer[0] == 'Q'){
			std::cout << "Fin de la conexión. Cerrando." << std::endl;	//Notificamos cierre
			shutdown(sd_src, SHUT_RDWR); 								//Cerramos socket
			break; 														//rompemos el bucle
		 }	

		std::cout << buffer << "\n";

	}

	//Limpiamos el puntero
	freeaddrinfo(res);

	return 0;
}
