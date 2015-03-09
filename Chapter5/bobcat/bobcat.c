//
// Bobcat hash
//
// Bobcat is a 48-bit hash modeled on Tiger
//
// Bobcat is based on 16-bit words
// The hash value is (a,b,c), where each is a 16-bit word
//
// Message is divided into 128 bit blocks, that is,
// each block consists of eight 16-bit words.
// If necessary, pad message with 0's to fill out
// final block.
//
// Bobcat then has the same structure as Tiger:
//     outer round composed of 3 functions, F_5, F_7 and F_9
//     each inner round consists of 8 functions f_m,i
//     (for a total of 24 rounds for each block)
//     a key schedule algorithm is used
//     there are 2 S-boxes, each mapping 4 bits to 16 bits 
//
// To compile: gcc bobcat.c -o bobcat
//
// Test vectors (all padded to full 128-bit blocks) with hash values:
//
//
// 2 blocks (16 words)
//message = 0x5920,0xc7cc,0x6234,0xb111,0x3090,0x8ab7,0xf373,0x46dd,
//          0x35d3,0xc06e,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
// Bobcat hash = 0x2bce98fa6186
//
//
// 2 blocks (16 words)
//message = 0xbe43,0x83c6,0x017a,0x3e71,0x44a5,0x0a9f,0x3349,0x2c8a,
//          0x0f84,0x6ce7,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
// Bobcat hash = 0x75ebe721c8cd
//
//
// 4 blocks (32 words)
//message = 0xbf37,0xc638,0x17a0,0x1f71,0xe4a5,0x6a9e,0x3449,0x2caa,
//          0x0f84,0x6ce7,0x3578,0x60fd,0xa780,0x81a8,0x67c0,0x789f,
//          0xbe34,0x3c66,0x417a,0x3e71,0x44a5,0x0a9f,0x3349,0x2c8a,
//          0x0f43,0x0ce7,0x4321,0x1234,0xfe23,0x0b0a,0x0000,0x0000
// Bobcat hash = 0xf20aec83c477
//
//
// 4 blocks (32 words)
//message = 0xa401,0xeb2c,0x988d,0xe8a4,0xbf27,0x189b,0xb231,0x2faa,
//          0x23bc,0xfd34,0x2033,0xb107,0x797f,0xe9d0,0x3d16,0xf236,
//          0xb871,0x85ff,0xd95b,0x8721,0x74f2,0xd547,0x4f91,0x6f0c,
//          0x3e68,0x549b,0xfbd3,0x9e3d,0xc869,0x593f,0x75db,0x6c6a
// Bobcat hash = 0x7e5abf5fad22
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// functions
//
void abcSave(int abc[], int old[]);
void f(int abc[], int zero, int one, int two, int x, int m);
void F(int abc[], int m, int x[]);
void keySchedule(int x[]);
void feedforward(int abc[], int old[]);
void bobcat(int abc[], int x[]);


//
// 2 S-boxes, each mapping 4 bits to 16 bits
//
int sbox[2][16]={{0xd131,0x0ba6,0x98df,0xb5ac,0x2ffd,0x72db,0xd01a,0xdfb7,
                  0xb8e1,0xafed,0x6a26,0x7e96,0xba7c,0x9045,0xf12c,0x7f99}, 
                 {0x24a1,0x9947,0xb391,0x6cf7,0x0801,0xf2e2,0x858e,0xfc16,
                  0x6369,0x20d8,0x7157,0x4e69,0xa458,0xfea3,0xf493,0x3d7e}};

//
// save the abc values at start of F_m
// (needed for feedforward)
//
void abcSave(int abc[], int old[])
{
    old[0] = abc[0];
    old[1] = abc[1];
    old[2] = abc[2];

//printf("old = 0x%04x%04x%04x\n\n", old[0], old[1], old[2]);

}// end abcSave


//
// inner round functions f_{m,i}
//
void f(int abc[], int zero, int one, int two, int x, int m)
{
    int aa, bb, cc;
    
    aa = abc[zero];
    bb = abc[one];
    cc = abc[two];
    
    cc = (cc ^ x) & 0xffff;
    aa = (aa - (sbox[0][cc & 0xf] ^ sbox[1][(cc >> 8) & 0xf])) & 0xffff;
    bb = (bb + (sbox[1][(cc >> 4) & 0xf] ^ sbox[0][(cc >> 12) & 0xf])) & 0xffff;
    bb = (bb * m) & 0xffff;

    abc[zero] = aa;
    abc[one] = bb;
    abc[two] = cc;
    
}// end f


