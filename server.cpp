/************************************************************/
/* This is a stream socket server sample program for UNIX   */
/* domain sockets. This program listens for a connection    */
/* from a client program, accepts it, reads data from the   */
/* client, then sends data back to connected UNIX socket.   */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

#define SOCK_PATH  "./.socket"
#define DATA "Hello from server"

int main(void){

    int server_sock, client_sock_fd, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;     
    char buf[256];
    int backlog = 10;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(buf, 0, 256);                
    
    /**************************************/
    /* Create a UNIX domain stream socket */
    /**************************************/
    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock == -1){
        printf("SOCKET ERROR");
        exit(1);
    }
    
    /***************************************/
    /* Set up the UNIX sockaddr structure  */
    /* by using AF_UNIX for the family and */
    /* giving it a filepath to bind to.    */
    /*                                     */
    /* Unlink the file so the bind will    */
    /* succeed, then bind to that file.    */
    /***************************************/
    server_sockaddr.sun_family = AF_UNIX;   
    strcpy(server_sockaddr.sun_path, SOCK_PATH); 
    socklen_t len = sizeof(server_sockaddr);
    
    unlink(SOCK_PATH);
    rc = bind(server_sock, (struct sockaddr *) &server_sockaddr, len);
    if (rc == -1){
        printf("BIND ERROR");
        close(server_sock);
        exit(1);
    }
    
    /*********************************/
    /* Listen for any client sockets */
    /*********************************/
    rc = listen(server_sock, backlog);
    if (rc == -1){ 
        printf("LISTEN ERROR");
        close(server_sock);
        exit(1);
    }
    printf("socket listening...\n");
    
    /*********************************/
    /* Accept an incoming connection */
    /*********************************/
    while(true){
        client_sock_fd = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);
        if (client_sock_fd == -1){
            printf("Client accept error");
            close(client_sock_fd);
        }
        bzero(buf, 256);
        /****************************************/
        /* Get the name of the connected socket */
        /****************************************/
        len = sizeof(client_sockaddr);
        // rc = getpeername(client_sock_fd, (struct sockaddr *) &client_sockaddr, &len);
        // if (rc == -1){
        //     printf("GETPEERNAME ERROR");
        //     close(server_sock);
        //     close(client_sock_fd);
        //     exit(1);
        // }
        // else {
        //     printf("Client socket filepath: %s\n", client_sockaddr.sun_path);
        // }
        
        /************************************/
        /* Read and print the data          */
        /* incoming on the connected socket */
        /************************************/
        printf("waiting to read...\n");
        if (read(client_sock_fd, buf, 256) == -1) {
            fprintf(stderr, "Error: Failed to read socket\n");
            continue;
        }
        printf("DATA RECEIVED = %s\n", buf);
        
        /******************************************/
        /* Send data back to the connected socket */
        /******************************************/ 
        printf("Sending data...\n");
        int ret;
        strcpy(buf, "Hello");
        if ((ret = write(client_sock_fd, buf, 256)) == -1){
        fprintf(stderr, "Error: Failed to write socket\n");
        }
        close(client_sock_fd);
    }
    
    /******************************/
    /* Close the sockets and exit */
    /******************************/
    close(server_sock);
    
    return 0;
}