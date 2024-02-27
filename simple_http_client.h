/*
Author: Brian Cook
Version: Milestone 3.0
Course: SWE-450
Professor: Mark Reha
Date of Creation: 2/26/2024
Derived from https://github.com/langhai/http-client-c  and https://github.com/reagent/http
*/

#ifndef SIMPLE_HTTP_CLIENT_H
#define SIMPLE_HTTP_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HTTP_PORT 80
#define BUFFER_SIZE 1024

#include <netdb.h>

// function for sending an HTTP request to the server
int send_http_request(const char *hostname, const char *path, const char *apikey) {
    // Create socket
    int sock;
    // Create server struct
    struct sockaddr_in server;
    // Create message buffer
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];
    struct hostent *server_host;

    // Get server IP from hostname
    server_host = gethostbyname(hostname);
    if (server_host == NULL) {
        perror("gethostbyname failed");
        return -1;
    }
    // Print the  IP address
    printf("Hostname resolved to IP: %s\n", inet_ntoa(*(struct in_addr*)server_host->h_addr_list[0]));

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return -1;
    }
    printf("Socket created.\n");
    // Set server struct values
    server.sin_addr = *(struct in_addr*)server_host->h_addr_list[0];
    server.sin_family = AF_INET;
    server.sin_port = htons(HTTP_PORT);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed");
        return -1;
    }
    printf("Connected to server.\n");

    // Send HTTP request to server with path and apikey
    sprintf(message, "GET %s HTTP/1.1\r\nHost: %s\r\nX-CMC_PRO_API_KEY: %s\r\n\r\n", path, hostname, apikey);
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Send failed");
        return -1;
    }
    // Print the message sent successfully
    printf("HTTP request sent. Message:\n%s\n", message);

    // Receive a reply from the server
    if (recv(sock, server_reply, sizeof(server_reply), 0) < 0) {
        perror("recv failed");
        return -1;
    }
    printf("HTTP response received. Response:\n%s\n", server_reply);
    // Close the socket
    close(sock);
    return 0;
}
// Function to do the same as above but to store the response in a string (char array)
char* store_http_request(const char *hostname, const char *path, const char *apikey) {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], *server_reply;
    struct hostent *server_host;

    // Allocate memory for the server reply
    server_reply = malloc(BUFFER_SIZE * sizeof(char));
    if(server_reply == NULL) {
        printf("Failed to allocate memory for server reply\n");
        return NULL;
    }

    // Get server IP from hostname
    server_host = gethostbyname(hostname);
    if (server_host == NULL) {
        perror("gethostbyname failed");
        free(server_reply);
        return NULL;
    }
    printf("Hostname resolved to IP: %s\n", inet_ntoa(*(struct in_addr*)server_host->h_addr_list[0]));

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        free(server_reply);
        return NULL;
    }
    printf("Socket created.\n");

    server.sin_addr = *(struct in_addr*)server_host->h_addr_list[0];
    server.sin_family = AF_INET;
    server.sin_port = htons(HTTP_PORT);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect failed");
        free(server_reply);
        return NULL;
    }
    printf("Connected to server.\n");

    // Send HTTP request
    sprintf(message, "GET %s HTTP/1.1\r\nHost: %s\r\nX-CMC_PRO_API_KEY: %s\r\n\r\n", path, hostname, apikey);
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Send failed");
        free(server_reply);
        return NULL;
    }
    printf("HTTP request sent. Message:\n%s\n", message);

    // Receive a reply from the server
    if (recv(sock, server_reply, BUFFER_SIZE, 0) < 0) {
        perror("recv failed");
        free(server_reply);
        return NULL;
    }
    printf("HTTP response received. Response:\n%s\n", server_reply);

    close(sock);
    return server_reply;
}
#endif /* SIMPLE_HTTP_CLIENT_H */