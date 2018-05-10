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
#include <iostream>file:///home/lab_redes/clons
file:///home/lab_redes/images
file:///home/lab_redes/clean
file:///home/lab_redes/README
file:///home/lab_redes/vlab.sh
file:///home/lab_redes/vregenerate

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
	
	// Inicializar socket
	struct addrinfo hints; //socket //V
	struct addrinfo* res;  //puntero al socket

	memset((void*) &hints,'\0', sizeof(struct addrinfo)); 	//inicializamos socket
	hints.ai_family = AF_UNSPEC;         		      	//ipV4 o ipV6 soportado
	hints.ai_socktype = SOCK_DGRAM; 			//UDP SOCK_DGRAM, TCP SOCK_STREAM 


	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	//Gestión de errores
	if(rc != 0){
		std::cout << "Error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}
	
	//Asignamos un descriptor al socket (sd = Socket descriptor)
	int sd = socket(res->ai_family, res->ai_socktype, 0);//V

	//Mensaje
	char buffer[80];

	//Enviamos el mensaje recibido por argumento
	size_t bytesSent = sendto(sd, argv[3], 2, 0, (struct sockaddr *) res->ai_addr, res->ai_addrlen);

	//Recibimos
	int bytesRecv = recvfrom(sd, buffer, 80, 0, (struct sockaddr *) res->ai_addr, &res->ai_addrlen);

	//Ponemos final de línea al mensaje recibido
	buffer[bytesRecv] = '\0';

	//Escribimos el mensaje recibido
	std::cout << buffer << "\n";

		freeaddrinfo(res);
	return 0;
}


