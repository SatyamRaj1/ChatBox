//we will transfer a file character by character
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h> // to use ispace function

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
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);  
    serv_addr.sin_port = htons(port_no); 
    if(connect(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Connection error");
    }
    bzero(buffer, 255);
    FILE *f; // file pointer
    int words = 0;
    char c;
    char filename[30];
    printf("Enter File Name : ");
    scanf("%s", filename);
    printf("\n");
    f = fopen(filename, "r"); //file pointer will point to first character in file
    if(f==NULL){
        error("Could not open file");
    }
    while((c = getc(f ))!=EOF){ //getc will point to next character to f pointer (EOF -> End of file)
        fscanf(f, "%s", buffer);
        if(isspace(c) || c == '\t')  //if a whitespace or tab is there then word count is increased
            words++; 
        // printf("%c", c);
    }
    write(socket_fd, &words, sizeof(int));  // to send no. of words
    rewind(f);  // as file pointer is now at the end of file so we need it to the start again
    char ch;
    while(ch!=EOF){
        fscanf(f, "%s", buffer);
        write(socket_fd, buffer, sizeof(buffer));
        ch = fgetc(f);
    }
    printf("The file is sent to client");




    close(socket_fd);
    return 0;
}