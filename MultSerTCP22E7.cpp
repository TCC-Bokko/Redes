//[Practica 2.2 Ejercicio 7] Servidor Eco TCP Multithreat
//El cliente podrá conectarse y escribir recibiendo un eco
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

//revisar https://www.cs.nmsu.edu/~jcook/Tools/pthreads/library.html
// http://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.htmllinux

//Clase propia para los hilos que gestionarán cada mensaje:
class HiloMensaje{
    public:
        //Constructora & Destructora
        HiloMensaje(int _sd): sd(_sd){};
        ~HiloMensaje() { //close(sd); 
        }
        
        //Método de gestion de mensajes
        void gestionar_mensaje(){
    

            char buffer[80]; //Creamos un contenedor del mensaje lo suficientemente grande.

            //Gestión del envío y recepción de mensajes
            while (1) {
                //Recepcion mensaje2
                // int bytes recoge el valor de tamaño del mensaje que devuelve recvfrom
                int bytes = recv(	sd,				//Descriptor del socket de conexión
                            buffer, 			//Donde almacenamos el mensaje (de tamaño mayor al mensaje recibido)
                            80,				//Tamaño del contenedor del mensaje (definido arriba)
                            0				//Flags
                            );
            
                std::cout << "Hilo: " << pthread_self() << std::endl;


                if (bytes == 0) {
                    std::cout << "Good bye!" << std::endl;
                    close(sd);                          //Cerramos descriptor
                    pthread_cancel(pthread_self());     //Cerramos thread         
                    //freeaddrinfo(res);
                    //return 0;	
                } else if(bytes != -1) {
                    //Enviamos respuesta
                    send(sd,		//Descriptor del socket 
                        buffer, 	//Mensaje
                        80,		//Longitud Mensaje
                        0 		//Flags	
                        );
                }

            }
            
        }

            
    protected:    
        
        
    private:
        int sd; //Descriptor del socket
        
};

//Funcion llamada en el pthread_create: http://man7.org/linux/man-pages/man3/pthread_create.3.html
static void * thread_start(void *arg){
           
           //Almacenamos localmente el hilo
           HiloMensaje* hm = static_cast<HiloMensaje*> (arg);

           //Llamamos a la función de gestion de mensajes
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
	hints.ai_socktype = SOCK_STREAM; 			//Ustart_routineDP SOCK_DGRAM, TCP SOCK_STREAM 


	//Obtener información de la dirección y almacenar resultado operacion
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(rc != 0){
		std::cout << "Error getaddrinfo(): " << gai_strerror(rc) << std::endl;
		return -1;
	}
	
	//sd es el socket descriptor
	int sd = socket(res->ai_family, res->ai_socktype, 0);
    bind(sd,res->ai_addr, res->ai_addrlen); //UDP

    //Listen del TCP
    listen(sd, 5);

    //Creamos variables donde almacenaremos los datos del cliente
    struct sockaddr client;     
    socklen_t client_len = sizeof(struct sockaddr);
    char host[NI_MAXHOST]; //IP
    char serv [NI_MAXSERV]; //Puerto
    int cd; //Descriptor del cliente
    

    while(1){
        cd = accept(sd, (struct sockaddr *) &client, &client_len);

        getnameinfo((struct sockaddr*) &client, client_len, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        //Obtener información del cliente
        getnameinfo((struct sockaddr *) &client,    //Dirección cliente
                client_len,             //Tamaño cliente
                host,               //Donde guardamos la dirección
                NI_MAXHOST,         //Tamaño que puede tener la dirección
                serv,               //Donde guardamos el puerto
                NI_MAXSERV,         //Tamaño que puede tener el puerto
                NI_NUMERICHOST|NI_NUMERICSERV   //Forma de mostrar de los datos
        );

        //Mostrar información obtenida
        
        std::cout << "Conexion desde IP: "<< host << " Puerto: "<< serv << std::endl;
        //std::cout << "Mensaje de tamaño ("<< bytes <<"): " << buffer; //buffer contiene en fin de línea

        //Threading: Generación y gestión de hilos y trata del mensaje
        pthread_t tid; //Contenedor de identificadores de cada hilo con el tamaño estipulado en la variable global

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
        HiloMensaje* hilo = new HiloMensaje(cd); //creamos el gestor del mensaje
        err = pthread_create(&tid, &tattr, thread_start, static_cast<void*>(hilo)); //Creamos el hilo: http://man7.org/linux/man-pages/man3/pthread_create.3.html
        
        //Gestion errores
        if (err != 0){ 
            std::cout << "Error (pthread_create)\n";
            return -1;
        } 
    } //Fin While
	
	//Limpiamos el puntero
	freeaddrinfo(res);
    
    //Esperar al cierre
    char c;
    while (c != 'q'); std::cin >> c;

	return 0;
}
