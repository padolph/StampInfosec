//
// Stego --- read stego info from a BMP image file
//

#include "stego.h"

int main(int argc, const char *argv[])
{
    FILE *in,
         *outdata;
    
    int i,
        j,
        x,
        ttt,
        shft,
        dataBytes,
        dataBytesWritten;
        
    char temp,
         data;

    char infname[80],
         outdatafname[80];

    if(argc != 3)
    {
oops:   fprintf(stderr, "\nUsage: %s stegoImage outData\n\n", argv[0]);
        fprintf(stderr, "where stegoImage == filename for image containing stego data\n");
        fprintf(stderr, "      outData == data read from stegoImage file\n\n");
        exit(0);
    }
    
    sprintf(infname, argv[1]);
    sprintf(outdatafname, argv[2]);

    in = fopen(infname, "r");
    if(in == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", infname);
        exit(0);
    }
       
    outdata = fopen(outdatafname, "w");
    if(outdata == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", outdatafname);
        exit(0);
    }

    //
    // skip first START_FROM bytes of image file
    //
    for(i = 0; i < START_FROM; ++i)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }
    }

    //
    // read 64 bits of the file
    // if not of the form 0xa5, then
    // file does not contains stego data
    //
    for(i = 0; i < 8; ++i)
    {
        ttt = 0x0;
        for(j = 0; j < 8; ++j)
        {
            x = fscanf(in, "%c", &temp);
            if(x != 1)
            {
                fprintf(stderr, "\nError in file %s\n\n", infname);
                exit(0);
            }
            ttt ^= ((temp & 0x1) << j);
        }
        if(ttt != 0xa5)
        {
            fprintf(stderr, "\nError --- file does not contain stego data that I can read\n\n");
            exit(0);
        }
    }

    dataBytes = 0;
    for(i = 0; i < 27; ++i)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }
        dataBytes ^= ((temp & 0x1) << i);
    }

    printf("dataBytes = %d\n", dataBytes);
    
    data = 0;
    shft = 0;
    dataBytesWritten = 0;
    for(i = 0; i < (dataBytes << 3); ++i)
    {
        x = fscanf(in, "%c", &temp);

        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }

//        printf("bit %d = %d\n", shft, temp & 0x1);
            
        data = data ^ ((temp & 0x1) << shft);            
        ++shft;
        if(shft == 8)
        {
//            printf("data = %c\n", data);
                
            fprintf(outdata, "%c", data);
            ++dataBytesWritten;
            data = 0;
            shft = 0;
                    
        }// end if
        
    }// next i
    
    printf("\ndata bytes written = %d\n", dataBytesWritten);
    
    printf("\n");

    fclose(in);
    fclose(outdata);

}// end main
