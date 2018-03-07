#include "cliente.h"


int main() {
  configure_logger();
  int socket = connect_to_server(IP, PUERTO);
  wait_hello(socket);
  send_hello(socket);
  void * content = wait_content(socket);
  send_md5(socket, content);
  wait_confirmation(socket);

  return 0;
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
        Si hubo un error, lo loggeamos y salimos ;).
        Pss, revisen los niveles de log de las commons.
  */

  // 4 Logeamos que pudimos conectar y retornamos el socket
}

void  wait_hello(int socket) {
  char * hola = "SYSTEM UTNSO 0.1";
  char * buf = GC_MALLOC(strlen(hola));
  printf("Waiting Hello\n");
  if (recv(socket, buf, strlen(hola), MSG_WAITALL) <= 0) {
    perror("Could not receive hello");
    exit(1);
  }

  if (strcmp(buf, hola) != 0) {
    printf("Invalid hello message");
    exit(1);
  }

  printf("Received hello message: '%s'\n", buf);
}

void send_hello(int socket) {
  printf("Sending student info\n");
  Alumno alumno = { .id_mensaje = 99, .legajo = 1466562, .nombre = "Carlos", .apellido = "Perez" };
  
  // Try invalid ascii chars
  // alumno.nombre[2] = -4;

  if (send(socket, &alumno, sizeof(Alumno), 0) <= 0) {
    perror("Could not send hello");
    exit(1);
  }
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