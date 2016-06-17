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

#include "gfx.h"

/*!
	\file texture.cpp
	
    \brief Create and manipulate textures through the TEXTURE interface.
		
	\details The TEXTURE structure provide an interface to handle OpenGLES texture.
	The structure support in-memory load of PNG and PVRTC and allow you to 
	convert 24 and 32 bits textures to 16 bits.
*/


/*!
	Create a new TEXTURE structure pointer.
	
	\param[in] name The internal name to use for the new TEXTURE.
	
	\return Return a new TEXTURE structure pointer.
*/
TEXTURE *TEXTURE_init( char *name )
{
	TEXTURE *texture = ( TEXTURE * ) calloc( 1, sizeof( TEXTURE ) );

	texture->target = GL_TEXTURE_2D;

	strcpy( texture->name, name );
	
	return texture;
}


/*!
	Free a previously initialized TEXTURE structure.

	\param[in,out] texture A valid TEXTURE structure pointer.
	
	\return Return a NULL TEXTURE structure pointer.
*/
TEXTURE *TEXTURE_free( TEXTURE *texture )
{
	TEXTURE_free_texel_array( texture );
	
	TEXTURE_delete_id( texture );
	
	free( texture );
	return NULL;
}


/*!
	Helper function to automatically create, load and generate a new TEXTURE.

	\param[in] name The internal name to use for the new TEXTURE.
	\param[in] filename The file to load.
	\param[in] relative_path Determine if the filename is a relative (1) or absolute (0) path.
	\param[in] flags The TEXTURE flags to use.
	\param[in] filter The mipmap filtering to use.
	\param[in] anisotropic_filter The anisotropic filtering to use for the texture.
	
	\return Return a new TEXTURE structure pointer, or return NULL if the function was unable
	to load the file.
*/
TEXTURE *TEXTURE_create( char *name, char *filename, unsigned char relative_path, unsigned int flags, unsigned char filter, float anisotropic_filter )
{
	TEXTURE *texture = TEXTURE_init( name );
	
	MEMORY *m = mopen( filename, relative_path );
	
	if( m )
	{
		TEXTURE_load( texture, m );
		
		TEXTURE_generate_id( texture, flags, filter, anisotropic_filter );
		
		TEXTURE_free_texel_array( texture );
		
		mclose( m );
	}
	
	return texture;
}


/*!
	Load and uncompress TEXTURE texels from a MEMORY stream.
	
	\param[in,out] texture A valid TEXTURE structure pointer.
	\param[in] memory The compressed texture stream (either PNG or PVR).
	
*/
void TEXTURE_load( TEXTURE *texture, MEMORY *memory )
{
	char ext[ MAX_CHAR ] = {""};
	
	get_file_name( memory->filename, texture->name );
	
	get_file_extension( memory->filename, ext, 1 );
	
	if( !strcmp( ext, "PNG" ) ) TEXTURE_load_png( texture, memory );
	
	else if( !strcmp( ext, "PVR" ) ) TEXTURE_load_pvr( texture, memory );
}


/*!
	Callback function used internally to decompress a PNG file in memory.
	
	\param[in] structp The PNG structure that point to the attached MEMORY stream.
	\param[in] bytep The byte pointer to of the stream to read.
	\param[in] size The size in bytes to read from the stream.
*/
void png_memory_read( png_structp structp, png_bytep bytep, png_size_t size )
{
	MEMORY *m = ( MEMORY * ) png_get_io_ptr( structp );

	mread( m, bytep, size );
}


