//IO
#include <string>
#include <unistd.h>
#include <string.h>

//Cabeceras propias
#include "Serializable.h"
#include "Socket.h"
#include "UDPServer.h"


//Variables globales
const bool debug = true;

//En esta practica se usan muchos metodos de copiado de cadenas de bytes
//existen muchas pero buscar copy, bcopy, memcpy, strncpy...

class ChatMessage: public Serializable
{
public:
    ChatMessage(){};

    ChatMessage(const char * n, const std::string m)
    {
        //Esta funcion es llamada con el nombre de usuario y mensaje enviado, que almacenaremos en variables locales name y message
        strncpy(name, n, 8); //strncpy(destino, origen, nº bytes a copiar del origen) Ver man strncpy
        m.copy(message,80);  //Metemos el mensaje recibido en m en nuestra variable mensaje
    };

    void to_bin()
    {
        //Damos el tamaño necesario a _data
        int32_t tamtotal = 80 * sizeof(char) + 8 * sizeof(char); //los 80 caracteres de message + 8 caracteres nombre usuario 
        
        //Debug
        if(debug) std::cout << "Tamaño total: " << tamtotal << "\n";

        //Llamamos al metodo que genera tamaño memoria
        alloc_data(tamtotal); //localizamos la memoria necesaria para albergar los datos a serializar
     
        //almacenamos los datos en _data
        if(debug) std::cout << "Procediendo a almacenar NAME en _data\n";
        _data = tamtotal;
        memcpy(_data, (void*) name, 8); //Copiamos en los primeros 8 bytes de data el nombre
        _data += 8;
        memcpy(_data, (void*) message, 80); //Copiamos en los 80 restantes el mensaje
        //_data += 80; //??

        if(debug) 
            std::cout << "Terminado de almacenar en _data.\n Nombre: ";
            for (int i = 0; i < 8; i++){
                std::cout << 
            }            }   
    }

    virtual int from_bin(char * bobj)
    {

    }

    char name[8];
    char message[80];
    char* _data; //variable donde se guardara la info en forma binaria
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class ChatServer : public UDPServer
{
public:
    ChatServer(const char * s, const char * p): UDPServer(s, p){};

    void do_message(char * buffer)
    {

    }
};

class ChatClient
{
public:
    ChatClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};

    void input_thread()
    {
    }

    void net_thread()
    {
    }

private:
    char nick[8];

    Socket socket;
};

