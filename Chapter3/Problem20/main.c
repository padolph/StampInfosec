#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

void encryptBlock(uint32_t *v, uint32_t *k);
void decryptBlock (uint32_t* v, uint32_t* k);
bool arraysEqual(uint8_t data1[], uint8_t data2[], size_t len);
void arraysCopy(uint8_t dst[], uint8_t src[], size_t len);
void encryptECB(uint32_t *data, size_t nBlocks, uint32_t *key);
void decryptECB(uint32_t *data, size_t nBlocks, uint32_t *key);
void encryptCBC(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv);
void decryptCBC(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv);
void encryptCTR(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv);
void decryptCTR(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv);

int main(int argc, char* argv[])
{
    uint8_t key[] = {0xa5, 0x6b, 0xab, 0xcd, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xab, 0xcd, 0xef, 0x01};

    // One block
    uint8_t blockRef[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    uint8_t block[]    = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    encryptBlock((uint32_t *)block, (uint32_t *)key);
    decryptBlock((uint32_t *)block, (uint32_t *)key);
    assert(arraysEqual(blockRef, block, 8));

    const unsigned char plaintext[] = "Four score and seven years ago, our fathers brought forth on this continent, "
                                      "a new nation, conceived in Liberty, and dedicated to the proposition that all "
                                      "men are created equal.";
    size_t len = sizeof(plaintext);
    uint8_t data[len];

    // ECB
    arraysCopy(data, (uint8_t *)plaintext, len);
    encryptECB((uint32_t *)data, len/4, (uint32_t *)key);
    decryptECB((uint32_t *)data, len/4, (uint32_t *)key);
    assert(arraysEqual((uint8_t *)plaintext, data, len));

    // CBC
    uint32_t iv[2] = {0xffffffff,0xffffffff};
    arraysCopy(data, (uint8_t *)plaintext, len);
    encryptCBC((uint32_t *)data, len/4, (uint32_t *)key, iv);
    decryptCBC((uint32_t *)data, len/4, (uint32_t *)key, iv);
    assert(arraysEqual((uint8_t *)plaintext, data, len));

    // CTR
    arraysCopy(data, (uint8_t *)plaintext, len);
    encryptCTR((uint32_t *)data, len/4, (uint32_t *)key, iv);
    decryptCTR((uint32_t *)data, len/4, (uint32_t *)key, iv);
    assert(arraysEqual((uint8_t *)plaintext, data, len));

    return 0;
}

bool arraysEqual(uint8_t data1[], uint8_t data2[], size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        if (data1[i] != data2[i])
            return false;
    }
    return true;
}

void arraysCopy(uint8_t dst[], uint8_t src[], size_t len)
{
    for (size_t i = 0; i < len; ++i)
        dst[i] = src[i];
}

void encryptCTR(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv)
{
    uint32_t *pBuf = data;
    uint32_t *pBufEnd = data + len;
    uint64_t counter = 0;
    uint32_t buf[2];
    while (pBuf < pBufEnd)
    {
        buf[0] = iv[0] + (uint32_t)((counter & 0xffff0000) >> 32);
        buf[1] = iv[1] + (uint32_t)(counter & 0x0000ffff);
        encryptBlock(buf, key);
        pBuf[0] ^= buf[0];
        pBuf[1] ^= buf[1];
        counter++;
        pBuf += 2;
    }
}

void decryptCTR(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv)
{
    // CTR decryption is the same as encryption
    encryptCTR(data, len, key, iv);
}

void encryptCBC(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv)
{
    uint32_t *pBuf = data;
    uint32_t *pBufEnd = data + len;

    pBuf[0] ^= iv[0];
    pBuf[1] ^= iv[1];
    encryptBlock(pBuf, key);
    uint32_t c0 = pBuf[0];
    uint32_t c1 = pBuf[1];
    pBuf += 2;

    while (pBuf < pBufEnd)
    {
        pBuf[0] ^= c0;
        pBuf[1] ^= c1;
        encryptBlock(pBuf, key);
        c0 = pBuf[0];
        c1 = pBuf[1];
        pBuf += 2;
    }
}

void decryptCBC(uint32_t *data, size_t len, uint32_t *key, uint32_t *iv)
{
    uint32_t *pBuf = data;
    uint32_t *pBufEnd = data + len;

    uint32_t c0 = pBuf[0];
    uint32_t c1 = pBuf[1];
    decryptBlock(pBuf, key);
    pBuf[0] ^= iv[0];
    pBuf[1] ^= iv[1];
    pBuf += 2;

    uint32_t t0, t1;
    while (pBuf < pBufEnd)
    {
        t0 = pBuf[0];
        t1 = pBuf[1];
        decryptBlock(pBuf, key);
        pBuf[0] ^= c0;
        pBuf[1] ^= c1;
        c0 = t0;
        c1 = t1;
        pBuf += 2;
    }
}

void encryptECB(uint32_t *data, size_t len, uint32_t *key)
{
    uint32_t *pBuf = data;
    uint32_t *pBufEnd = data + len;
    while (pBuf < pBufEnd)
    {
        // encrypt one 64-bit block at a time
        encryptBlock(pBuf, key);
        pBuf += 2;
    }
}

void decryptECB(uint32_t *data, size_t len, uint32_t *key)
{
    uint32_t *pBuf = data;
    uint32_t *pBufEnd = data + len;
    while (pBuf < pBufEnd)
    {
        // decrypt one 64-bit block at a time
        decryptBlock(pBuf, key);
        pBuf += 2;
    }
}

// Reference TEA from http://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
void encryptBlock(uint32_t *v, uint32_t *k) {
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
 
void decryptBlock (uint32_t* v, uint32_t* k) {
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
