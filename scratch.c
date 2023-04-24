#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "Buffer.h"

char rotateByteRight(char byte){
    return (byte >> 1) | (byte << 7);
}

char rotateByteLeft(char byte) {
    return (byte << 1) | (byte >> 7);
}

void printBits(unsigned char* bytes, size_t size) {
    for(size_t i = size; i > 0; i--){
        size_t chunk = bytes[i - 1];
        for(size_t j = 8; j > 0; j--){
            if (chunk & 0x80){
                printf("1");
            } else {
                printf("0");
            }
            chunk <<= 1;
        }
    }
}

void printByteBits(char byte){
    printBits(&byte, 1);
}




int main() {
    Buffer *buf = newBuffer();
    bufferSetString(buf, 0, "0123456789");
    bufferAppendByte(buf, '\0');
    bufferInsertByte(buf, 0, '9');

    printf("%s\n", buf->data);

    bufferInsert(buf, 2, buf->data + 2, 3);

    printf("%s\n", buf->data);

    deleteBuffer(buf);



    return 0;
}