/*!
	Helper function to decompress a PNG texture in-memory.
	
	\param[in,out] texture A valid TEXTURE pointer.
	\param[in] memory A valid MEMORY pointer that contains a PNG buffer.
*/
void TEXTURE_load_png( TEXTURE *texture, MEMORY *memory )
{
	png_structp structp;

	png_infop infop;

	png_bytep *bytep = NULL;

	unsigned int i = 0;
	
	int n,
		png_bit_depth,
		png_color_type;

	structp = png_create_read_struct( PNG_LIBPNG_VER_STRING,
									  NULL,
									  NULL,
									  NULL );

	infop = png_create_info_struct( structp );

	png_set_read_fn( structp, ( png_voidp * )memory, png_memory_read );

	png_read_info( structp, infop );

	png_bit_depth = png_get_bit_depth( structp, infop );
	
	png_color_type = png_get_color_type( structp, infop );

	if( png_color_type == PNG_COLOR_TYPE_PALETTE )
	{ png_set_expand( structp ); }

	if( png_color_type == PNG_COLOR_TYPE_GRAY && png_bit_depth < 8 )
	{ png_set_expand( structp ); }
	
	if( png_get_valid( structp, infop, PNG_INFO_tRNS ) )
	{ png_set_expand( structp ); }
	
	if( png_bit_depth == 16 )
	{ png_set_strip_16( structp ); }
	
	if( png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
	{ png_set_gray_to_rgb( structp ); }

	png_read_update_info( structp, infop );

	png_get_IHDR( structp,
				  infop,
				  ( png_uint_32 * )( &texture->width  ),
				  ( png_uint_32 * )( &texture->height ),
				  &png_bit_depth,
				  &png_color_type,
				  NULL, NULL, NULL );

	switch( png_color_type )
	{
		case PNG_COLOR_TYPE_GRAY:
		{
			texture->byte			 = 1;
			texture->internal_format = 
			texture->format		     = GL_LUMINANCE;
			
			break;
		}

		case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			texture->byte			 = 2;
			texture->internal_format = 
			texture->format		     = GL_LUMINANCE_ALPHA;
					
			break;
		}

		case PNG_COLOR_TYPE_RGB:
		{
			texture->byte			 = 3;
			texture->internal_format = 
			texture->format		     = GL_RGB;
			
			break;
		}

		case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			texture->byte			 = 4;
			texture->internal_format = 
			texture->format		     = GL_RGBA;
			
			break;
		}
	}

	texture->texel_type = GL_UNSIGNED_BYTE;
	
	texture->size = texture->width * texture->height * texture->byte;
	
	texture->texel_array = ( unsigned char * ) malloc( texture->size );

	bytep = ( png_bytep * ) malloc( texture->height * sizeof( png_bytep ) );

	
	while( i != texture->height )
	{
		n = texture->height - ( i + 1 );
		
		bytep[ n ] = texture->texel_array + ( n * texture->width * texture->byte );
		
		++i;
	}	

	
	png_read_image( structp, bytep );

	png_read_end( structp, NULL );

	png_destroy_read_struct( &structp,
							 &infop,
							 NULL );

	free( bytep );
}


/*!
	Helper function to decompress a PVR texture in-memory.
	
	\param[in,out] texture A valid TEXTURE pointer.
	\param[in] memory A valid MEMORY pointer that contains a PVR buffer.
*/
void TEXTURE_load_pvr( TEXTURE *texture, MEMORY *memory )
{
	const unsigned char pvrtc_identifier[ 4 ] = { 'P', 'V', 'R', '!' };

	PVRHEADER *pvrheader = ( PVRHEADER * )memory->buffer;

	if( ( ( pvrheader->tag >> 0  ) & 0xFF ) != pvrtc_identifier[ 0 ] ||
		( ( pvrheader->tag >> 8  ) & 0xFF ) != pvrtc_identifier[ 1 ] ||
		( ( pvrheader->tag >> 16 ) & 0xFF ) != pvrtc_identifier[ 2 ] ||
		( ( pvrheader->tag >> 24 ) & 0xFF ) != pvrtc_identifier[ 3 ] )
	{ return; }


	if( ( pvrheader->flags & 0xFF ) == 24 || // PVRTC2
		( pvrheader->flags & 0xFF ) == 25 )  // PVRTC4
	{
		texture->width    = pvrheader->width;
		texture->height   = pvrheader->height;
		texture->byte     = pvrheader->bpp;
		texture->n_mipmap = pvrheader->n_mipmap + 1;

		if( pvrheader->bitalpha )
		{
			texture->compression = pvrheader->bpp == 4 ?
								   GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
								   GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		}
		else
		{
			texture->compression = pvrheader->bpp == 4 ?
								   GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
								   GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
		}

		texture->texel_array = ( unsigned char * ) malloc( pvrheader->datasize );

		memcpy( texture->texel_array,
				&memory->buffer[ sizeof( PVRHEADER ) ],
				pvrheader->datasize );
	}
}


