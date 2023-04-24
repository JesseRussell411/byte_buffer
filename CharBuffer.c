#include <stdlib.h>
#include "Buffer.h"

typedef struct CharBuffer {
    Buffer block;
} CharBuffer;

CharBuffer createCharBufferOfSize(size_t size) {
    CharBuffer result;
    result.block = createBufferOfSize(size + 1);
    bufferSetByte(&result.block, 0, '\0');
    return result;
}

CharBuffer createCharBuffer() {
    return createCharBufferOfSize(0);
}

void charBufferClear(CharBuffer *buffer) {
    bufferClear(&buffer->block);
}

CharBuffer *newCharBufferOfSize(size_t size) {
    // allocate heap space for the charbuffer
    CharBuffer *result = malloc(sizeof(CharBuffer));

    // check if allocation failed
    if (NULL == result){
        return NULL;
    }

    // create char buffer
    *result = createCharBufferOfSize(size);

    // check if char buffer creation was successful
    if (result->block.size != size + 1){
        destroyBuffer(&(result->block));
        free(result);
        return NULL;
    }

    // TODO there may be a faster way to do this. look into using memcpy
    // initialize to spaces
    for (size_t i = 0; i < size; i++){
        result->block.data[i] = ' ';
    }

    // add null terminator to end
    result->block.data[size] = '\0';

    return result;
}

size_t charBufferLength(const CharBuffer *charBuffer) {
    return charBuffer->block.size - 1;
}

char *charBufferAsString(const CharBuffer *charBuffer) {
    return charBuffer->block.data;
}
