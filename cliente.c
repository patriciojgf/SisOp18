#include "cliente.h"


int main() {
  configure_logger();
  int socket = connect_to_server(IP, PUERTO);
  wait_hello(socket);
  send_hello(socket);
  void * content = wait_content(socket);
  send_md5(socket, content);
  wait_confirmation(socket);
  exit_gracefully(0);
}

void configure_logger() {
  /*
    1.  Creemos el logger con la funcion de las commons log_create.
        Tiene que: guardarlo en el archivo tp0.log, mostrar 'tp0' al loggear,
        mostrarse por pantalla y mostrar solo los logs de nivel info para arriba
        (info, warning y error!)
  */
}

int connect_to_server(char * ip, char * port) {
  struct addrinfo hints;
  struct addrinfo *server_info;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;    // Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
  hints.ai_socktype = SOCK_STREAM;  // Indica que usaremos el protocolo TCP

  getaddrinfo(ip, port, &hints, &server_info);  // Carga en server_info los datos de la conexion

  // 2. Creemos el socket con el nombre "server_socket" usando la "server_info" que creamos anteriormente

  // 3. Conectemosnos al server a traves del socket!

  freeaddrinfo(server_info);  // No lo necesitamos mas

  /*
    3.1 Recuerden chequear por si no se pudo contectar.
        Si hubo un error, lo loggeamos y podemos terminar el programa con la funcioncita
        exit_gracefully pasandole 1 como parametro para indicar error ;).
        Pss, revisen los niveles de log de las commons.
  */

  // 4 Logeamos que pudimos conectar y retornamos el socket
}

void  wait_hello(int socket) {
  char * hola = "SYSTEM UTNSO 0.1";

  /*
    5.  Ya conectados al servidor, vamos a hacer un handshake!
        Para esto, vamos a, primero recibir un mensaje del
        servidor y luego mandar nosotros un mensaje.
        Deberìamos recibir lo mismo que está contenido en la
        variable "hola". Entonces, vamos por partes:
        5.1.  Reservemos memoria para un buffer para recibir el mensaje.
  */
  char * buffer = malloc(/*5.1*/);
  /*
        5.2.  Recibamos el mensaje en el buffer.
        Recuerden el prototipo de recv:
        conexión - donde guardar - cant de bytes - flags(si no se pasa ninguno puede ir NULL)
        Nota: Palabra clave MSG_WAITALL.
  */
  int result_recv = recv(/*5.2*/);
  /*
        5.3.  Chequiemos errores al recibir! (y logiemos, por supuesto)
        5.4.  Comparemos lo recibido con "hola".
              Pueden usar las funciones de las commons para comparar!
        No se olviden de loggear y devolver la memoria que pedimos!
        (si, también si falló algo, tenemos que devolverla, atenti.)
  */

}

void send_hello(int socket) {
  log_info(logger, "Enviando info de Estudiante");
  /*
    6.    Ahora nos toca mandar el hola, por lo que mandemos un alumno!
          Alumno es esa estructura que definimos en el .h, vamos a crearla
          usando el id del protocolo para mandar un alumno: el id 99.
          Tambien, completemos los demas datos con tus datos propios.
  */
  Alumno alumno = { /* ??? */ };
  /*
    6.1.  Como algo extra, podes probar enviando caracteres invalidos en el nombre
          o un id de otra operacion a ver que responde el servidor y como se
          comporta nuestro cliente.
  */  

  /*
    7.    Finalmente, enviemos la estructura por el socket!
          Recuerden que nuestra estructura esta definida como __attribute__((packed))
          por lo que no tiene padding y la podemos mandar directamente sin necesidad
          de un buffer y usando el tamaño del tipo Alumno!
  */
}

void * wait_content(int socket) {
  /*
    8.    Ahora tenemos que recibir un contenido de tamaño variable
          Para eso, primero tenemos que confirmar que el id corresponde al de una
          respuesta de contenido variable (18) y despues junto con el id de operacion
          vamos a haber recibido el tamaño del contenido que sigue. Por lo que:
  */

  log_info(logger, "Esperando el encabezado del contenido(%ld bytes)", sizeof(ContentHeader));
  // 8.1. Reservamos el suficiente espacio para guardar un ContentHeader
  ContentHeader * header = { /* 8.1. */ };

  // 8.2. Recibamos el header en la estructura y chequiemos si el id es el correcto.
  //      No se olviden de validar los errores!

  log_info(logger, "Esperando el contenido (%d bytes)", header->len);

  /*
      9.    Ahora, recibamos el contenido variable. Ya tenemos el tamaño,
            por lo que reecibirlo es lo mismo que veniamos haciendo:
      9.1.  Reservamos memoria
      9.2.  Recibimos el contenido en un buffer (si hubo error, fallamos, liberamos y salimos
  */

  /*
      10.   Finalmente, no te olvides de liberar la memoria que pedimos
            para el header y retornar el contenido recibido.
  */
}

void send_md5(int socket, void * content) {
  /*
    11.   Ahora calculemos el MD5 del contenido, para eso vamos
          a armar el digest:
  */

  void * digest = malloc(MD5_DIGEST_LENGTH);
  MD5_CTX context;
  MD5_Init(&context);
  MD5_Update(&context, content, strlen(content) + 1);
  MD5_Final(digest, &context);

  /*
    12.   Luego, nos toca enviar a nosotros un contenido variable.
          A diferencia de recibirlo, para mandarlo es mejor enviarlo todo de una,
          siguiendo la logida de 1 send - N recv.
          Asi que:
  */

  //      12.1. Creamos un Md5Header para guardar un mensaje de id 33 y el tamaño del md5

  Md5Header header = { /* 12.1. */ };

  /*
          12.2. Creamos un buffer del tamaño del mensaje completo y copiamos el header y la info de "digest" allí.
          Recuerden revisar la función memcpy(ptr_destino, ptr_origen, tamaño)!
  */

  /*
    13.   Con todo listo, solo nos falta enviar el paquete que armamos y liberar la memoria que usamos.
          Si, TODA la que usamos, eso incluye a la del contenido del mensaje que recibimos en la función
          anterior.
  */
}

void wait_confirmation(int socket) {
  int result = 1; // Dejemos creado un resultado por defecto
  /*
    14.   Ahora nos toca recibir la confirmacion del servidor.
          Si el resultado obvenido es distinto de 0, entonces hubo un error
  */

  log_info(logger, "Los MD5 concidieron!");
}

void exit_gracefully(int return_nr) {
  /*
    15.   Siempre llamamos a esta funcion para cerrar el programa.
          Asi solo necesitamos destruir el logger y usar la llamada al
          sistema exit() para terminar la ejecucion
  */
}