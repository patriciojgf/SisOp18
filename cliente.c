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
  Alumno alumno = ;
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
  printf("Waiting content header (%ld bytes)\n", sizeof(ContentHeader));
  ContentHeader * header = GC_MALLOC(sizeof(ContentHeader));

  if (recv(socket, header, sizeof(ContentHeader), 0) <= 0) {
    perror("Could not receive content header");
    exit(1);
  }

  if (header->id != 18) {
    printf("Invalid message id %d, but was expecting 18", header->id);
  }

  printf("Waiting content (%d bytes)\n", header->len);
  void * buf = GC_MALLOC(header->len);
  if (recv(socket, buf, header->len, MSG_WAITALL) <= 0) {
    perror("Error receiving content");
    exit(1);
  }

  printf("Received content '%s'\n", (char*) buf);
  return buf;
}

void send_md5(int socket, void * content) {
  void * digest = GC_MALLOC(MD5_DIGEST_LENGTH);
  MD5_CTX context;
  MD5_Init(&context);
  MD5_Update(&context, content, strlen(content) + 1);
  MD5_Final(digest, &context);

  Md5Header header = { .id = 33, .len = MD5_DIGEST_LENGTH };
  
  // Try sending invalid header length
  // Md5Header header = { .id = 33, .len = 500 };
  
  int message_size = sizeof(Md5Header) + MD5_DIGEST_LENGTH;
  void * buf = GC_MALLOC(message_size);

  memcpy(buf, &header, sizeof(Md5Header));
  memcpy(buf + sizeof(Md5Header), digest, MD5_DIGEST_LENGTH);

  printf("Sending MD5\n");
  if (send(socket, buf, message_size, 0) <= 0) {
    perror("Could not send md5");
  }
}

void wait_confirmation(int socket) {
  int result = 1;
  printf("Waiting confirmation\n");
  if (recv(socket, &result, sizeof(int), 0) <= 0) {
    perror("Could not receive confirmation");
    exit(1);
  }

  if (result != 0) {
    printf("Result md5 did not match\n");
    exit(1);
  }

  printf("MD5 Matched!\n");
}

void exit_gracefully(int return_nr) {
  /*
    Así como lo creamos, no olvidemos de destruirlo al final
  */
}