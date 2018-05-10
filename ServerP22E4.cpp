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
	struct addrinfo hints; 									//Declaramos socket
	struct addrinfo* res;  									//Declaramos puntero al socket
	memset((void*) &hints,'\0', sizeof(struct addrinfo)); 	//inicializamos socket
	hints.ai_family = AF_UNSPEC;         		    	  	//ipV4 o ipV6 soportado
	hints.ai_socktype = SOCK_STREAM; 						//UDP SOCK_DGRAM, TCP SOCK_STREAM 

	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	//Gestion de posibles errores
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	//Asignamos un descriptor al socket (sd = Socket descriptor) / (ssd = Server Socket Descriptor)
	int ssd = socket(res->ai_family, res->ai_socktype, 0);

	//Hacemos el Bind
	bind(ssd,res->ai_addr, res->ai_addrlen);

	//Activamos el Listen
	listen(ssd, 5);
	
	//-----------------------------
	// 2. Configuración del cliente
	//-----------------------------
	int csd; 										//client socket descriptor
	struct sockaddr client;							//Donde almacenaremos la info del cliente
	socklen_t client_len = sizeof(struct sockaddr);	//Asignamos tamaño
	char host[NI_MAXHOST];							//IP
	char serv [NI_MAXSERV];							//Puerto
	char buffer[256];								//Mensajee


	//-----------------------------
	// 3. Bucle principal
	//-----------------------------
	while(true){
		//Conexión cliente
  		csd = accept(ssd, (struct sockaddr *) &client, &client_len); //Asignamos descriptor y almacenamos datos del cliente

  		//Procesamos la información
  		getnameinfo((struct sockaddr *) &client, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, 
  			NI_NUMERICHOST|NI_NUMERICSERV);
  
  		std::cout << "Conexión establecida con: " << host << ":" << serv << "\n";
  		//printf("Conexión:%s Puerto:%s\n",host, serv);

  		//Procesado de cada conexión
  		while (true){

  			//recibimos el mensaje y almacenamos su tamaño en "s"
  			ssize_t s = recv(csd, buffer, 255, 0);

	  		//Gestión de la desconexión del cliente
	  		if(s == 0)
	  		{
	  			close(csd);	//Cierra el socket
	  			break;		//Rompe el bucle que gestiona esta conexion
	  		} 

	  		//Mandamos el cliente el eco de su mensaje almacenado en el buffer
	  		send(csd, buffer, s, 0);
  		}

  		std:: cout << "Cliente " << host << ":" << serv << " desconectado\n";
	}
	
	freeaddrinfo(res);
	return 0;
}
