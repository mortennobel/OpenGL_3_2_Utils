#include "TextureLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

unsigned char * loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight, bool flipY){
	printf("Reading image %s\n", imagepath);
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("Image could not be opened\n"); return NULL;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 byes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return NULL;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return NULL;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return NULL;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	outWidth      = *(int*)&(header[0x12]);
	outHeight     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=outWidth*outHeight*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	if (flipY){
		// swap y-axis
		unsigned char * tmpBuffer = new unsigned char[outWidth*3];
		int size = outWidth*3;
		for (int i=0;i<outHeight/2;i++){
			// copy row i to tmp
			memcpy_s(tmpBuffer,size,data+outWidth*3*i,size);
			// copy row h-i-1 to i
			memcpy_s(data+outWidth*3*i, size, data+outWidth*3*(outHeight-i-1), size);
			// copy tmp to row h-i-1
			memcpy_s(data+outWidth*3*(outHeight-i-1), size,tmpBuffer, size);
		}
		delete [] tmpBuffer;
	}

	return data;
}

