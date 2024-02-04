#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int port = 5456;
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"--port") == 0 || strcmp(argv[i], "-p") == 0){
            i++;
            port = atoi(argv[i]);
            if(port == 0){
                printf("that is not a valid port");
                return -1;
            }
        }
    }



    while(1){
        const int fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd == -1){
            perror("socket create failed");
            return -1;
        }
        struct sockaddr_in addr = { 0 };
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        char addrstr[NI_MAXHOST + NI_MAXSERV + 1];
        snprintf(addrstr, sizeof(addrstr), "10.0.0.193:%d", port);
    
        //convert from string to binary storing the addr in addr.sin_addr
        inet_pton(AF_INET, addrstr, &addr.sin_addr);

        if(connect(fd, (struct sockaddr*)&addr, sizeof(addr))){
            perror("can not connect");
            return -2;
        }


        char msg[1024];
        gets(msg);
        if(strcmp(msg, "closeclient") == 0)
            break;
        
        if(send(fd, msg, strlen(msg) + 1, 0) < 0){
            perror("Failed to send");
            return -3;
        }
 
        char response[1024];
        if(recv(fd, response, sizeof(response), 0) < 0){
            perror("Failed to recv");
            return -4;
        }
        printf("server response is %s\n", response);

        close(fd);    
    }
    
    return 0;
    
}