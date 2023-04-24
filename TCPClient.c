// to run
// filename server_ipaddress portno.
// eg.
// ./Client 127.0.0.1 9999
// to get server ipaddress use ifconfig on server 
// if on same machine write loopback or loopback address ( 127.0.0.1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <netdb.h> //defines hostent structure -> used to store information about host i.e., name, ipv4 address

void error(const char* msg){
    perror(msg);
    exit(1);
}
int main(int argc, char* argv[]){
    if(argc<3){
        fprintf(stderr, "Usage %s hostipaddress portnumber\n", argv[0]); //argv[0] stores filename and then all rest cells stores arguements
        exit(1);
    }
    int socket_fd, port_no, n;
    char buffer[255];
    struct sockaddr_in serv_addr;
    struct hostent *server;

    port_no = atoi(argv[2]); 
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd<0){  
        error("Error in opening a socket");
    }
    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "Error : No such Host");
    }

    bzero((char*) &serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);  //bcopy fuction copies server haddr to serv_addr
    serv_addr.sin_port = htons(port_no); 
    if(connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Connection error");
    }
    while(1){
        bzero(buffer, 255);
        printf("Enter Message for Server : ");
        fgets(buffer, 255, stdin);
        n = write(socket_fd, buffer, strlen(buffer));
        if(n<0){
            error("Write Error");
        }
        bzero(buffer, 255);
        n = read(socket_fd, buffer, 255);
        if(n<0){
            error("Read Error");
        }
        printf("\nServer : %s", buffer);
        int i = strncmp("Bye", buffer, 3);  //compare string , if buffer is Bye then close 
        if(i==0)
            break;
    }
    close(socket_fd);
    return 0;
}