/*

GFX Lightweight OpenGLES 2.0 Game and Graphics Engine

Copyright (C) 2011 Romain Marucchi-Foino http://gfx.sio2interactive.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of
this software. Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that
you wrote the original software. If you use this software in a product, an acknowledgment
in the product would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be misrepresented
as being the original software.

3. This notice may not be removed or altered from any source distribution.

*/


#ifndef FONT_H
#define FONT_H

/*!
	\file font.h
	
	\brief Function prototypes and definitions to use with the FONT structure.
*/


//! Structure to create a TTF font texture in video memory.
typedef struct
{
	//! Internal name to use for the font.
	char			name[ MAX_CHAR ];
	
	//! The TTF font character data, contains UVs and offsets.
	stbtt_bakedchar *character_data;
	
	//! The font size used at creation time. \sa FONT_load
	float			font_size;
	
	//! The texture width used at creation time. It is recommended to use a value that is a power of 2. \sa FONT_load
	int				texture_width;
	
	//! The texture height used at creation time. It is recommended to use a value that is a power of 2.
	int				texture_height;
	
	//! The first character (in ASCII value) to use as the start character.
	int				first_character;
	
	//! How many ASCII should be generated from the first_character ASCII. 
	int				count_character;
	
	//! The shader program used to draw the font on screen. This shader program will be auto-generated. Feel free to override it with your own if you want to create other font effects.
	PROGRAM			*program;

	//! The internal font texture id maintained by OpenGLES.
	unsigned int	tid;

} FONT;


FONT *FONT_init( char *name );

FONT *FONT_free( FONT *font );

unsigned char FONT_load( FONT *font, char *filename, unsigned char relative_path, float font_size, unsigned int texture_width, unsigned int texture_height, int first_character, int count_character );

void FONT_print( FONT *font, float x, float y, char *text, vec4 *color );

float FONT_length( FONT *font, char *text );

#endif
