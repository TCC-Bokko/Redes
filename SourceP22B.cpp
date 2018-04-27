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

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
        //ipV4 o ipV6 soportado
	hints.ai_family = AF_UNSPEC; 
	//Protocolo UDP
	hints.ai_socktype = SOCK_DGRAM; //UDP SOCK_DGRAM, TCP SOCK_STREAM 

	struct addrinfo* res;
	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	//Creamos Socket 
	struct sockaddr client;
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	socklen_t client_len = sizeof(struct sockaddr);
	bind(sd,res->ai_addr, res->ai_addrlen);

	//IP / Puerto
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
	char buffer[80]; //Creamos un contenedor del mensaje lo suficientemente grande.

	//Como se vio en clase ahora esto es un while
	while (1) {
		//Recepcion mensaje
		// int bytes recoge el valor de tamaño del mensaje que devuelve recvfrom
 		int bytes = recvfrom(	sd,				//Socket de conexión
					buffer, 			//Donde almacenamos el mensaje (tamaño mayor al mensaje propio)
					80,				//tam Mensaje (definido arriba)
					0,				//Flags
					(struct sockaddr *) &client,	//Dirección de quien lo ha mandado
				    	 &client_len			//Tamaño info de quien lo ha mandado
				);
  		//Fin mensaje
  	
		//Ponemos el carácter de fin de línea al final del mensaje recogido en el buffer. 
		//(La última posición es la dada por el tamaño del mensaje bytes).
		buffer[bytes]='\0'; 
    
    		//Obtener información
  		getnameinfo(	(struct sockaddr *) &client, 	//Dirección cliente
				client_len, 			//Tamaño cliente
				host, 				//Donde guardamos la dirección
				NI_MAXHOST,			//Tamaño que puede tener la dirección
    	 			serv, 				//Donde guardamos el puerto
				NI_MAXSERV,			//Tamaño que puede tener el puerto
				NI_NUMERICHOST|NI_NUMERICSERV	//Forma de mostrar de los datos
			);

  		//Mostrar información obtenida
  		std::cout << "IP: "<< host << " Port: "<< serv << std::endl;
  		std::cout<< "Mensaje de tamaño ("<< bytes <<"): " << buffer; //buffer contiene en fin de línea

  		//Generamos el mensaje de respuesta
  		size_t answer = getTime(buffer[0], buffer);

  		if (answer == 0) {
  			std::cout << "Good bye!" << std::endl;
  			freeaddrinfo(res);
  			return 0;	
  		} else if(answer != -1) {
  			//Enviamos respuesta
  			sendto(	sd,		//Socket 
				buffer, 	//Mensaje
				80,		//Longitud Mensaje
				0, 		//Flags	
				(struct sockaddr *) &client, //A quien lo enviamos
				client_len	//Longitud direccion
			      );
  		}
	}

	freeaddrinfo(res);
	return 0;
}
