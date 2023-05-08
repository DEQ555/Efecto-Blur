#include "Render2023-v2.h"
#include "BMP.H"

static unsigned int *spr=NULL;
static int w=0,h=0,b=0;

void AplicBlur( const int w, const int h ){
	unsigned int *P = (unsigned int*)malloc( sizeof(unsigned int) * b );
	memcpy( P, spr, (sizeof(unsigned int)*b) );
	unsigned int *_v0 = P, *_v1, count, row, col, r, c;
	unsigned char *channel;
	float totalR = 0, totalG = 0, totalB = 0;
	for ( row = 0; row < h; ++row){
		for ( col = 0; col < w; ++col){
			count = 0;
			const unsigned int xaxis[] = {row - 1, row, row + 1}, yaxis[] = {col - 1, col, col + 1};
            totalR = 0, totalG = 0, totalB = 0;
            for ( r = 0; r < 3; ++r){
            	for ( c = 0; c < 3; ++c){
					const int curRow = xaxis[r];
                    const int curCol = yaxis[c];
            		if ( (unsigned int)curRow < h && (unsigned int)curCol < w ){
            			channel = (unsigned char*)&spr[ curRow * w + curCol ];
            			totalB += *channel++;
            			totalG += *channel++;
            			totalR += *channel++;
            			count++;
            		}
            	}
            }
			totalR /= count, totalG /= count, totalB /= count;
			*_v0++ = COLOR_RGB(totalR,totalG,totalB);
		}
	}
	free(spr),spr=NULL;
	spr = P;
}

void BUCLE( unsigned int *vbuff, int fps, int deltatime ){
	ClearScreen(0xffffffff);
	if(GetKey(27))SetLoopNull();
	for (unsigned int Y = 0; Y < h; ++Y){
		for (unsigned int X = 0; X < w; ++X){
			vbuff[ Y * (320<<1) + X ] = spr[ Y * w + X ];
		}
	}
}


int main(int argc, char const *argv[]){
	if( argc > 1 ){
		spr = (unsigned int*)LoadBMPConverter(argv[1],&w,&h,&b);
	}
	else {
		spr = (unsigned int*)LoadBMPConverter("in.bmp",&w,&h,&b);
	}
	b = w*h;
	AplicBlur(w,h);
	CrearScreen( 320<<1, 240<<1 );
	SetLoop(BUCLE);
	StartLoop();
	SaveBMPConverter("out.bmp",spr,w,h,32);
	free(spr),spr=NULL;
	return 0;
}