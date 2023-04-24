#include <stdlib.h>

typedef struct Buffer
{
    size_t size;
    char *data;
} Buffer;

typedef enum BufferError
{
    BUFFER_NO_ERROR = 0,
    BUFFER_ALLOCATION_FAILURE = 1
} BufferError;

Buffer createBuffer();
Buffer createBufferOfSize(size_t size);

void destroyBuffer(Buffer *buffer);
void bufferClear(Buffer *buffer);

Buffer *newBuffer();
Buffer *newBufferOfSize(size_t size);

void deleteBuffer(Buffer *buffer);

BufferError bufferSetSize(Buffer *buffer, size_t size);

BufferError bufferAppend(Buffer *buffer, char *data, size_t dataSize);
BufferError bufferAppendString(Buffer *buffer, char *string);
BufferError bufferAppendByte(Buffer *buffer, char byte);

BufferError bufferInsert(Buffer *buffer, size_t i, char *data, size_t dataSize);
BufferError bufferInsertString(Buffer *buffer, size_t i, char *string);
BufferError bufferInsertByte(Buffer *buffer, size_t i, char byte);

BufferError bufferSet(Buffer *buffer, size_t i, char *data, size_t dataSize);
BufferError bufferSetString(Buffer *buffer, size_t i, char *string);
BufferError bufferSetByte(Buffer *buffer, size_t i, char byte);

BufferError bufferRemove(Buffer *buffer, size_t i, size_t size);

// typedef struct Buffer
// {
//     size_t size;
//     char *data;
// } Buffer;

// typedef enum BufferError
// {
//     BUFFER_NO_ERROR = 0,
//     BUFFER_ALLOCATION_FAILURE = 1
// } BufferError;

// Buffer createBuffer();
// Buffer createBufferWithCapacity(size_t capacity);
// inline void destroyBuffer(Buffer buffer);

// Buffer *newBuffer();
// Buffer *newBufferWithCapacity(size_t capacity);
// void deleteBuffer(Buffer *buffer);

// BufferError bufferTrimToSize(Buffer *buffer);
// BufferError bufferShrink(Buffer *buffer, size_t newSize);

// BufferError bufferEnsureCapacity(Buffer *buffer, size_t minCapacity);
// BufferError bufferSetCapacity(Buffer *buffer, size_t capacity);

// BufferError bufferSet(Buffer *buffer, size_t i, const char *data, size_t dataSize);
// BufferError bufferSetString(Buffer *buffer, size_t i, const char *string);

// BufferError bufferAppend(Buffer *buffer, const char *data, size_t dataSize);
// BufferError bufferAppendString(Buffer *buffer, const char *string);
// BufferError bufferAppendByte(Buffer *buffer, char byte);

// BufferError bufferInsert(Buffer *buffer, size_t i, char *data, size_t dataSize);
// BufferError bufferInsertString(Buffer *buffer, size_t i, char *string);
// BufferError bufferInsertByte(Buffer *buffer, size_t i, char byte);

// void bufferPop(Buffer *buffer, size_t count);

// char bufferPeek(Buffer *buffer, size_t depth);

// void bufferRemove(Buffer *buffer, size_t i, size_t length);