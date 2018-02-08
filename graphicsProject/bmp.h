//--------------------------------------------------------------------------------------------
// File:		bmp.h
// Version:		V1.0
// Author:		Daniel Rhodes
// Date:		30/05/2006
// Description:	Loads a bmp texture
// Notes:		30/05/2006 - Create .bmp class
//--------------------------------------------------------------------------------------------
// Only include this file once, may cause problems with some compilers.
#pragma once
// Same effect as:
// #ifndef BMP_H
// #define BMP_H
//
// main body of code
//
// #endif // !defined(BMP_H)

#define BITMAP_ID 0x4D42	// The universal bitmap ID

//-----------------------------------------------
// Header files
//-----------------------------------------------
// System header files
#include <stdio.h>          // Standard (C-style) input / output

// Windows header files
#include <windows.h>        // Windows header file

// OpenGL header files
// --

// Custom header files
// --
//-----------------------------------------------
// End: Header files
//-----------------------------------------------

class BMP
{
public:
	BMP();
	~BMP();

	bool LoadBitmap( char *filename );	// Load a .bmp image file
	void FreeImage();					// Delete the image data

	unsigned int ID;					// Texture ID
	int imageWidth;						// Width of image
	int imageHeight;					// Height of image
	unsigned char *imageData;			// Image data
};