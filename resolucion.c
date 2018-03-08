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
  logger = log_create("tp0.log", "TP0", true, LOG_LEVEL_INFO);
}

int connect_to_server(char * ip, char * port) {
  struct addrinfo hints;
  struct addrinfo *server_info;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;    // Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
  hints.ai_socktype = SOCK_STREAM;  // Indica que usaremos el protocolo TCP

  getaddrinfo(ip, port, &hints, &server_info);  // Carga en server_info los datos de la conexion

  // 2. Creemos el socket con el nombre "server_socket" usando la "server_info" que creamos anteriormente
  int server_socket = server_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

  // 3. Conectemosnos al server a traves del socket!
  int res = connect(server_socket, server_info->ai_addr, server_info->ai_addrlen);

  freeaddrinfo(server_info);  // No lo necesitamos mas

  /*
    3.1 Recuerden chequear por si no se pudo contectar.
        Si hubo un error, lo loggeamos y podemos terminar el programa con la funcioncita
        exit_gracefully pasandole 1 como parametro para indicar error ;).
        Pss, revisen los niveles de log de las commons.
  */
  if (res < 0) {
    log_error(logger, "No me pude conectar al servidor");
    exit_gracefully(1);
  }
  // 4 Logeamos que pudimos conectar y retornamos el socket
  log_info(logger, "Conectado!");
  return server_socket;
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
  char * buffer = (char*) malloc(strlen(hola));
  /*
        5.2.  Recibamos el mensaje en el buffer.
        Recuerden el prototipo de recv:
        conexión - donde guardar - cant de bytes - flags(si no se pasa ninguno puede ir NULL)
        Nota: Palabra clave MSG_WAITALL.
  */
  int result_recv = recv(socket, buffer, strlen(hola), MSG_WAITALL);

  /*
        5.3.  Chequiemos errores al recibir! (y logiemos, por supuesto)
        5.4.  Comparemos lo recibido con "hola".
              Pueden usar las funciones de las commons para comparar!
        No se olviden de loggear y devolver la memoria que pedimos!
        (si, también si falló algo, tenemos que devolverla, atenti.)
  */

  void _exit_with_error(char* error_msg) {
    log_error(logger, error_msg);
    free(buffer);
    exit_gracefully(1);
  }

  if(result_recv <= 0) {
    _exit_with_error("No se pudo recibir hola");
  }

  if (strcmp(buffer, hola) != 0) {
    _exit_with_error("No se pudo recibir hola");
  }

  log_info(logger, "Mensaje de hola recibido: '%s'", buffer);
}

void send_hello(int socket) {
  log_info(logger, "Enviando info de Estudiante");
  /*
    6.    Ahora nos toca mandar el hola, por lo que mandemos un alumno!
          Alumno es esa estructura que definimos en el .h, vamos a crearla
          usando el id del protocolo para mandar un alumno: el id 99.
          Tambien, completemos los demas datos con tus datos propios.
  */
  Alumno alumno = { .id_mensaje = 99, .legajo = 1466562, .nombre = "Carlos", .apellido = "Perez" };
  /*
    6.1.  Como algo extra, podes probar enviando caracteres invalidos en el nombre
          o un id de otra operacion a ver que responde el servidor y como se
          comporta nuestro cliente.
  */  

  // alumno.id = 33;
  // alumno.nombre[2] = -4;

  /*
    7.    Finalmente, enviemos la estructura por el socket!
          Recuerden que nuestra estructura esta definida como __attribute__((packed))
          por lo que no tiene padding y la podemos mandar directamente sin necesidad
          de un buffer y usando el tamaño del tipo Alumno!
  */
  if (send(socket, &alumno, sizeof(Alumno), 0) <= 0) {
    log_error(logger, "Could not send hello");
    exit_gracefully(1);
  }
}

void exit_gracefully(int return_nr) {
  /*
    Así como lo creamos, no olvidemos de destruirlo al final
  */
  log_destroy(logger);
  exit(return_nr);
}