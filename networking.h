#if !defined NETWORKING

struct packet{
    //struct packetHeader;
    //struct segHeader;
    //struct segType;
    //struct ipcHeader;
};


struct Buffer{
    uint8_t *data;
    int size;
    int index;
};

char *inputPort(int, char*[]);
void bufferMalloc(struct Buffer *, int, int);
void writeInt(struct Buffer *, uint32_t);
uint32_t readInt(struct Buffer *);

#endif
#define NETWORKING 1