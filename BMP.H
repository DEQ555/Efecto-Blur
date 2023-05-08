#ifndef ___BMP_H___
#define ___BMP_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* __fastcall LoadBMP( const char* filename, unsigned int *ancho, unsigned int *alto, unsigned char *bpp )
{
    int Y;
    unsigned int X;
    unsigned int STEP;
    unsigned char header[54];
    unsigned char BYTES_BAZURAS[3];
    unsigned char *data = NULL;
    FILE *f = fopen( filename, "rb" );
    fread( header, sizeof(unsigned char), 54, f );
    const unsigned int ANCHO = *(int*)&header[18];
    const unsigned int ALTO  = *(int*)&header[22];
    const unsigned char BPP  = *(int*)&header[28];
    const unsigned int size  = ( ANCHO * ALTO * ((BPP)/8) );
    const unsigned char padding = ( ( 4 - ( ANCHO * (BPP/8) ) % 4) % 4 );
    data = (unsigned char*)malloc( size * (BPP/8) );
    fseek( f, 54, SEEK_SET );
    for ( Y = ALTO-1; Y >=0; --Y){
        for ( X = 0; X < ANCHO; ++X){
            STEP = ( ( Y * ANCHO + X ) * (BPP/8) );
            fread( &data[ STEP ], (BPP/8), 1, f );
        }
        fread( BYTES_BAZURAS, padding, 1, f );
    }
    fclose(f),f=NULL;
    *ancho = ANCHO;
    *alto = ALTO;
    *bpp = BPP;
    return data;
}

void* __fastcall LoadBMPConverter( const char *filename, int *w, int *h, int *bpp )
{
    unsigned int *data=NULL;
    unsigned int x,y,i;
    unsigned char n;
    unsigned char *temp = LoadBMP( filename, &x, &y, &n );
    *w=x,*h=y,*bpp=(n/8);
    data=(unsigned int*)malloc( x*y*sizeof(unsigned int));
    unsigned int *pixels=data;
    if(*bpp==4){
        memcpy(pixels,temp,(x*y*sizeof(unsigned int)));
        *bpp=4;
        free( temp );
        return data;
    }
    for (i=0;i<(x*y*(n/8));i+=(n/8))
        *(pixels++)=(temp[i]<<(0))|(temp[i+1]<<(8))|(temp[i+2]<<16)|(255<<24);
    *bpp=4;
    free( temp );
    return data;
}

void __fastcall SaveBMP(
    const char* filename,
    unsigned char *data,
    const unsigned int ancho,
    const unsigned int alto,
    unsigned char bpp
    )
{
    int Y;
    unsigned int X;
    unsigned int STEP;
    unsigned char BPP;
    unsigned char BYTES_BAZURAS[3];
    if ( bpp<24 )bpp*=8;
    if ( !data ) return;
    const unsigned char padding = ( ( 4 - ( ancho * (bpp/8) ) % 4) % 4 );
    unsigned char header[14], informacionheader[ 40 ];
    memset( header, 0, ( sizeof(unsigned char) * 14 ) );
    memset( informacionheader, 0, ( sizeof(unsigned char) * 40 ) );
    const unsigned int filesize = 14 + 40 + ancho * alto * (bpp/8) + padding * ancho;
    header[0] = 'B',header[1] = 'M';
    header[2] = filesize, header[3] = filesize >> 8, header[4] = filesize >> 16, header[5] = filesize >> 24;
    header[10] = 14 + 40;
    informacionheader[0] = 40;
    informacionheader[4] = ancho, informacionheader[5] = ancho >> 8, informacionheader[6] = ancho >> 16, informacionheader[7]  = ancho >> 24;
    informacionheader[8] = alto,  informacionheader[9] = alto >> 8, informacionheader[10] = alto >> 16,  informacionheader[11] = alto >> 24;
    informacionheader[12] = 1, informacionheader[14] = bpp;
    FILE *f = fopen( filename, "wb" );
    fwrite( header, 14, 1, f );
    fwrite( informacionheader, 40, 1, f );
    BPP = (bpp/8);
    for ( Y = alto-1; Y >=0 ; --Y){
        for ( X = 0; X < ancho; ++X) STEP = ( Y * ancho + X ) * BPP, fwrite( &data[ STEP ], BPP, 1, f );
        fwrite( BYTES_BAZURAS, padding, 1, f );
    }
    fclose(f),f=NULL;
    return;
}

void __fastcall SaveBMPConverter( const char *filename, void *data, const unsigned int w, const unsigned int h, const unsigned int bpp )
{
    SaveBMP( filename, (unsigned char*)data, w, h, bpp );
}

#endif