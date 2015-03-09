//
// Stego --- insert info in a BMP image file
//

#include "stego.h"

int main(int argc, const char *argv[])
{
    FILE *in,
         *indata,
         *out;
    
    int i,
        j,
        x,
        ttt,
        shft,
        byteCount,
        moreData,
        moreImage,
        imageBytes,
        dataBytes,
        dataBytesWritten,
        imageBytesWritten;

    char temp,
         data;

    char infname[80],
         indatafname[80],
         outfname[80];

    if(argc != 4)
    {
oops:   fprintf(stderr, "\nUsage: %s plainImage stegoImage stegoData\n\n", argv[0]);
        fprintf(stderr, "where plainImage == filename containing initial image\n");
        fprintf(stderr, "      stegoImage == filename for image containing stego data\n");
        fprintf(stderr, "      stegoData == data to insert into stegoImage file\n\n");
        exit(0);
    }
    
    sprintf(infname, argv[1]);
    sprintf(outfname, argv[2]);
    sprintf(indatafname, argv[3]);

    in = fopen(infname, "r");
    if(in == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", infname);
        exit(0);
    }

    imageBytes = 0;
    while(1)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            break;
        }
        ++imageBytes;
    }
    fclose(in);
    
    printf("image bytes = %d, capacity = %d bytes\n", imageBytes, imageBytes >> 3);
    
    in = fopen(infname, "r");
    if(in == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", infname);
        exit(0);
    }

    indata = fopen(indatafname, "r");
    if(indata == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", indatafname);
        exit(0);
    }
    
    dataBytes = 0;
    while(1)
    {
        x = fscanf(indata, "%c", &data);
        if(x != 1)
        {
            break;
        }
        ++dataBytes;
    }
    fclose(indata);
    
    printf("dataBytes = %d\n", dataBytes);
    
    indata = fopen(indatafname, "r");
    if(indata == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", indatafname);
        exit(0);
    }

    if((imageBytes - START_FROM - 27 - 64) <= 0)
    {
        fprintf(stderr, "\nImage file %s too small\n\n", infname);
        exit(0);

    }// end if
    
    if((dataBytes << 3) >= (imageBytes - START_FROM - 27 - 64))
    {
        fprintf(stderr, "\nData file %s too large for this image file %s\n\n",
            indatafname, infname);
        exit(0);

    }// end if

    if(dataBytes > (1 << 27))
    {
        fprintf(stderr, "\nData file %s too large (max size is %d)\n\n",
            indatafname, (1 << 27));
        exit(0);

    }// end if

    out = fopen(outfname, "w");
    if(out == NULL)
    {
        fprintf(stderr, "\nError opening file %s\n\n", outfname);
        exit(0);
    }

    //
    // skip first START_FROM bytes of image file
    //
    imageBytesWritten = 0;
    for(i = 0; i < START_FROM; ++i)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }
        fprintf(out, "%c", temp);
        ++imageBytesWritten;
    }

    //
    // insert 64 bits of the form 0xa5
    // to indicate that the file contains stego data
    //
    ttt = 0xa5;
    for(i = 0; i < 8; ++i)
    {
        for(j = 0; j < 8; ++j)
        {
            x = fscanf(in, "%c", &temp);
            if(x != 1)
            {
                fprintf(stderr, "\nError in file %s\n\n", infname);
                exit(0);
            }
            temp = (temp & 0xfe) ^ ((ttt >> j) & 0x1);
            fprintf(out, "%c", temp);
            ++imageBytesWritten;
        }
    }
    
    //
    // insert bits of the number dataBytes (27 bits)
    //
    for(i = 0; i < 27; ++i)
    {
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            fprintf(stderr, "\nError in file %s\n\n", infname);
            exit(0);
        }
        temp = (temp & 0xfe) ^ ((dataBytes >> i) & 0x1);
        fprintf(out, "%c", temp);
        ++imageBytesWritten;
    }

    //
    // read first byte of data
    //
    x = fscanf(indata, "%c", &data);
    if(x != 1)
    {
        fprintf(stderr, "\nFile %s is empty\n\n", indatafname);
        exit(0);
    }
    
    moreImage = 1;
    shft = 0;
    dataBytesWritten = 0;
    
    while(moreImage)
    {
//        printf("byteCount = %d\n", byteCount);
        
        x = fscanf(in, "%c", &temp);
        if(x != 1)
        {
            moreImage = 0;
        }
        else
        {
            if(dataBytesWritten < dataBytes)
            {
                temp = (temp & 0xfe) ^ ((data >> shft) & 0x1);
                shft = (shft + 1) & 0x7;
            
                if(shft == 0)
                {
                    ++dataBytesWritten;

                    x = fscanf(indata, "%c", &data);
/***
                    printf("data = %c = ", data);
                    for(i = 0; i < 8; ++i)
                    {
                        printf("%1d", (data >> i) & 0x1);
                    }
                    printf("\n");
***/
                    if(x != 1)
                    {
                        moreData = 0;
                    }
                }

            }// end if

            fprintf(out, "%c", temp);
            ++imageBytesWritten;

        }// end if

    }// end while
    
    printf("\nimage bytes written = %d, data bytes written = %d\n", 
        imageBytesWritten, dataBytesWritten);

    if(moreData)
    {
        printf("Entire data not inserted into image\n");
    }
    printf("\n");

    fclose(in);
    fclose(out);
    fclose(indata);

}// end main
