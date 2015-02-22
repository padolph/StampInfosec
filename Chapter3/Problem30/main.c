#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void encrypt (uint32_t* v, uint32_t* k);
void decrypt (uint32_t* v, uint32_t* k);
int readFile(char *name, char **buffer, unsigned long *fileLen);
int writeFile(char *name, char *buffer, unsigned long len);


int main(int argc, char* argv[])
{
    unsigned long fileLen;
    char *buffer;
    uint32_t key[4] = {0x9e3779b9, 0xC6EF3720, 0xC6E23720, 0x9e3679b9};
    char *pBuf, *pBufEnd;
    int res;

    res = readFile("alice.jpg", &buffer, &fileLen);
    if (!res) {
        return 0;
    }
    printf("File size: %d\n", fileLen);

    pBuf = buffer + 10 * 8;  // skip first 10 64-bit blocks
    pBufEnd = buffer + fileLen;
    while (pBuf < pBufEnd - 8) // ignore the last bit of the file so we don't have to pad
    {
        // encrypt one 64-bit block at a time
        encrypt((uint32_t *)pBuf, key);
        pBuf += 8;
    }

    writeFile("alice_enc.jpg", buffer, fileLen);

    free(buffer);
    return 0;
}

// Reference TEA from http://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
void encrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0, i;           /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i < 32; i++) {                       /* basic cycle start */
        sum += delta;
        v0 += ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        v1 += ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}
 
void decrypt (uint32_t* v, uint32_t* k) {
    uint32_t v0=v[0], v1=v[1], sum=0xC6EF3720, i;  /* set up */
    uint32_t delta=0x9e3779b9;                     /* a key schedule constant */
    uint32_t k0=k[0], k1=k[1], k2=k[2], k3=k[3];   /* cache key */
    for (i=0; i<32; i++) {                         /* basic cycle start */
        v1 -= ((v0<<4) + k2) ^ (v0 + sum) ^ ((v0>>5) + k3);
        v0 -= ((v1<<4) + k0) ^ (v1 + sum) ^ ((v1>>5) + k1);
        sum -= delta;
    }                                              /* end cycle */
    v[0]=v0; v[1]=v1;
}

int readFile(char *name, char **buffer, unsigned long *fileLen)
{
    FILE *file;
    //Open file
    file = fopen(name, "rb");
    if (!file)
    {
        fprintf(stderr, "Unable to open file %s", name);
        return 0;
    }
    //Get file length
    fseek(file, 0, SEEK_END);
    *fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);
    //Allocate memory
    *buffer=(char *)malloc(*fileLen+1);
    if (!*buffer)
    {
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
    }
    //Read file contents into buffer
    fread(*buffer, *fileLen, 1, file);
    fclose(file);
    return 1;
}

int writeFile(char *name, char *buffer, unsigned long len) {
    FILE *file = fopen(name, "wb");
    if (!file)
    {
        fprintf(stderr, "Unable to open file %d", name);
        return 0;
    }
    fwrite(buffer, len, sizeof(char), file);
    return 1;
}
