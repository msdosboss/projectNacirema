#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "networking.h"


char *inputPort(int argc, char *argv[]){
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"--port") == 0 || strcmp(argv[i], "-p") == 0){
            i++;
            int port = atoi(argv[i]);
            if(port == 0){
                printf("that is not a valid port\n");
                return NULL;
            }
            return argv[i];
        }
    }
}

void bufferMalloc(struct Buffer *buffer, int n, int sizeEle){
    buffer->index = 0;
    buffer->size = n * sizeEle;
    buffer->data = malloc(n * sizeEle);
}

void writeInt(struct Buffer *buffer, uint32_t value){
    if(buffer->index + sizeof(int) > buffer->size){
        printf("\nbuffer is full!!\n");
        return;
    }
    *((uint32_t*)(buffer->data + buffer->index)) = value;
    buffer->index = buffer->index + sizeof(uint32_t);
}

uint32_t readInt(struct Buffer *buffer){
    if(buffer->index >= buffer->size){
        printf("Index is being reset to 0");
        buffer->index = 0;
    }
    printf("\nbuffer.index is %d", buffer->index);
    uint32_t value;
    value = *((uint32_t*)(buffer->data + buffer->index));
    buffer->index = buffer->index + sizeof(uint32_t);
    return value;
}

int main(){
    struct Buffer buffer;
    bufferMalloc((struct Buffer *) &buffer, 3, sizeof(uint32_t));
    writeInt((struct Buffer *) &buffer, 3);
    writeInt((struct Buffer *) &buffer, 6);
    writeInt((struct Buffer *) &buffer, 60);
    buffer.index = 0;
    printf("\n%d",readInt((struct Buffer *) &buffer));
    printf("\n%d",readInt((struct Buffer *) &buffer));
    printf("\n%d",readInt((struct Buffer *) &buffer));
}