/*!
	Helper function to convert an uncompressed 24bits or 32bits image to a 16 bits image.
	
	\param[in,out] texture A valid TEXTURE pointer.
	\param[in] use_5551 Determine if a 32bits image should be converted using 5551 bits instead of the default 4444 bits.
*/
void TEXTURE_convert_16_bits( TEXTURE *texture, unsigned char use_5551 )
{
	unsigned int i = 0,
				 s = texture->width * texture->height,
				*t = NULL;

	unsigned short *texel_array = NULL;
	
	switch( texture->byte )
	{
		case 3:
		{
			unsigned int j = 0;
			
			unsigned char *tmp_array = NULL;
			
			texture->byte		= 4;
			texture->size		= s * texture->byte;
			texture->texel_type = GL_UNSIGNED_SHORT_5_6_5;

			tmp_array = ( unsigned char * ) malloc( texture->size );

			while( i != texture->size )
			{
				tmp_array[ i     ] = texture->texel_array[ j     ];
				tmp_array[ i + 1 ] = texture->texel_array[ j + 1 ];
				tmp_array[ i + 2 ] = texture->texel_array[ j + 2 ];
				tmp_array[ i + 3 ] = 255;
				
				i += texture->byte;
				j += 3;				
			}

			free( texture->texel_array );
			texture->texel_array = tmp_array;

			texel_array = ( unsigned short * )texture->texel_array;

			t = ( unsigned int * )texture->texel_array;
			
			i = 0;
			while( i != s )
			{
			   *texel_array++ = ( ( (   *t         & 0xff ) >> 3 ) << 11 ) | 
								( ( ( ( *t >>  8 ) & 0xff ) >> 2 ) <<  5 ) |
									( ( *t >> 16 ) & 0xff ) >> 3;
				++t;
				++i;					
			}
			
			break;
		}
		
		case 4:
		{
			texel_array = ( unsigned short * )texture->texel_array;

			t = ( unsigned int * )texture->texel_array;
						
			if( use_5551 )
			{
				texture->texel_type = GL_UNSIGNED_SHORT_5_5_5_1;

				while( i != s )
				{
					*texel_array++ = ( ( (   *t         & 0xff ) >> 3 ) << 11 ) |
									 ( ( ( ( *t >>  8 ) & 0xff ) >> 3 ) <<  6 ) |
									 ( ( ( ( *t >> 16 ) & 0xff ) >> 3 ) <<  1 ) |
								         ( ( *t >> 24 ) & 0xff ) >> 7;
					++t;
					++i;
				}
			}
			else
			{
				texture->texel_type = GL_UNSIGNED_SHORT_4_4_4_4;
		
				while( i != s )
				{
					*texel_array++ = ( ( (   *t         & 0xff ) >> 4 ) << 12 ) |
									 ( ( ( ( *t >>  8 ) & 0xff ) >> 4 ) <<  8 ) |
									 ( ( ( ( *t >> 16 ) & 0xff ) >> 4 ) <<  4 ) |
										 ( ( *t >> 24 ) & 0xff ) >> 4;
					++t;
					++i;
				}			
			}
		
			break;
		}
	}
}


