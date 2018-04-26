#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <features.h>


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
	if(selection == 't') 		return strftime (buffer,26,"%r",tm); 
	else if(selection == 'd')	return strftime (buffer,26,"%F",tm);
	else if(selection == 'q')	return 0;
	else {	
	   std::cout << "Please choose a valid option" << std::endl;
							return -1;
	} 
}


int main(int argc, char** argv){

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; //ipV4 o ipV6 soportado
	hints.ai_socktype = SOCK_DGRAM; //UDP

	struct addrinfo* res;
	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	//Socket
	struct sockaddr client;
	int sd = socket(res->ai_family, res->ai_socktype, 0);
	socklen_t client_len = sizeof(struct sockaddr);
	bind(sd,res->ai_addr, res->ai_addrlen);

	//IP / Puerto
	char host[NI_MAXHOST];
	char serv [NI_MAXSERV];
	char buffer[26];

	//Como se vio en clase ahora esto es un while
	while (1) {
		//Recepcion mensaje
 		int bytes = recvfrom(sd, buffer, 26, 0, (struct sockaddr *) &client,
    	 &client_len);
  		//Fin mensaje
  		buffer[bytes]='\0'; 
    
    	//Obtener información
  		getnameinfo((struct sockaddr *) &client, client_len, host, NI_MAXHOST,
    	  serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

  		//Mostrar información
  		std::cout << "IP: "<< host << " Port: "<< serv << std::endl;
  		std::cout<< "\tMessage ("<< bytes <<"): " << buffer << std::endl;

  		//Generamos el mensaje de respuesta
  		size_t answer = getTime(buffer[0], buffer);

  		if (answer == 0) {
  			std::cout << "Good bye!" << std::endl;
  			freeaddrinfo(res);
  			return 0;	
  		} else if(answer != -1) {
  			//Enviamos respuesta
  			sendto(sd, buffer, tam, 0, (struct sockaddr *) &client, client_len);
  		}
	}

	freeaddrinfo(res);
	return 0;
}