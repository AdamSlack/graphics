//--------------------------------------------------------------------------------------------
// File:		bmp.cpp
// Version:		V1.0
// Author:		Daniel Rhodes
// Date:		30/05/2006
// Description:	Loads a bmp texture
// Notes:		30/05/2006 - Create .bmp class
//				02/06/2006 - Altered to accept non multiple of 4 width bmp's by taking into
//							 account the padding automatically added to the bmp in that scenario
//				05/06/2006 - Problem with non multiple of 4 width textures over 2x2 size fixed
//				05/06/2006 - Problem with non multiple of 4 width textures where size not
//							 given in bmp header fixed
//--------------------------------------------------------------------------------------------

//-----------------------------------------------
// Header files
//-----------------------------------------------
// System header files
// --

// Windows header files
// --

// OpenGL header files
// --

// Custom header files
#include "bmp.h"
//-----------------------------------------------
// End: Header files
//-----------------------------------------------

//-----------------------------------------------------------------------------
// Name:	BMP
// Parms:	None
// Returns:	None
// Desc:	Default constuctor, sets all the default values
//-----------------------------------------------------------------------------
BMP::BMP()
{
	imageData = 0;
	imageWidth = 0;
	imageHeight = 0;
}

//-----------------------------------------------------------------------------
// Name:	BMP
// Parms:	None
// Returns:	None
// Desc:	Default destuctor, frees all the image data
//-----------------------------------------------------------------------------
BMP::~BMP()
{
	FreeImage();
}

//-----------------------------------------------------------------------------
// Name:	LoadBMP
// Parms:	file = name of file to read
// Returns:	true for success, false for failure
// Desc:	Loads .bmp image to memory in a format useful to us
//-----------------------------------------------------------------------------
bool BMP::LoadBitmap( char *file )
{
	FILE *pFile = 0;
	
	BITMAPINFOHEADER bitmapInfoHeader;	// Bitmap header info
	BITMAPFILEHEADER header;			// Bitmap file info

	// Used when we need to swap the image colors from BGR to RGB	
	int iPadding	= 0;
	int iRowCount	= 0;

	// Used to find whether or not the image width is a multiple of 4 and hence whether or not padding has been used
	int mod4;

	if( !file )						// Check for filename
		return false;				// If no filename given get out
	pFile = fopen( file, "rb" );	// Open file
	if( !pFile )					// Check file has been opened
		return false;				// If file not open get out

	// Read in the bitmap file header info into the BITMAPFILEHEADER variable
	fread( &header, sizeof(BITMAPFILEHEADER), 1, pFile );

	// Make sure this is a real bitmap by checking the ID
	if( header.bfType != BITMAP_ID )
	{// If not a bmp clean up and return error
		fclose( pFile );
		return false;		
	}

	// Read in the bitmap header info into the BITMAPINFOHEADER variable
	fread( &bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile );

	// Save the image width and height
	imageWidth	= bitmapInfoHeader.biWidth;
	imageHeight	= bitmapInfoHeader.biHeight;

	mod4 = ( ( imageWidth * 3 ) % 4 );
	
	// if ( witdh * 3 ) not a multiple of 4 then need to take padding into account
	if ( mod4 != 0 )
	{
		iPadding = ( ( int( imageWidth * 3 + 4 ) / 4 )* 4 - ( imageWidth * 3 ) );
		// Get image size in case where we need space for end of row padding
		if( bitmapInfoHeader.biSizeImage == 0 )
			bitmapInfoHeader.biSizeImage = ( imageWidth * imageHeight * 3 ) + ( iPadding * imageHeight );
	}
	else	
	{
		// Get image size in case where no padding is used
		if ( bitmapInfoHeader.biSizeImage == 0 ) 
			bitmapInfoHeader.biSizeImage = imageWidth * imageHeight * 3;
	}

	// Find image data
	fseek( pFile, header.bfOffBits, SEEK_SET );

	// Assign memory for image
	unsigned char *tempImageData = new unsigned char[bitmapInfoHeader.biSizeImage];

	// Make sure the memory was allocated
	if( !tempImageData )
	{// If memory not allocated correctly then clean up and return error
		delete[] tempImageData;
		tempImageData = NULL;
		fclose( pFile );
		return false;
	}

	ZeroMemory( tempImageData, sizeof( unsigned char ) * bitmapInfoHeader.biSizeImage );
	
	// Read in the image data
	fread( tempImageData, 1, bitmapInfoHeader.biSizeImage, pFile );

	// Assign memory for image
	imageData = new unsigned char[bitmapInfoHeader.biSizeImage];
	// Make sure the memory was allocated
	if( !imageData )
	{// If memory not allocated correctly then clean up and return error
		delete[] imageData;
		imageData = NULL;
		fclose( pFile );
		return false;
	}
	
	ZeroMemory( imageData, sizeof( unsigned char ) * bitmapInfoHeader.biSizeImage );
	
	if ( imageWidth == 1 && imageHeight == 1 )
	{
		imageData[0]	= tempImageData[2];		// Red
		imageData[1]	= tempImageData[1];		// Green
		imageData[2]	= tempImageData[0];		// Blue
	}
	else
	{
		// Convert image data from BGR to RGB
		for( int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index += 3 )
		{
			imageData[index]		= tempImageData[index + 2];		// Red
			imageData[index + 1]	= tempImageData[index + 1];		// Green
			imageData[index + 2]	= tempImageData[index];			// Blue
			iRowCount++;
			if ( iRowCount == imageWidth )
			{
				index += iPadding;
				iRowCount = 0;
			}
		}
	}

	if( tempImageData )
	{
		delete[] tempImageData;
		tempImageData = NULL;
	}

	fclose( pFile );	// Close the file
	return true;	// and return success
}

//-----------------------------------------------------------------------------
// Name:	FreeImage
// Parms:	None
// Returns:	None
// Desc:	Make sure memory is freed if used
//-----------------------------------------------------------------------------
void BMP::FreeImage( )
{
	// Delete all dynamically allocated memory
	if( imageData )
	{
		delete[] imageData;
		imageData = NULL;
	}
}