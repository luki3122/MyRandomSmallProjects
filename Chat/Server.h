#pragma once

#include "Definitions.h"
#include <sys/signal.h>

#ifdef __APPLE__
#include <sys/_types/_size_t.h>
#endif
#ifdef __linux__
#include <stddef.h>
#endif

typedef int boolean;

int check(int exp, char *error_message);

void nameChange(char *buffer, size_t buffer_size, int client_socket);
char *getName(int client_socket);

boolean isFunction(const char *buffer);
boolean checkForFunction(const char *buffer, size_t buffer_size,
                         const char *command);
void processFunctions(char *buffer, size_t buffer_size, int client_socket);

void handleClientDisconnect(int client_socket);
void broadcast(int sender_socket, char *buffer, size_t buffer_size);

int setupServer(short port, int backlog);
int acceptNewConnection(int server_socket);
void handleConnection(int client_socket);
