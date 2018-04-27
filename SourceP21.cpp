//Antonio Cardona Costa

//io
#include <stdio.h> 
#include <iostream>
//error
#include <errno.h>
#include <string.h>
//setuid
#include <unistd.h>
#include <sys/types.h>
//time
#include <time.h>
#include <sys/time.h>

int main() {
   /* Comprobar la ocurrencia de error y notificarlo con la llamada perror(3) */ 
    int result = setuid(0);
    
    if (result == -1) {
	   
       //Ejercicio 1
        perror("[P2.1 Ej.1] (perror) Error: ");
    
       //Ejercicio 2
        std::cout << "[P2.1 Ej.2] (cout) Error: " << errno << " -> " << strerror(errno) << std::endl; //strerror muestra el mensaje de error
        //printf("(printf) Error: %i", errno);

    }

    //Ejercicio 3
    time_t t;
    std::cout << "[P2.1 Ej.3] Current time: " << time(&t) << std::endl;

    //Ejercicio 4
    char buffer[26]; //Minimo necesario segun man 26 Bytes, utilizan 80 en http://www.cplusplus.com/reference/ctime/strftime/
    ctime_r(&t, buffer); //ver man ctime_r: transforma la info del time_t a un formato legible
    std::cout << "[P2.1 Ej.4] Date & time: " << buffer; //buffer contiene el endline

    //Ejercicio 5
    //Ver: https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
    //timespec contiene una representación en nanosegundos.
   
    struct timespec inicio;
    struct timespec fin;

    clock_gettime(CLOCK_REALTIME, &inicio); //tomar tiempo al inicio
    
    int i = 0;

    while ( i < 10000) { i++; }

    clock_gettime(CLOCK_REALTIME, &fin); //tomar tiempo al final

    long int diff = fin.tv_nsec-inicio.tv_nsec;

    //ver como pasar a nanosegundos
    std::cout << "[P2.1 Ej.5] 10000 bucle iterations completed in: " << diff << " nanoseconds" << std::endl;

    //Ejercicio 6
    //Para que funcione lo del time usar al llamar realtime -> al compilar utilizar modificador -lrt
    //Ver ctime(3) donde explica el tm
    /*
    time(&t); //actualizar t en el momento actual
    struct tm* tm; //estructura tm
    tm = localtime (&t); //rellenamos el struct
    //especificadores http://www.cplusplus.com/reference/ctime/strftime/
    //std::cout << "[P2.1 Ej.6] Buffer info: " << buffer;
    std::cout << "[P2.1 Ej.6] strftime expresion: " << std::endl;
    strftime(buffer,80,"Now it's %I:%M%p.",tm);
    //strftime(buffer, 26, "Estamos en el año: %G. Hoy es %A, %R.", tm);
    //printf("Estamos en el año: %G", buffer);
    */

    //Ejercicio 6.b
    //Desde el principio usando el ejemplo de http://www.cplusplus.com/reference/ctime/strftime/
    time_t rawtime;
    struct tm * timeinfo;
    char bufferb [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (bufferb,80,"[P2.1 Ej.6] Estamos en el año: %G. Hoy es %A, %R.",timeinfo);
    puts (bufferb);


    return 1;
}
