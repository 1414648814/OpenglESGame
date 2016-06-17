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

#ifndef TEXTURE_H
#define TEXTURE_H

/*!
	\file texture.h
	
    \brief Contains structure definition and functions to use with a TEXTURE structure.
*/


enum
{
	//! Enable image clamping on the UV, 0 to 1 only no texture repeat.
	TEXTURE_CLAMP = ( 1 << 0 ),

	//! Enable automatic mipmap generation.
	TEXTURE_MIPMAP = ( 1 << 1 ),
	
	//! Enable automatic 16 bits conversion.
	TEXTURE_16_BITS = ( 1 << 2 ),

	//! Force the conversion of 32 bits textures to use use 5551 instead of 4444.
	TEXTURE_16_BITS_5551 = ( 1 << 3 )
};


enum
{
	//! Image filtering nearest.
	TEXTURE_FILTER_0X = 0,
	
	//! Image filtering linear.
	TEXTURE_FILTER_1X = 1,
	
	//! Bilinear image filtering.
	TEXTURE_FILTER_2X = 2,

	//! Trilinear image filtering.
	TEXTURE_FILTER_3X = 3
};


//! PVRTC file header data.
typedef struct
{
	//! The size of the PVRTC header.
	unsigned int headersize;
	
	//! The height of the original texture size.
	unsigned int height;

	//! The width of the original texture size.
	unsigned int width;
	
	//! The number of number of mipmap contained in the PVR image stream.
	unsigned int n_mipmap;
	
	//! The flags of the PVR image.
	unsigned int flags;
	
	//! Data block size.
	unsigned int datasize;
	
	//! The number of bits per pixel.
	unsigned int bpp;
	
	//! The size of the red bit.
	unsigned int bitred;

	//! The size of the green bit.
	unsigned int bitgreen;

	//! The size of the blue bit.
	unsigned int bitblue;
	
	//! The size of the alpha bit.	
	unsigned int bitalpha;
	
	//! Unknown.
	unsigned int tag;
	
	//! The number of surface held in the PVR file.
	unsigned int n_surface;

} PVRHEADER;


//! The TEXTURE structure used to control a texture behaviors and properties.
typedef struct
{
	//! Internal name to use for the TEXTURE.
	char			name[ MAX_CHAR ];
	
	//! The OpenGLES texture id.
	unsigned int	tid;
	
	//! The texture width.
	unsigned short	width;
	
	//! The texture height.
	unsigned short	height;
	
	//! The texture bits.
	unsigned char	byte;
	
	//! The total size of the texture in bytes.
	unsigned int	size;

	//! The target of the texture. (Default GL_TEXTURE_2D)
	unsigned int	target;
	
	//! The internal format of the texture.
	unsigned int	internal_format;
	
	//! The format of the texture.
	unsigned int	format;

	//! The type of texel to use for this texture. (Default GL_UNSIGNED_BYTE)
	unsigned int	texel_type;

	//! The raw texel array.
	unsigned char	*texel_array;

	//! The number of mipmaps (PVRTC only).
	unsigned int	n_mipmap;
	
	//! The compression type.
	unsigned int	compression;
		
} TEXTURE;


TEXTURE *TEXTURE_init( char *name );

TEXTURE *TEXTURE_free( TEXTURE *texture );

TEXTURE *TEXTURE_create( char *name, char *filename, unsigned char relative_path, unsigned int flags, unsigned char filter, float anisotropic_filter );

void TEXTURE_load( TEXTURE *texture, MEMORY *memory );

void TEXTURE_load_png( TEXTURE *texture, MEMORY *memory );

void TEXTURE_load_pvr( TEXTURE *texture, MEMORY *memory );

void TEXTURE_convert_16_bits( TEXTURE *texture, unsigned char use_5551 );

void TEXTURE_generate_id( TEXTURE *texture, unsigned int flags, unsigned char filter, float anisotropic_filter );

void TEXTURE_delete_id( TEXTURE *texture );

void TEXTURE_free_texel_array( TEXTURE *texture );

void TEXTURE_draw( TEXTURE *texture );

void TEXTURE_scale( TEXTURE *texture, unsigned int width, unsigned int height );

#endif