/*!
	Helper function to automatically generate an OpenGLES texture.
	
	\param[in,out] texture A valid TEXTURE structure pointer.
	\param[in] flags The bit flags to use to generate the GLES texture.
	\param[in] filter The type of mipmap filtering to use.
	\param[in] anisotropic_filter The amount of anisotropic filtering to apply on the texture.
*/
void TEXTURE_generate_id( TEXTURE *texture, unsigned int flags, unsigned char filter, float anisotropic_filter )
{
	if( texture->tid ) TEXTURE_delete_id( texture );

	glGenTextures( 1, &texture->tid );

	glBindTexture( texture->target, texture->tid );
	
	
	if( !texture->compression )
	{
		switch( texture->byte )
		{
			case 1: glPixelStorei( GL_PACK_ALIGNMENT, 1 ); break;
			case 2: glPixelStorei( GL_PACK_ALIGNMENT, 2 ); break;
			case 3:
			case 4: glPixelStorei( GL_PACK_ALIGNMENT, 4 ); break;
		}

		if( flags & TEXTURE_16_BITS ) TEXTURE_convert_16_bits( texture, flags & TEXTURE_16_BITS_5551 );
	}
	

	if( flags & TEXTURE_CLAMP )
	{
		glTexParameteri( texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
	else
	{
		glTexParameteri( texture->target, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( texture->target, GL_TEXTURE_WRAP_T, GL_REPEAT );			
	}
	

	if( anisotropic_filter )
	{
		static float texture_max_anisotropy = 0.0f;
		
		if( !texture_max_anisotropy ) glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,
												   &texture_max_anisotropy );

		anisotropic_filter = CLAMP( anisotropic_filter,
									0.0f,
									texture_max_anisotropy );

		glTexParameterf( texture->target,
						 GL_TEXTURE_MAX_ANISOTROPY_EXT,
						 anisotropic_filter );
	}
	
	
	if( flags & TEXTURE_MIPMAP )
	{
		switch( filter )
		{
			case TEXTURE_FILTER_1X:
			{
				glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
				glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
								
				break;
			}
							
			case TEXTURE_FILTER_2X:
			{
				glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
				glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );				
				
				break;
			}
							
			case TEXTURE_FILTER_3X:
			{
				glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
				glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );				
				
				break;
			}
			
			case TEXTURE_FILTER_0X:
			default:
			{
				glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
				glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );					

				break;
			}
		}
	}
	else
	{
		switch( filter )
		{
			case TEXTURE_FILTER_0X:
			{
				glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
				glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			
				break;
			}
			
			default:
			{
				glTexParameteri( texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
				glTexParameteri( texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			
				break;
			}
		}
	}


	if( texture->compression )
	{
		unsigned int i		= 0,
					 width  = texture->width,
					 height = texture->height,
					 size	= 0,
					 offset = 0,
					 bsize  = texture->byte == 4 ? 16 : 32,
					 bwidth,
					 bheight;

		while( i != texture->n_mipmap )
		{
			if( width  < 1 ){ width  = 1; }
			if( height < 1 ){ height = 1; }
				
			bwidth = texture->byte == 4 ?
					 width >> 2:
					 width >> 3;

			bheight = height >> 2;	
			
			size = bwidth * bheight * ( ( bsize * texture->byte ) >> 3 );				

			if( size < 32 ){ size = 32; }
			
			glCompressedTexImage2D( texture->target,
									i,											
									texture->compression,
									width,
									height,
									0,
									size,
									&texture->texel_array[ offset ] );
			width  >>= 1;
			height >>= 1;
			
			offset += size;

			++i;
		}
	}
	else
	{
		glTexImage2D( texture->target,
					  0,
					  texture->internal_format,
					  texture->width,
					  texture->height,
					  0,
					  texture->format,
					  texture->texel_type,
					  texture->texel_array );
	}


	if( flags & TEXTURE_MIPMAP && !texture->n_mipmap ) glGenerateMipmap( texture->target );
}


/*!
	Delete the GLES texture id attached to the TEXTURE structure.
	
	\param[in,out] texture A valid TEXTURE structure pointer.
*/
void TEXTURE_delete_id( TEXTURE *texture )
{
	if( texture->tid )
	{
		glDeleteTextures( 1, &texture->tid );
		texture->tid = 0;
	}
}


/*!
	Free the TEXTURE texel array.
	
	\param[in,out] texture A valid TEXTURE structure pointer.
*/
void TEXTURE_free_texel_array( TEXTURE *texture )
{
	if( texture->texel_array )
	{
		free( texture->texel_array );
		texture->texel_array = NULL;
	}
}


/*!
	Bind the OpenGLES texture id for drawing.
	
	\param[in] texture A valid TEXTURE structure pointer.
*/
void TEXTURE_draw( TEXTURE *texture )
{
	glBindTexture( texture->target, 
				   texture->tid );
}


/*!
	Rescale a texture, please take note that this function have to be called
	before you call TEXTURE_generate_id.
	
	\param[in] texture A valid TEXTURE structure pointer.
	\param[in] width The new width of the texture.
	\param[in] height The new height of the texture.
*/
void TEXTURE_scale( TEXTURE *texture, unsigned int width, unsigned int height )
{
	unsigned int i = 0,
				 j,
				 offset,
				 *px = ( unsigned int * ) malloc( width  << 3 ),
				 *py = ( unsigned int * ) malloc( height << 3 );

	float t = 0.0f,
		  ratio_x = ( float )texture->width  / ( float )width,
		  ratio_y = ( float )texture->height / ( float )height;


	unsigned char *texel_array = ( unsigned char * ) malloc( width * height * texture->byte );

	while( i != width )
	{
		px[ i ] = ( unsigned int )t;

		t += ratio_x;

		++i;
	}


	i = 0;
	t = 0.0f;
	while( i != height )
	{
		py[ i ] = ( unsigned int )t;

		t += ratio_y;

		++i;
	}


	i = 0;
	while( i != height )
	{
		offset = py[ i ] * texture->width;

		j = 0;
		while( j != width )
		{
			switch( texture->byte )
			{
				case 1:
				{
					texel_array[ ( i * width + j ) ] = texture->texel_array[ ( offset + px[ j ] ) ];

					break;
				}

				case 2:
				{
					texel_array[ ( i * width + j ) * texture->byte     ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte     ];
					texel_array[ ( i * width + j ) * texture->byte + 1 ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte + 1 ];

					break;
				}

				case 3:
				{
					texel_array[ ( i * width + j ) * texture->byte     ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte     ];
					texel_array[ ( i * width + j ) * texture->byte + 1 ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte + 1 ];
					texel_array[ ( i * width + j ) * texture->byte + 2 ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte + 2 ];

					break;
				}

				case 4:
				{
					texel_array[ ( i * width + j ) * texture->byte     ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte     ];
					texel_array[ ( i * width + j ) * texture->byte + 1 ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte + 1 ];
					texel_array[ ( i * width + j ) * texture->byte + 2 ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte + 2 ];
					texel_array[ ( i * width + j ) * texture->byte + 3 ] = texture->texel_array[ ( offset + px[ j ] ) * texture->byte + 3 ];

					break;
				}
			}

			++j;
		}


		++i;
	}


	free( px );
	free( py );

	px =
	py = NULL;

	texture->width  = width;
	texture->height = height;

	free( texture->texel_array );
	texture->texel_array = texel_array;
}