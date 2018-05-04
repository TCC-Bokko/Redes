# Redes
Parte de programación de Redes

Desarrollado en Unix
Programa práctico de estudio de redes.

Compilación
  $ g++ -o "nombre_ejecutable_a_generar" "archivo_código"
  En el caso de requerir la librería de "real time" (P21)
  $ g++ -lrt -o "nombre_ejecutable_a_generar" "archivo_código"
  
Ejecución
Se abriran distintos terminales(shell/consola) para emular servidor / cliente
  $ ./"Nombre ejecutable" "IP" "Puerto" (Servidor)
  $ nc "IP" "Puerto" (cliente)
  
Revisión
  En otra consola podemos revisar el estado de las conexiones
  $ netstat -un | grep "puerto" (UDP)
  $ netstat -tan | grep "puerto" (TCP)
  Otros modificadores útiles
    -a : ver que puertos udp/tcp estan en modo "listen"
    -n : muestra numéricamente las conexiones.
  
