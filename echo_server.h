#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX_CLIENTS 5       // Maximum number of clients that can connect
#define BUFFER_SIZE 1024    // Buffer size for reading data from clients
#define PORT 8888           // Port number
#define NULL_CHAR '\0'      // We check for NULL at the end

#endif // ECHO_SERVER_H 