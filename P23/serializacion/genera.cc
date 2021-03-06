//Headers propios
#include "Serializable.h"

//Gestion ficheros
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//IO
#include <iostream>
#include <string.h>
#include <unistd.h>

//Gestion errores
#include <errno.h>

//Variables globales
const bool debug = true;

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
	//Damos el tamaño necesario a _data
	int32_t tamtotal = strlen(name) + 4;
	
	//Debug
	if(debug) std::cout << "Tamaño total: " << tamtotal << "\n";

	//Llamamos al metodo que genera tamaño memoria
	alloc_data(tamtotal); //le pasamos el tamaño total del nombre + 2 coordenadas y el ya crea cabecera+datos
 
	//almacenamos los datos en _data
	if(debug) std::cout << "Procediendo a almacenar NAME en _data\n";
	int i=0; // indice

	//primero almacenamos el nombre ///// ESTA PARTE DA ERROR ////////
   	for (i; i<strlen(name); i++){
		_data[i] = name[i];
	}
	if(debug) std::cout << "Procediendo a almacenar X e Y en _data\n";
	//Despues las variables de posicion
  	i++;
   	_data[i] = x;
   	i++;
   	_data[i] = y;

	if(debug) std::cout << "Terminado de almacenar en _data\n";
    }

    int from_bin(char * data)
    {
		
    }

public:
    char name[80];

    int16_t x;
    int16_t y;
    
    char* _data; //variable donde se guardara la info en forma binaria
};


int main(int argc, char **argv)
{

   
   //Debug (comando llamada ./"nombreApp" 127.0.0.1 3000 t
   if (debug){
	std::cout << "argc: " << argc << std::endl; //4
	std::cout << "argv[0]: " << argv[0] << std::endl; //./client
	std::cout << "argv[1]: " << argv[1] << std::endl; //127.0.0.1
	std::cout << "argv[2]: " << argv[2] << std::endl; //3000
	std::cout << "argv[3]: " << argv[3] << std::endl; //t
   }

   //Guardado en variables locales de los atributos pasados por comando
   char* lcname = argv[1];
   int16_t lcx = atoi(argv[2]);
   int16_t lcy = atoi(argv[3]);

   //Debug 
   if (debug){
	std::cout << "lcname: " << lcname << std::endl; //4
	std::cout << "lcx: " << lcx << std::endl; //./client
	std::cout << "lcy: " << lcy << std::endl; //127.0.0.1
   }

   //Generar el jugador con los atributos almacenados en las variables locales
   Jugador* p1 = new Jugador(lcname, lcx, lcy);
	
   //Mostramos en pantalla los datos almacenados
   if (debug){
	std::cout << "Jugador 1: " << p1->name << " en posicion (" << p1->x << "," << p1->y << ")\n";
   }
   
   //Creación del archivo donde vamos a guardar los datos  
   int fd = open("/home/usuario_vms/Descargas/practica2.3/serializacion/plydata",O_CREAT | O_RDWR | O_TRUNC, 0666); //fd es el file descriptor, O_CREAT es un flag para que cree el archivo) //0666 permisos

   if (fd == -1){
	std::cout << "Error al generar el fichero, fallo num(" << errno << "): " << strerror(errno) << "\n";
   } 
   
   //Le decimos al player que se serialize (pase a binario) y podamos usar su _data
   if (debug) std::cout << "Entrando en to_bin()" << "\n";

   p1->to_bin();

   
   //
   /* 
   //Creamos Buffer para almacenar datos
   char* buffer;
   buffer = (char*) malloc(strlen(lcname) + sizeof(int16_t));

   //Rellenamos el buffer con los datos
   int i=0;
   for (i; i<strlen(lcname); i++){
	buffer[i] = lcname[i];
   }
   i++;
   buffer[i] = lcx;
   i++;
   buffer[i] = lcy;
   */


   //Debug del serializado del jugador
   if (debug) {
	std::cout << "P1->_data size: " << strlen(p1->_data) << std::endl;
	std::cout << "Mensaje almacenado: ";

  	for(int j = 0; j < strlen(p1->_data); j++){
	   std::cout << p1->_data[j]; ; //4
	}

	std::cout << "\n";
   }

   //Escribimos los datos
   ssize_t wrterr = write(fd, p1->_data, strlen(p1->_data));
 
   if (wrterr == 0){
	std::cout << "Error al escribir, no se escribieron datos.\n";
   }
   else if (wrterr == -1){
        std::cout << "Error al escribir, fallo num(" << errno << "): " << strerror(errno) << "\n";
   }
   

   //Cerramos el archivo
   int clerr = close(fd);
   
   if (clerr == -1){
	std::cout << "Error al cerrar, fallo num(" << errno << "): " << strerror(errno) << "\n";
   }


   //Eliminar puntero jugador
   delete(p1);

}



