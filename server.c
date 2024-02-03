#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <mysql/mysql.h>            //This is how I have been compiling with this library gcc -o server -L/usr/lib/mysql -lmysqlclient server.c

char* sqlConnector(){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    if(!(conn = mysql_init(0))){
        return "failed to connect";
    }
    if(!mysql_real_connect(
        conn,   //connection
        "127.0.0.1",     //host
        "sqlServer",    //user
        "sqlpass",      //pass
        "projectNacirema",   //default database
        3306,       //port
        NULL,       //path to socket file
        0           //flags

    )){
        fprintf(stderr, "Error connecting to Server: %s\n", mysql_error(conn));
        mysql_close(conn);
        return "failed to connect2";
    }
    char buf[256];
    if(mysql_query(conn, "SELECT * FROM orca;")){
        fprintf(stderr, "Error sending query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return "failed to query";
    }
    if((res = mysql_use_result(conn)) == NULL){
        fprintf(stderr, "Error using result: %s\n", mysql_error(conn));
        mysql_close(conn);
        return "failed to use result";
    }
    printf("query results are:\n");
    while((row = mysql_fetch_row(res)) != NULL){
        for(int i = 0; i < mysql_num_fields(res); i++){
            printf("\n%s", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    mysql_close(conn);

    return "the end";
}


int main(int argc, char *argv[]){
    printf("%s\n",sqlConnector());
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