// to run use
// filename portno. 
//eg.
// ./Server 9999
//portno >=1024

#include<stdio.h>  //input output library
#include<stdlib.h>  // atoi function
#include<string.h>  //strlen function
#include<unistd.h>  //for read write close functions
#include<sys/types.h>  
#include<sys/socket.h>
#include<netinet/in.h>

void error(const char* msg){
    perror(msg);
    exit(1);
}
int main(int argc, char* argv[]){
    if(argc<2){
        fprintf(stderr, "Port no. not found. Connection terminating\n");
        exit(1);
    }
    int socket_fd, newsocket_fd, port_no, n;
    char buffer[255];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t clientlength;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);  //sockstream for TCP and sock_dgram for udp 0 default protocol for tcp
    if (socket_fd<0){  //-1 if error else 0
        error("Error in opening a socket");
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));  // cleaar memory (variable) so that if anything is present in it removed
    port_no = atoi(argv[1]); //atoi convert string to int
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no);  //host to network short (also a function host to netword long(htol))
    if(bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){  //typecasting sockaddr_in  into sockaddr as both are different structures and then bindng them (assigning address to socket)
        error("binding error");
    }
    listen(socket_fd, 4); //maximum 4 client can connect to server
    clientlength = sizeof(client_addr);
    newsocket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &clientlength); //accept
    if(newsocket_fd<0){
        error("Accept failed");
    }
    //communication
    while(1){
        bzero(buffer, 255); //clearing buffer
        n = read(newsocket_fd, buffer, 255);
        if(n<0)
            error("Read Error");
        printf("client : %s\n", buffer);
        bzero(buffer, 255); 
        printf("Enter Server Reply (Bye to exit chat) : ");
        fgets(buffer, 255, stdin); //function reads bytes from stream, for here input stream (stdin) belong to stdio.h library
        n = write(newsocket_fd, buffer, strlen(buffer));
        if(n<0)
            error("Write error");
        int i = strncmp("Bye", buffer, 3);  //compare string , if buffer is Bye then close 
        if(i==0)
            break;
        printf("\n");
    }
    close(newsocket_fd);
    close(socket_fd);
    return 0;
}