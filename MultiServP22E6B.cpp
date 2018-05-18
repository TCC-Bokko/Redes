//[Practica 2.2 Ejercicio 6] Servidor horario UDP Multithreat
//El cliente podrá conectarse y solicitar información mediante ordenes en el mensaje
//Cada cliente tendra su propio threat

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

//Threading
#include <pthread.h>
#define NUM_THREADS 5
//revisar https://www.cs.nmsu.edu/~jcook/Tools/pthreads/library.html
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html


//Método de obtención de fecha 
size_t getTime(char selection, char *buffer){
	//Declaración
	time_t t;
	struct tm * tm;
	//std::string message;

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
	   /*
	   message = "Please choose a valid option.";
	   buffer = message.toCharArray();
	   std::cout << "Client made a bad request." << std::endl;
	   */
     	   return -1;
	} 
}


//Clase propia para los hilos que gestionarán cada mensaje:
class HiloMensaje{
    public:
        //Constructora & Destructora
        HiloMensaje(int _sd): sd(_sd){};
        ~HiloMensaje() { close(sd); }
        
        //Método de gestion de mensajes
        void gestionar_mensaje(){
            //Creamos variables donde almacenaremos los datos del cliente
            struct sockaddr client;		
            socklen_t client_len = sizeof(struct sockaddr);
            char buffer[80]; //Creamos un contenedor del mensaje lo suficientemente grande.
	    size_t answer; //Almacenamos el resultado de gettime
	    int bytes; //Tamaño del mensaje recibido
	    int errgetname;

            //Gestión del envío y recepción de mensajes
            while (bytes != -1) {
                //Recepcion mensaje2
                // int bytes recoge el valor de tamaño del mensaje que devuelve recvfrom
                bytes = recvfrom(	sd,				//Descriptor del socket de conexión
                            buffer, 			//Donde almacenamos el mensaje (de tamaño mayor al mensaje recibido)
                            80,				//Tamaño del contenedor del mensaje (definido arriba)
                            0,				//Flags
                            (struct sockaddr *) &client,	//Donde almacenaremos la dirección del cliente
                                &client_len			//Tamaño del contenedor de la dirección del cliente
                        );

		if (bytes == -1) std::cout << "Conexión cerrada" << std::endl;
            
                //Ponemos el carácter de fin de línea al final del mensaje recogido en el buffer. 
                //(La última posición es la dada por el tamaño del mensaje bytes).
                buffer[bytes]='\0'; 
            
                //Obtener información del cliente
                errgetname = getnameinfo(	(struct sockaddr *) &client, 	//Dirección cliente
                        client_len, 			//Tamaño cliente
                        host, 				//Donde guardamos la dirección
                        NI_MAXHOST,			//Tamaño que puede tener la dirección
                            serv, 				//Donde guardamos el puerto
                        NI_MAXSERV,			//Tamaño que puede tener el puerto
                        NI_NUMERICHOST|NI_NUMERICSERV	//Forma de mostrar de los datos
                    );

                //Mostrar información obtenida
		if (errgetname == 0) {
		        std::cout << "Hilo: " << pthread_self() << std::endl;
		        std::cout << "Conexion desde IP: "<< host << " Puerto: "<< serv << std::endl;
		        std::cout << "Mensaje de tamaño ("<< bytes <<"): " << buffer; //buffer contiene en fin de línea
		} else if (errgetname == -1) {
			std::cout << "Conexión cerrada" << std::endl;
		}

                //Generamos el mensaje de respuesta
                
                //Mensaje eco
                //sendto(sd, buffer, bytes, 0, (struct sockaddr*) &cliente, cliente_len);
                
                //Mensaje servidor de hora
                answer = getTime(buffer[0], buffer); //la primera posicion del buffer deberia contener la orden del cliente.

                if (answer == 0) {
                    std::cout << "Solicitud de cierre" << std::endl;          
		    bytes = -1;
                    //freeaddrinfo(res);
                    //return 0;	
		    //break;
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
                
                //pequeña pausa para el hilo
                sleep(5);
                
                //Devolvemos exito
                //break;

		} //while
            } //gestion_Mensaje

    void showinfo(){
	//Mostrar información obtenida
	std::cout << "Nuevo Hilo Generado\n";
        std::cout << "Hilo: " << pthread_self() << std::endl;
        std::cout << "Conexion desde IP: "<< host << " Puerto: "<< serv << std::endl;
    }

            
    protected:    
        char host[NI_MAXHOST]; //IP
        char serv [NI_MAXSERV]; //Puerto
        
    private:
        int sd; //Descriptor del socket
        
};

//Funcion llamada en el pthread_create: http://man7.org/linux/man-pages/man3/pthread_create.3.html
static void * thread_start(void *arg){
           
           //Almacenamos localmente el hilo
           HiloMensaje* hm = static_cast<HiloMensaje*> (arg);
	
           //Llamamos a la función de gestion de mensajes
	   //hm->showinfo();
           hm->gestionar_mensaje();
           
           //Borramos el puntero
           delete hm;
           
           //Devolvemos exito
           return 0;
 }

int main(int argc, char** argv){

	//Inicializar socket
    struct addrinfo hints; //socket
	struct addrinfo* res;  //puntero al socket
	memset((void*) &hints,'\0', sizeof(struct addrinfo)); 	//inicializamos socket
	hints.ai_family = AF_UNSPEC;         		      	//ipV4 o ipV6 soportado
	hints.ai_socktype = SOCK_DGRAM; 			//Ustart_routineDP SOCK_DGRAM, TCP SOCK_STREAM 


	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}
	
	//sd es el socket descriptor
	int sd = socket(res->ai_family, res->ai_socktype, 0);
    bind(sd,res->ai_addr, res->ai_addrlen); //UDP

    //Threading: Generación y gestión de hilos y trata del mensaje
    pthread_t tid[NUM_THREADS];	//Contenedor de identificadores de cada hilo con el tamaño estipulado en la variable global
    

    //BUCLE GESTOR
    for(int i = 0; i < NUM_THREADS; i++){
        //Atributos del thread, revisar https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032l/index.html
        pthread_attr_t tattr;
	pthread_attr_init (&tattr);
	int err = pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
        
        //Gestion errores
        if (err != 0){ 
            std::cout << "Error (pthread_attr_setdetachstate)\n";
            return -1;
        }
        
        //Generamos el hilo
        HiloMensaje* hilo = new HiloMensaje(sd); //creamos el gestor del mensaje
	err = pthread_create(&tid[i], &tattr, thread_start, static_cast<void*>(hilo)); //Creamos el hilo: http://man7.org/linux/man-pages/man3/pthread_create.3.html
        
        if (err != 0){ 
            std::cout << "Error (pthread_create)\n";
            return -1;
        }

    }
	
     //Limpiamos el puntero
     freeaddrinfo(res);

    
    //Esperar al cierre
    std::cout << "Esperando a nueva conexión u orden de cierre 'q'\n";
    char c;
    while (c != 'q') std::cin >> c;

    return 0;
}
