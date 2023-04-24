#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<unistd.h>  
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
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if (socket_fd<0){ 
        error("Error in opening a socket");
    }
    bzero((char*) &serv_addr, sizeof(serv_addr)); 
    port_no = atoi(argv[1]); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no);  
    if(bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){  
        error("binding error");
    }
    listen(socket_fd, 4); 
    clientlength = sizeof(client_addr);
    newsocket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &clientlength); 
    if(newsocket_fd<0){
        error("Accept failed");
    }
    FILE *f;
    int words;
    f = fopen("RecievedFile.txt", "a");   //a -> appends data if there is a file woth same name exits w-> overwrites
    int ch = 0;
    read(newsocket_fd, &words, sizeof(int));
    while(ch!=words){
        read(newsocket_fd, buffer, 255);
        fprintf(f, "%s ", buffer);  //after words we will have whitespace so space after %s
        ch++;
    }
    printf("File Recieved!!!"); 
    close(newsocket_fd);
    close(socket_fd);
    return 0;
}