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
    // char buffer[255];
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
    int num1, num2, ans, choice;
    S : 

    n = write(newsocket_fd, "Enter Choice :\n1 - Addition\n2 - Subtraction\n3 - Multiplication\n4 - Division\n5 - Reminder\n6 - Quit\n", strlen("Enter Choice :\n1 - Addition\n2 - Subtraction\n3 - Multiplication\n4 - Division\n5 - Reminder\n6 - Quit\n"));
    if(n<0){
        error("Write Error");
    }
    n = read(newsocket_fd, &choice, sizeof(int));
    if(n<0){ 
        error("Read Error");
    }
    printf("choice is %d\n", choice);
    if(choice == 6)
        goto Q;

    n =  write(newsocket_fd, "Enter Number 1 : ", strlen("Enter Number 1 : "));
    if(n<0){
        error("Write Error");
    }
    n = read(newsocket_fd, &num1, sizeof(int));
    if(n<0){
        error("Read Error");
    }
    printf("Number 1 : %d\n", num1);

    n =  write(newsocket_fd, "Enter Number 2 : ", strlen("Enter Number 2 : "));
    if(n<0){
        error("Write Error");
    }
    n = read(newsocket_fd, &num2, sizeof(int));
    if(n<0){
        error("Read Error");
    }
    printf("Number 2 : %d\n", num2);
    
    switch (choice){
    case 1: ans = num1+num2;
        break;
    case 2: ans = num1 - num2;
        break;
    case 3: ans = num1 * num2;
        break;
    case 4: ans = num1 / num2;
        break;
    case 5: ans = num1 % num2;
        break;
    default: goto Q;
        break;
    }
    printf("Calculated Answer : %d\n", ans);
    n = write(newsocket_fd, &ans, sizeof(int));
    if(n<0){
        error("Write Error");
    }
    goto S;
    Q : 
    close(newsocket_fd);
    close(socket_fd);
    return 0;
}