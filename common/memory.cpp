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
	\file memory.cpp
	
	\brief Re-implement the standard FILE interface but using a memory stream.
    
	\details The MEMORY structure allow you to deal with files but strictly in memory. This
	implementation is ideal for the loading game assets. In addition, the MEMORY structure offers
	you built-in functionalities to read and extract files from and Android APK file, directly in
	C so you won't have to use the Java API.
*/


/*!
	Open/Extract a file from disk and load it in memory.
	
	\param[in] filename The file to load in memory.
	\param[in] relative_path Determine if the filename is an absolute or relative path.
	
	\return Return a MEMORY structure pointer if the file is found and loaded, instead will return
	NULL.
*/
MEMORY *mopen( char *filename, unsigned char relative_path )
{
	#ifdef __IPHONE_4_0

		FILE *f;
		
		char fname[ MAX_PATH ] = {""};
		
		if( relative_path )
		{
			get_file_path( getenv( "FILESYSTEM" ), fname );
			
			strcat( fname, filename );
		}
		else strcpy( fname, filename );

		f = fopen( fname, "rb" );
		
		if( !f ) return NULL;
		
		
		MEMORY *memory = ( MEMORY * ) calloc( 1, sizeof( MEMORY ) );
		
		strcpy( memory->filename, fname );
		
		
		fseek( f, 0, SEEK_END );
		memory->size = ftell( f );
		fseek( f, 0, SEEK_SET );
		
		
		memory->buffer = ( unsigned char * ) calloc( 1, memory->size + 1 );
		fread( memory->buffer, memory->size, 1, f );
		memory->buffer[ memory->size ] = 0;
		
		
		fclose( f );
		
		return memory;
	
	
	#else
	
		char fpath[ MAX_PATH ] = {""},
			 fname[ MAX_PATH ] = {""};

		unzFile		    uf;
		unz_file_info   fi;
		unz_file_pos    fp;

		strcpy( fpath, getenv( "FILESYSTEM" ) );

		uf = unzOpen( fpath );
		
		if( !uf ) return NULL;

		if( relative_path ) sprintf( fname, "assets/%s", filename );
		else strcpy( fname, filename );
		
		unzGoToFirstFile( uf );

		MEMORY *memory = ( MEMORY * ) calloc( 1, sizeof( MEMORY ) );

		unzGetFilePos( uf, &fp );
		
		if( unzLocateFile( uf, fname, 1 ) == UNZ_OK )
		{
			unzGetCurrentFileInfo(  uf,
								   &fi,
									memory->filename,
									MAX_PATH,
									NULL, 0,
									NULL, 0 );
		
			if( unzOpenCurrentFilePassword( uf, NULL ) == UNZ_OK )
			{
				memory->position = 0;
				memory->size	 = fi.uncompressed_size;
				memory->buffer   = ( unsigned char * ) realloc( memory->buffer, fi.uncompressed_size + 1 );
				memory->buffer[ fi.uncompressed_size ] = 0;

				while( unzReadCurrentFile( uf, memory->buffer, fi.uncompressed_size ) > 0 ){}

				unzCloseCurrentFile( uf );

				unzClose( uf );
					
				return memory;
			}
		}
		
		unzClose( uf );

		return NULL;
		
	#endif
}


/*!
	Close and free a previously initialized MEMORY stream.
	
	\param[in,out] memory A valid MEMORY structure pointer.
	
	\return Return a NULL MEMORY structure pointer.
*/
MEMORY *mclose( MEMORY *memory )
{
	if( memory->buffer ) free( memory->buffer );
	
	free( memory );
	return NULL;
}


/*!
	Similar as the read method, this function read a chunk of bytes
	at starting from the current location of the cursor in the memory stream
	to a specific size. The result will be stored in the dst pointer.
	
	\param[in,out] memory A valid MEMORY structure pointer.
	\param[in,out] dst A void pointer to store the bytes that was read from this function.
	\param[in] size The number of bytes to read from the memory stream.
	
	\return Return the number of bytes that the function store in the dst pointer.
*/
unsigned int mread( MEMORY *memory, void *dst, unsigned int size )
{
	if( ( memory->position + size ) > memory->size )
	{ size = memory->size - memory->position; }

	memcpy( dst, &memory->buffer[ memory->position ], size );
	
	memory->position += size;

	return size;
}


/*!
	Insert text at a specific position in a memory stream.
	
	\param[in,out] memory A valid MEMORY structure pointer with a loaded buffer.
	\param[in] str The string of text to insert.
	\param[in] position The position (in bytes) to inser the text.
*/
void minsert( MEMORY *memory, char *str, unsigned int position )
{
	unsigned int s1 = strlen( str ),
				 s2 = memory->size + s1 + 1;

	char *buffer = ( char * )memory->buffer,
		 *tmp	 = ( char * )calloc( 1, s2 );
	
	if( position )
	{ strncpy( &tmp[ 0 ], &buffer[ 0 ], position ); }

	strcat( &tmp[ position ], str );
	
	strcat( &tmp[ position + s1 ], &buffer[ position ] );

	memory->size = s2;
	
	free( memory->buffer );
	memory->buffer = ( unsigned char * )tmp;	
}
