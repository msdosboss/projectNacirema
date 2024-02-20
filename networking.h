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

struct bitPackWrite{
    uint64_t scratch;
    int totalBits;
    int numBitsRead;
    int scratchBits;
    int wordIndex;
    uint32_t * buffer;
};

char *inputPort(int, char*[]);
void bufferMalloc(struct Buffer *, int, int);
void writeInt(struct Buffer *, uint32_t);
uint32_t readInt(struct Buffer *);
void writeBitPackerInit(struct bitPackWrite *, int);
void readBitPackerInit(struct bitPackWrite *);
void writeBitPacker(struct bitPackWrite *, int, uint32_t);
uint32_t readBitPacker(struct bitPackWrite *, int);

#endif
#define NETWORKING 1