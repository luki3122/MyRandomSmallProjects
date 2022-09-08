// Server side C/C++ program to demonstrate Socket
// programming
#include "Server.h"
#include "StringUtils.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#ifdef __APPLE__
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
#endif
#ifdef __linux__
#endif

#define PORT 8080
#define MAX_CLIENTS 30
#define MESSAGE_BUFFER_SIZE 1024
#define FUNCTION_PREFIX '!'
#define MAX_NAME_LENGTH 12

// struct session {
//   int socket;
//   char name[MAX_NAME_LENGTH + 1];
// };
// struct session Sessions[MAX_CLIENTS];
int Server_socket;
fd_set Current_sockets;
int Max_socket;
char Names[1024][MAX_NAME_LENGTH + 1];
char *Banned_words[2] = {"blockme", 0};

void printLog(int source, const char *message) {

  char buffer[1024] = {0};
  if (source)
    strcpy(buffer, "Client: ");
  else
    strcpy(buffer, "Server: ");

  strcat(buffer, message);

  if (!source)
    strcat(buffer, "\n");

  printf("%s", buffer);
}

int check(int exp, char *error_message) {
  if (exp == -1) {
    char buffer[1024] = {0};
    strcat(buffer, error_message);
    strcat(buffer, "\n");
    perror(buffer);
    exit(EXIT_FAILURE);
  }
  return exp;
}

void nameChange(char *buffer, size_t buffer_size, int client_socket) {
  int iter = 0;
  char *read = buffer + 1;
  while (!charIsWhitespace(*read++) && iter++ < buffer_size - 1)
    ;

  char name[MAX_NAME_LENGTH + 1];
  char *write = name;
  int name_iter = 0;
  while (!charIsWhitespace(*read) && iter++ < buffer_size - 1 &&
         name_iter++ < MAX_NAME_LENGTH)
    *write++ = *read++;

  *write = 0;
  strcpy(Names[client_socket], name);
}

char *getName(int client_socket) { return Names[client_socket]; }

boolean isFunction(const char *buffer) { return *buffer == FUNCTION_PREFIX; }

boolean checkForFunction(const char *buffer, size_t buffer_size,
                         const char *command) {
  const char *read = buffer + 1;
  return compareWords(read, command);
}

void processFunctions(char *buffer, size_t buffer_size, int client_socket) {

  if (!isFunction(buffer))
    return;
  if (checkForFunction(buffer, buffer_size, "name")) {
    // getArgs()
    nameChange(buffer, buffer_size, client_socket);
  }
}

void handleClientDisconnect(int client_socket) {
  // printf("SERVER: Client Disconnected: %i\n", client_socket);
  strcpy(Names[client_socket], "Annon");
  FD_CLR(client_socket, &Current_sockets);
}

void sendMessage(int sender_socket, int receiver_socket, const char *buffer) {
  char name_buffer[MAX_NAME_LENGTH + 3];
  strcpy(name_buffer, getName(sender_socket));
  strcat(name_buffer, ": ");
  send(receiver_socket, name_buffer, strlen(name_buffer), 0);
  send(receiver_socket, buffer, strlen(buffer), 0);
}

void broadcast(int sender_socket, char *buffer, size_t buffer_size) {
  for (int i = 0; i <= Max_socket; i++) {
    if (FD_ISSET(i, &Current_sockets)) {
      if (i != sender_socket && i != Server_socket) {
        sendMessage(sender_socket, i, buffer);
        // printf("SERVER: Message Sent to: %i\n", i);
      }
    }
  }
}

int setupServer(short port, int backlog) {
  int server_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
        "Setup Failed: socket()");

  check(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)),
        "Setup Failed: setsockopt()");

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  check(bind(server_socket, (struct sockaddr *)&address, sizeof(address)),
        "Setup Failed: bind()");
  check(listen(server_socket, backlog), "Setup Failed: listen()");
  return server_socket;
}

int acceptNewConnection(int server_socket) {
  int client_socket;
  struct sockaddr_in address;
  size_t addrlen = sizeof(address);
  check((client_socket = accept(server_socket, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen)),
        "New Connection Error: accept()");
  strcpy(Names[client_socket], "Annon");
  return client_socket;
}

boolean containsBannedWords(const char *string) {

  for (int i = 0; Banned_words[i]; i++) {
    if (strstr(string, Banned_words[i]))
      return 1;
  }
  return 0;
}

void handleConnection(int client_socket) {
  char buffer[MESSAGE_BUFFER_SIZE] = {0};
  int bytes_read;

  bytes_read = recv(client_socket, buffer, MESSAGE_BUFFER_SIZE, 0);
  if (bytes_read <= 0) {
    handleClientDisconnect(client_socket);
    return;
  }

  formatMessage(buffer, MESSAGE_BUFFER_SIZE);
  processFunctions(buffer, MESSAGE_BUFFER_SIZE, client_socket);
  if (containsBannedWords(buffer))
    return;
  broadcast(client_socket, buffer, MESSAGE_BUFFER_SIZE);
}

int main(int argc, char const *argv[]) {

  Server_socket = setupServer(PORT, 10);

  fd_set ready_sockets;
  Max_socket = Server_socket;

  FD_ZERO(&Current_sockets);
  FD_SET(Server_socket, &Current_sockets);

  while (1) {
    ready_sockets = Current_sockets;

    if (select(Max_socket + 1, &ready_sockets, NULL, NULL, NULL) < 0)
      perror("Select Error: select()");

    for (int i = 0; i <= Max_socket; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == Server_socket) {

          int client_socket = acceptNewConnection(Server_socket);
          // printf("SERVER: New connection on socket: %i\n", client_socket);
          FD_SET(client_socket, &Current_sockets);
          Max_socket = MAX(Max_socket, client_socket);
        } else {
          // printf("SERVER: Message Sent from client: %i\n", i);
          handleConnection(i);
          // FD_CLR(i, &Current_sockets);
        }
      }
    }
  }

  // closing the listening socket
  shutdown(Server_socket, SHUT_RDWR);
  return 0;
}
