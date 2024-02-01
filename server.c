#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

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
    
    const int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket failed to create\n");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if(bind(fd, (struct sockaddr*) &addr, sizeof(addr))){
        perror("Bind failed");
        return -2;
    }
    socklen_t addrLen = sizeof(addr);

    if(listen(fd, 10)){
        perror("Listen failed");
        return -3;
    }
    //accept connections
    while(1){
        struct sockaddr_storage caddr;
        socklen_t caddrLen = sizeof(caddr);
        int cfd = accept(fd, (struct sockaddr*) &caddr, &caddrLen);
        if(cfd == -1){
            perror("accept failed");
            return -4;
        }
        char buf[1024];
        if(recv(cfd, buf, sizeof(buf), 0) > 0){
            send(cfd, "Orca", sizeof("Orca") + 1, 0);
        }
        printf("%s\n", buf);
        close(cfd);
        if(strcmp(buf,"close") == 0){
            break;
        }
    }
    close(fd);
    return 0;
}