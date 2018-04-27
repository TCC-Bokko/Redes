//[Practica 2.2 Ejercicio 2] Servidor horario
//El cliente podrá conectarse y solicitar información mediante ordenes en el mensaje

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

//Método de obtención de fecha 
size_t getTime(char selection, char *buffer){
	//Declaración
	time_t t;
	struct tm * tm;

	//Inicialización
	time (&t);
	tm = localtime (&t);

	//Tratar el mensaje, ver opciones en
	//especificadores http://www.cplusplus.com/reference/ctime/strftime/
	//¿Quizá hacer un switch?
	if(selection == 't') 		return strftime (buffer,80,"%r",tm); 
	else if(selection == 'd')	return strftime (buffer,80,"%F",tm);
	else if(selection == 'q')	return 0;
	else {	
	   std::cout << "Please choose a valid option" << std::endl;
     	   return -1;
	} 
}


int main(int argc, char** argv){
	/////////////////////////////
	/// UDP
	////////////////////////////

	struct addrinfo hints; //socket
	struct addrinfo* res;  //puntero al socket

	// ---------------------
	// 1. Inicializar socket
	// ---------------------
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

	//Creamos variables donde almacenaremos los datos del cliente
	struct sockaddr client;		
	socklen_t client_len = sizeof(struct sockaddr);
	char host[NI_MAXHOST]; //IP
	char serv [NI_MAXSERV]; //Puerto
	char buffer[80]; //Creamos un contenedor del mensaje lo suficientemente grande.

	//Gestión del envío y recepción de mensajes
	while (1) {
		//Recepcion mensaje
		// int bytes recoge el valor de tamaño del mensaje que devuelve recvfrom
 		int bytes = recvfrom(	sd,				//Descriptor del socket de conexión
					buffer, 			//Donde almacenamos el mensaje (de tamaño mayor al mensaje recibido)
					80,				//Tamaño del contenedor del mensaje (definido arriba)
					0,				//Flags
					(struct sockaddr *) &client,	//Donde almacenaremos la dirección del cliente
				    	 &client_len			//Tamaño del contenedor de la dirección del cliente
				);
  	
		//Ponemos el carácter de fin de línea al final del mensaje recogido en el buffer. 
		//(La última posición es la dada por el tamaño del mensaje bytes).
		buffer[bytes]='\0'; 
    
    		//Obtener información del cliente
  		getnameinfo(	(struct sockaddr *) &client, 	//Dirección cliente
				client_len, 			//Tamaño cliente
				host, 				//Donde guardamos la dirección
				NI_MAXHOST,			//Tamaño que puede tener la dirección
    	 			serv, 				//Donde guardamos el puerto
				NI_MAXSERV,			//Tamaño que puede tener el puerto
				NI_NUMERICHOST|NI_NUMERICSERV	//Forma de mostrar de los datos
			);

  		//Mostrar información obtenida
  		std::cout << "Conexion desde IP: "<< host << " Puerto: "<< serv << std::endl;
  		std::cout<< "Mensaje de tamaño ("<< bytes <<"): " << buffer; //buffer contiene en fin de línea

  		//Generamos el mensaje de respuesta
		
		//Mensaje eco
		//sendto(sd, buffer, bytes, 0, (struct sockaddr*) &cliente, cliente_len);
		
		//Mensaje servidor de hora
  		size_t answer = getTime(buffer[0], buffer); //la primera posicion del buffer deberia contener la orden del cliente.

  		if (answer == 0) {
  			std::cout << "Good bye!" << std::endl;
  			freeaddrinfo(res);
  			return 0;	
  		} else if(answer != -1) {
  			//Enviamos respuesta
  			sendto(	sd,		//Descriptor del socket 
				buffer, 	//Mensaje
				80,		//Longitud Mensaje
				0, 		//Flags	
				(struct sockaddr *) &client, //A quien lo enviamos
				client_len	//Longitud direccion
			      );
  		}
	}
	
	//Limpiamos el puntero
	freeaddrinfo(res);

	return 0;
}