//
// outer round functions F_m
//
void F(int abc[], int m, int x[])
{
    f(abc,0,1,2,x[0],m);// a,b,c
    f(abc,1,2,0,x[1],m);// b,c,a
    f(abc,2,0,1,x[2],m);// c,a,b
    f(abc,0,1,2,x[3],m);// a,b,c
    f(abc,1,2,0,x[4],m);// b,c,a
    f(abc,2,0,1,x[5],m);// c,a,b
    f(abc,0,1,2,x[6],m);// a,b,c
    f(abc,1,2,0,x[7],m);// b,c,a
    
}// end F


//
// "key" schedule
//
void keySchedule(int x[])
{
      x[0] = (x[0] - (x[7] ^ 0xa5a5a5a5)) & 0xffff;
      x[1] = (x[1] ^ x[0]) & 0xffff;
      x[2] = (x[2] + x[1]) & 0xffff;
      x[3] = (x[3] - (x[2] ^ ((~x[1]) << 5))) & 0xffff;
      x[4] = (x[4] ^ x[3]) & 0xffff;
      x[5] = (x[5] + x[4]) & 0xffff;
      x[6] = (x[6] - (x[5] ^ ((~x[4]) >> 6))) & 0xffff;
      x[7] = (x[7] ^ x[6]) & 0xffff;
      x[0] = (x[0] + x[7]) & 0xffff;
      x[1] = (x[1] - (x[0] ^ ((~x[7]) << 5))) & 0xffff;
      x[2] = (x[2] ^ x[1]) & 0xffff;
      x[3] = (x[3] + x[2]) & 0xffff;
      x[4] = (x[4] - (x[3] ^ ((~x[2]) >> 6))) & 0xffff;
      x[5] = (x[5] ^ x[4]) & 0xffff;
      x[6] = (x[6] + x[5]) & 0xffff;
      x[7] = (x[7] - (x[6] ^ 0x235689bd)) & 0xffff;

}// end keySchedule


//
// add in the old values to produce new a,b,c
//
void feedforward(int abc[], int old[])
{
    abc[0] = (abc[0] ^ old[0]) & 0xffff;
    abc[1] = (abc[1] - old[1]) & 0xffff;
    abc[2] = (abc[2] + old[2]) & 0xffff;

}// end feedforward


//
// compression function (one outer round)
//
void bobcat(int abc[], int x[])
{
    int old[3];

//printf("abc = 0x%04x%04x%04x\n\n", abc[0], abc[1], abc[2]);

    abcSave(abc, old);
    F(abc, 5, x);
    keySchedule(x);
    F(abc, 7, x);
    keySchedule(x);
    F(abc, 9, x);
    feedforward(abc, old);

}// end bobcat


//
// main
//
int main(int argc, const char *argv[])
{
	// len == number of 128-bit message blocks (128 = 8*16)
	// x[i] contains the message, in 16-bit words
	// size of x[] array must be a multiple of 8 
	// if message not a multiple of 8, pad with 0's
    int i,
        j,
		len;
        
    int abc[3];

	//********************************************************************
	//********************************************************************
	//********************************************************************
	// 2 blocks (16 words)
	// Bobcat hash = 0x2bce98fa6186
	len = 2;// number of 128-bit blocks
	int x[32] = {0x5920,0xc7cc,0x6234,0xb111,0x3090,0x8ab7,0xf373,0x46dd,
				 0x35d3,0xc06e,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
	//********************************************************************
	//********************************************************************
	//********************************************************************
	
    // initial constants a,b,c
    abc[0] = 0xface;
    abc[1] = 0xe961;
    abc[2] = 0x041d;

    for(i = 0; i < len; ++i)
    {
		// bobcat() does one outer round
        bobcat(abc, &x[8*i]);
    }
    printf("\nBobcat hash = 0x%04x%04x%04x\n\n", abc[0], abc[1], abc[2]);
    
}// end main
