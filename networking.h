#if !defined NETWORKING
char *inputPort(int, char*[]);

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

#endif
#define NETWORKING 1