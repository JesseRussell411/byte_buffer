#include <stdlib.h>
#include <string.h>

#include "Buffer.h"

static size_t minsize_t(size_t a, size_t b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

static size_t maxsize_t(size_t a, size_t b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

static int memoryOverlaps(const char *blockA, size_t sizeA, const char *blockB, size_t sizeB)
{
    if (blockA + sizeA < blockB)
        return 0;
    if (blockB + sizeB < blockA)
        return 0;
    return 1;
}

Buffer createBuffer() {
    return createBufferOfSize(0);
}

Buffer createBufferOfSize(size_t size) {
    Buffer buffer;
    if (0 == size) {
        buffer.data = NULL;
        buffer.size = size;
        return buffer;
    }

    char *data = malloc(size);
    
    if (NULL == data){
        buffer.data = NULL;
        buffer.size = 0;
        return buffer;
    } else {
        buffer.data;
        buffer.size = size;
        return buffer;
    }
}

void destroyBuffer(Buffer *buffer) {
    bufferClear(buffer);
}

void bufferClear(Buffer *buffer) {
    buffer->size = 0;
    if (NULL != buffer->data) {
        free(buffer->data);
        buffer->data = NULL;
    }
}

Buffer *newBuffer()
{
    return newBufferOfSize(0);
}

Buffer *newBufferOfSize(size_t size)
{
    Buffer *buffer = malloc(sizeof(Buffer));
    if (NULL == buffer)
    {
        return NULL;
    }

    *buffer = createBufferOfSize(size);
    if (buffer->size != size){
        deleteBuffer(buffer);
        return NULL;
    }
    return buffer;
}

void deleteBuffer(Buffer *buffer)
{
    if (NULL == buffer)
    {
        return;
    }

    if (NULL != buffer->data)
    {
        free(buffer->data);
    }

    free(buffer);
}

BufferError bufferSetSize(Buffer *buffer, size_t size)
{
    if (size == buffer->size)
        return BUFFER_NO_ERROR;

    char *newData = realloc(buffer->data, size);

    if (NULL == newData)
    {
        return BUFFER_ALLOCATION_FAILURE;
    }

    buffer->data = newData;
    buffer->size = size;

    return BUFFER_NO_ERROR;
}

BufferError bufferAppend(Buffer *buffer, char *data, size_t dataSize)
{
    size_t i = buffer->size;
    BufferError error = bufferSetSize(buffer, buffer->size + dataSize);
    if (error)
        return error;

    // use memcpy because the block of memory being copied to is uninitialized so the source shouldn't be in that block
    memcpy(buffer->data + i, data, dataSize);

    return BUFFER_NO_ERROR;
}

BufferError bufferAppendString(Buffer *buffer, char *string)
{
    size_t size = strlen(string);
    return bufferAppend(buffer, string, size);
}

BufferError bufferAppendByte(Buffer *buffer, char byte)
{
    return bufferAppend(buffer, &byte, 1);
}

BufferError bufferInsert(Buffer *buffer, size_t i, char *data, size_t dataSize)
// TODO insertion of data following insertion location
{
    char *iPtr = buffer->data + i;
    char *followingDst = iPtr + dataSize;
    size_t followingSize = buffer->size - i;

    if (data == iPtr) {
        // data equals destination

        // resize buffer
        BufferError error = bufferSetSize(buffer, buffer->size + dataSize);
        if (error) return error;

        memmove(followingDst, iPtr, followingSize);
    } else if (memoryOverlaps(iPtr, followingSize + dataSize, data, dataSize)) {
        // data will be overwritten by moving following data

        // create temp memory to copy data into
        char *temp = malloc(dataSize);
        if (NULL == temp) return BUFFER_ALLOCATION_FAILURE;

        // resize buffer
        BufferError error = bufferSetSize(buffer, buffer->size + dataSize);
        if (error){
            free(temp);
            return error;
        }

        memcpy(temp, data, dataSize);
        memmove(followingDst, iPtr, followingSize);
        memcpy(iPtr, temp, dataSize);

        free(temp);
    } else {
        // data does not overlap

        // resize buffer
        BufferError error = bufferSetSize(buffer, buffer->size + dataSize);
        if (error) return error;

        memmove(followingDst, iPtr, followingSize);
        memcpy(iPtr, data, dataSize);
    }
}

BufferError bufferInsertString(Buffer *buffer, size_t i, char *string)
{
    size_t size = strlen(string);
    return bufferInsert(buffer, i, string, size);
}

BufferError bufferInsertByte(Buffer *buffer, size_t i, char byte)
{
    return bufferInsert(buffer, i, &byte, 1);
}

BufferError bufferSet(Buffer *buffer, size_t i, char *data, size_t dataSize)
{
    BufferError error = bufferSetSize(buffer, maxsize_t(buffer->size, i + dataSize));
    if (error) return error;

    // use memmove because the source could be inside of the destination block.
    memmove(buffer->data + i, data, dataSize);

    return BUFFER_NO_ERROR;
}

BufferError bufferSetString(Buffer *buffer, size_t i, char *string)
{
    size_t size = strlen(string);
    return bufferSet(buffer, i, string, size);
}

BufferError bufferSetByte(Buffer *buffer, size_t i, char byte)
{
    return bufferSet(buffer, i, &byte, 1);
}

BufferError bufferRemove(Buffer *buffer, size_t i, size_t size)
{
    size = minsize_t(size, buffer->size - i);

    char *dst = buffer->data + i;
    char *src = dst + size;
    memmove(dst, src, buffer->size - (size + i));
    size_t newSize = buffer->size - size;

    char *newData = realloc(buffer->data, newSize);
    buffer->size = newSize;

    if (NULL != newData) buffer->data = newData;

    return BUFFER_NO_ERROR;
}

inline int bufferIsEmpty(const Buffer buffer) {
    return 0 == buffer.size;
}

inline int bufferNonEmpty(const Buffer buffer) {
    return !bufferIsEmpty(buffer);
}



// static const size_t initial_size = 0;

// static size_t minsize_t(size_t a, size_t b)
// {
//     if (a < b)
//     {
//         return a;
//     }
//     else
//     {
//         return b;
//     }
// }

// static size_t maxsize_t(size_t a, size_t b)
// {
//     if (a > b)
//     {
//         return a;
//     }
//     else
//     {
//         return b;
//     }
// }

// Buffer createBuffer()
// {
//     return createBufferWithSize(initial_size);
// }

// Buffer createBufferWithSize(size_t size)
// {
//     Buffer result;
//     result.data = malloc(size);

//     if (NULL == result.data)
//     {
//         result.size = 0;
//     }
//     else
//     {
//         result.size = size;
//     }

//     result.size = 0;

//     return result;
// }

// void destroyBuffer(Buffer buffer)
// {
//     free(buffer.data);
// }

// Buffer *newBuffer()
// {
//     return newBufferWithSize(initial_size);
// }

// Buffer *newBufferWithSize(size_t size)
// {
//     Buffer *result = (Buffer *)malloc(sizeof(Buffer));
//     if (NULL == result)
//     {
//         return NULL;
//     }

//     *result = createBufferWithSize(size);

//     if (NULL == result->data)
//     {
//         free(result);
//         return NULL;
//     }

//     return result;
// }

// void deleteBuffer(Buffer *buffer)
// {
//     destroyBuffer(*buffer);
//     free(buffer);
// }

// BufferError bufferAppend(Buffer *buffer, const char *data, size_t dataSize)
// {
//     BufferError error = bufferEnsureSize(buffer, buffer->size + dataSize);

//     if (error)
//     {
//         return error;
//     }

//     memmove(buffer->data + buffer->size, data, dataSize);

//     buffer->size += dataSize;

//     return BUFFER_NO_ERROR;
// }

// BufferError bufferTrimToSize(Buffer *buffer)
// {
//     return bufferSetSize(buffer, buffer->size);
// }
// BufferError bufferShrink(Buffer *buffer, size_t newSize)
// {
//     if (newSize < buffer->size)
//     {
//         return bufferSetSize(buffer, newSize);
//     }

//     return BUFFER_NO_ERROR;
// }

// BufferError bufferEnsureSize(Buffer *buffer, size_t minSize)
// {
//     if (buffer->size >= minSize)
//     {
//         return BUFFER_NO_ERROR;
//     }

//     if (buffer->size > (SIZE_MAX / 2))
//     {
//         buffer->size = minSize;
//     }

//     size_t newSize = buffer->size;
//     if (newSize <= 0)
//     {
//         newSize = 1;
//     }

//     // will take at most 64 iterations
//     while (newSize < minSize)
//     {
//         newSize *= 2;
//     }

//     return bufferSetSize(buffer, newSize);
// }

// BufferError bufferSetSize(Buffer *buffer, size_t size)
// {
//     char *newData = realloc(buffer->data, size);
//     if (NULL == newData)
//     {
//         return BUFFER_ALLOCATION_FAILURE;
//     }

//     buffer->data = newData;
//     buffer->size = size;

//     return BUFFER_NO_ERROR;
// }

// BufferError bufferSet(Buffer *buffer, size_t i, const char *data, size_t dataSize)
// {
//     BufferError error = bufferEnsureSize(buffer, i + dataSize);
//     if (error)
//     {
//         return error;
//     }

//     memmove(buffer->data + i, data, dataSize);

//     size_t newSize = i + dataSize;
//     if (newSize > buffer->size)
//     {
//         buffer->size = newSize;
//     }

//     return BUFFER_NO_ERROR;
// }

// BufferError bufferAppendString(Buffer *buffer, const char *string)
// {
//     size_t dataSize = strlen(string);
//     return bufferAppend(buffer, string, dataSize);
// }

// BufferError bufferSetString(Buffer *buffer, size_t i, const char *string)
// {
//     size_t dataSize = strlen(string);
//     return bufferSet(buffer, i, string, dataSize);
// }

// BufferError bufferAppendByte(Buffer *buffer, char byte)
// {
//     return bufferAppend(buffer, &byte, 1);
// }

// BufferError bufferInsert(Buffer *buffer, size_t i, char *data, size_t dataSize)
// {
//     if (i >= buffer->size)
//     {
//         BufferError error = bufferEnsureSize(buffer, i + dataSize);
//         if (error)
//         {
//             return error;
//         }
//         memmove(buffer->data + i, data, dataSize);
//     }
//     else
//     {
//         BufferError error = bufferEnsureSize(buffer, buffer->size + dataSize);
//         if (error)
//         {
//             return error;
//         }
//         memmove(buffer->data + i + dataSize, buffer->data + i, buffer->size - i);
//         memmove(buffer->data + i, data, dataSize);
//     }

//     return BUFFER_NO_ERROR;
// }

// BufferError bufferInsertString(Buffer *buffer, size_t i, char *string)
// {
//     size_t dataSize = strlen(string);
//     return bufferInsert(buffer, i, string, dataSize);
// }

// BufferError bufferInsertByte(Buffer *buffer, size_t i, char byte)
// {
//     return bufferInsert(buffer, i, &byte, 1);
// }

// void bufferPop(Buffer *buffer, size_t count)
// {
//     if (count < buffer->size)
//     {
//         buffer->size -= count;
//     }
//     else
//     {
//         buffer->size = 0;
//     }
// }

// char bufferPeek(Buffer *buffer, size_t depth)
// {
//     return buffer->data[buffer->size - depth - 1];
// }

// void bufferRemove(Buffer *buffer, size_t i, size_t length)
// {
//     if (i < buffer->size)
//     {
//         length = minsize_t(buffer->size - i, length);
//         char *dst = buffer->data + i;
//         char *src = dst + length;
//         memmove(dst, src, buffer->size - (i + length));
//         buffer->size -= length;
//     }
// }