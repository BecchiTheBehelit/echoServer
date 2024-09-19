#include "echo_server.h"  

int main() 
{
    int server_socket, client_sockets[MAX_CLIENTS];  // Server socket and array to store client sockets
    fd_set read_fds;                                 // File descriptor set for select()
    struct sockaddr_in server_addr, client_addr;     // Server and client address structures
    socklen_t client_len = sizeof(client_addr);      // Length of the client address structure
    char buffer[BUFFER_SIZE];                        // Buffer to hold data sent by clients 

    // Initialize client sockets to 0 (not in use)
    for(int i = 0; i < MAX_CLIENTS; i++) 
    {
        client_sockets[i] = 0;
    }

    // Create server socket (IPv4, TCP)
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == -1) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow reuse of address and port
    int opt = 1;

    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;                // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;        // Any available interface
    server_addr.sin_port = htons(PORT);              // Port number 

    // Bind the server socket to the specified IP address and port
    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections with a maximum backlog of MAX_CLIENTS
    if(listen(server_socket, MAX_CLIENTS) < 0) 
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Echo server is listening on port %d\n", PORT);

    while(1) 
    {
        // Clear and set the file descriptor set for select()
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);  // Add server socket to the set
        int max_sd = server_socket;        // Initialize max_sd to the server socket

        // Add client sockets to the file descriptor set
        for(int i = 0; i < MAX_CLIENTS; i++) 
        {
            int sd = client_sockets[i];

            if(sd > 0) 
            {
                FD_SET(sd, &read_fds);     // Add valid client socket to the set
            }

            if(sd > max_sd) 
            {
                max_sd = sd;               // Update max_sd to the highest descriptor
            }
        }

        // Wait for activity on one of the sockets using select()
        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);

        if(activity < 0) 
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // If activity on the server socket, it's a new incoming connection
        if(FD_ISSET(server_socket, &read_fds)) 
        {
            int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

            if(new_socket < 0) 
            {
                perror("Accept failed");
                exit(EXIT_FAILURE);
            }

            // Print details of the new connection
            printf("New connection, socket fd is %d, IP is: %s, port: %d\n",
                   new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Add the new socket to the array of client sockets
            for(int i = 0; i < MAX_CLIENTS; i++) 
            {
                if(client_sockets[i] == 0) 
                {
                    client_sockets[i] = new_socket;  // Store the socket
                    break;
                }
            }
        }

        // Handle input/output operations on the client sockets
        for(int i = 0; i < MAX_CLIENTS; i++) 
        {
            int sd = client_sockets[i];
            
            if(FD_ISSET(sd, &read_fds)) 
            {
                int valread = recv(sd, buffer, BUFFER_SIZE, 0);  // Read data from the client

                if(valread == 0) 
                {
                    // Client disconnected, get details of the disconnected client
                    getpeername(sd, (struct sockaddr *)&client_addr, &client_len);
                    printf("Host disconnected, IP %s, port %d\n",
                           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    close(sd);                // Close the socket
                    client_sockets[i] = 0;    // Mark socket as available
                } 
                
                else 
                {
                    // Null-terminate the received string and echo it back to the client
                    buffer[valread] = NULL_CHAR; 
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }

    return 0;
}
