#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <mysql/mysql.h>            //This is how I have been compiling with this library gcc -o server -L/usr/lib/mysql -lmysqlclient server.c
#include "networking.h"

char **sqlQuery(MYSQL *conn, const char *query){
    char **queryResults;
    MYSQL_ROW row;
    MYSQL_RES *res;
    queryResults = malloc(sizeof(char *));
    if(mysql_query(conn, query)){                   //SELECT * FROM orca;
        fprintf(stderr, "Error sending query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    if((res = mysql_use_result(conn)) == NULL){
        fprintf(stderr, "Error using result: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    int ii;
    printf("query results are:\n");
    while((row = mysql_fetch_row(res)) != NULL){
        for(int i = 0; i < mysql_num_fields(res); i++){
            ii = i;
            queryResults = realloc(queryResults, sizeof(char *) * (i + 1));
            if(!row[i]){
                queryResults[i] = malloc(sizeof(char) * 5);
                strcpy(queryResults[i],"NULL");
                continue;
            }
            
            queryResults[i] = malloc(sizeof(char) * (strlen(row[i]) + 1));
            printf("\nis this for loop running?");
            strcpy(queryResults[i], row[i]);
            printf("\n%s", queryResults[i]);
            
        }
        printf("\n");
    }
    queryResults = realloc(queryResults, sizeof(char *) * (ii + 2));
    queryResults[ii + 1] = NULL;
    printf("\norca: %s\n", queryResults[0]);
    return queryResults;

    
}

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
    char **queryResults = sqlQuery(conn, "SELECT sampletext FROM orca;");
    for(int i = 0; queryResults[i] != NULL; i++){
        printf("queryResults[%d] = %s\n", i, queryResults[i]);
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
    
    struct bitPackWrite packer;
    writeBitPackerInit(&packer, 4);
    writeBitPacker(&packer, 2, 2);
    writeBitPacker(&packer, 3, 7);
    writeBitPacker(&packer, 3, 6);
    
    const int fd = socket(AF_INET, SOCK_DGRAM, 0); //SOCK_STREAM
    if(fd == -1){
        perror("socket failed to create\n");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;              //ipv4
    addr.sin_addr.s_addr = INADDR_ANY;      //setting bound ip address
    addr.sin_port = htons(port);            //port

    if(bind(fd, (struct sockaddr*) &addr, sizeof(addr))){
        perror("Bind failed");
        return -2;
    }
    socklen_t addrLen = sizeof(addr);

    //accept connections
    while(1){
        struct sockaddr_in caddr;
        memset(&caddr, 0, sizeof(caddr));
        socklen_t caddrLen = sizeof(caddr);
        char buf[1024];
        printf("\ntotalBits is %d\n", packer.totalBits);
        if(recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) &caddr, &caddrLen) > 0){
            sendto(fd, packer.buffer, sizeof(packer.buffer) + 1, 0, (struct sockaddr *) &caddr, caddrLen);
            sendto(fd, &(packer.totalBits), sizeof(packer.totalBits) + 1, 0, (struct sockaddr *) &caddr, caddrLen);
            //sendto(fd, "Orca", sizeof("Orca") + 1, 0, (struct sockaddr *) &caddr, caddrLen);
        }
        printf("%s\n", buf);
        if(strcmp(buf,"close") == 0){
            break;
        }
    }
    close(fd);
    return 0;
}