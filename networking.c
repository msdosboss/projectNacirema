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

writeBitPackerInit(struct bitPackWrite * packer, int size){
    packer->scratch = 0;
    packer->scratchBits = 0;
    packer->totalBits = 0;
    packer->wordIndex = 0;
    packer->buffer = malloc(sizeof(uint32_t) * size);
}

readBitPackerInit(struct bitPackWrite * packer){
    packer->scratch = 0;
    packer->scratchBits = 0;
    packer->numBitsRead = 0;
    packer->wordIndex = 0;
}

void writeBitPacker(struct bitPackWrite * packer, int size, uint32_t value){
    packer->scratch = packer->scratch | (value & ((1 << size) - 1) << packer->scratchBits);
    packer->scratchBits += size;
    printf("\n%d\n", packer->scratch);
    if(packer->scratchBits >= 32){
        *(packer->buffer + packer->wordIndex) = packer->scratch;
        packer->scratch = packer->scratch << 32;
        packer->scratchBits -= 32;
        packer->wordIndex++;
    }
    packer->totalBits += size;   
}

uint32_t readBitPacker(struct bitPackWrite * packer, int size){
    uint32_t value;
    memset(&value, 0, 4);
    if(packer->scratchBits < size){
        packer->scratch = (*(packer->buffer + packer->wordIndex) << packer->scratchBits);
        packer->scratchBits += 32;
        packer->wordIndex++;
    }
    value = packer->scratch & ((1 << size) - 1);
    packer->numBitsRead += size;
    packer->scratch >>= size;
    packer->scratchBits -= size;
    return value;
    
}


int main(){
    struct bitPackWrite packer;
    writeBitPackerInit(&packer, 4);
    writeBitPacker(&packer, 2, 2);
    writeBitPacker(&packer, 3, 7);
    writeBitPacker(&packer, 3, 6);
    readBitPackerInit(&packer);
    //printf("\n%d\n", readBitPacker(&packer, 3));
    //printf("\n%d\n", readBitPacker(&packer, 2));
    //printf("\n%d\n", readBitPacker(&packer, 3));
}