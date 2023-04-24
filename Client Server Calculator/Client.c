 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <netdb.h> 

void error(const char* msg){
    perror(msg);
    exit(1);
}
int main(int argc, char* argv[]){
    if(argc<3){
        fprintf(stderr, "Usage %s hostipaddress portnumber\n", argv[0]); 
        exit(1);
    }
    int socket_fd, port_no, n;
    char buffer[1000];
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
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);  
    serv_addr.sin_port = htons(port_no); 
    if(connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Connection error");
    }
    int num1, num2, choice, ans;
    S : 
    bzero(buffer, 1000);
    n = read(socket_fd, buffer, 1000);
    if(n<0) 
        error("Read Error");
    printf("Server -\n%s", buffer);
    scanf("%d", &choice);
    write(socket_fd, &choice, sizeof(int));
    if(choice == 6)
        goto Q;

    bzero(buffer, 1000);
    n = read(socket_fd, buffer, 1000);
    if(n<0)
        error("Read Error");
    printf("Server - %s", buffer);
    scanf("%d", &num1);
    write(socket_fd, &num1, sizeof(int));

    bzero(buffer, 1000);
    n = read(socket_fd, buffer, 1000);
    if(n<0) 
        error("Read Error");
    printf("Server - %s", buffer);
    scanf("%d", &num2);
    write(socket_fd, &num2, sizeof(int));
    
    n = read(socket_fd, &ans, sizeof(int));
    printf("Answer : %d \n", ans);
    if(n<0) 
        error("Read Error");
    goto S;

    Q : 
    
    close(socket_fd);
    return 0;
}