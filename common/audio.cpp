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
	\file audio.cpp
	
	\brief The global audio implementation used by GFX.
    
	\details This source file contains the functions to start and stop the audio system.
	In addition it contains the necessary callbacks to be able to decompress and OGG
	Vorbis sound file from memory.
*/


//! Global audio structure to initialize and maintain the OpenAL device and context.
AUDIO audio;


/*!
	Start the audio system. This function will automatically use the first valid
	OpenAL device found on your device and will create an OpenAL context. Make sure
	you call this function at initialization time if you plan to use audio. In addition,
	this function will link the necessary callbacks to handle the OGG decompression
	in memory.
*/
void AUDIO_start( void )
{
	memset( &audio, 0, sizeof( AUDIO ) );

	audio.al_device = alcOpenDevice( NULL );

	audio.al_context = alcCreateContext( audio.al_device, NULL );

	alcMakeContextCurrent( audio.al_context );

	console_print( "\nAL_VENDOR:      %s\n", ( char * )alGetString ( AL_VENDOR     ) );
	console_print( "AL_RENDERER:    %s\n"  , ( char * )alGetString ( AL_RENDERER   ) );
	console_print( "AL_VERSION:     %s\n"  , ( char * )alGetString ( AL_VERSION    ) );
	console_print( "AL_EXTENSIONS:  %s\n"  , ( char * )alGetString ( AL_EXTENSIONS ) );
	
	audio.callbacks.read_func  = AUDIO_ogg_read;
	audio.callbacks.seek_func  = AUDIO_ogg_seek;
	audio.callbacks.tell_func  = AUDIO_ogg_tell;
	audio.callbacks.close_func = AUDIO_ogg_close;

	AUDIO_error();
}


/*!
	Stop the system. This function will also destroy the OpenAL context and close
	the current OpenAL device in use and reset the global audio structure. This function
	should be called inside the exit function of your app, only if you have previous
	called AUDIO_start.
*/
void AUDIO_stop( void )
{
	alcMakeContextCurrent( NULL );

	alcDestroyContext( audio.al_context );

	alcCloseDevice( audio.al_device );
	
	memset( &audio, 0, sizeof( AUDIO ) );
}


/*!
	Helper function that will allow you to track if an OpenAL error occur. In the
	case the driver return an error it will be printed on the console (XCode) or
	LogChat (Eclipse).
*/
void AUDIO_error( void )
{
	unsigned int error;

	while( ( error = glGetError() ) != GL_NO_ERROR )
	{
		char str[ MAX_CHAR ] = {""};

		switch( error )
		{
			case AL_INVALID_NAME:
			{
				strcpy( str, "AL_INVALID_NAME" );
				break;
			}

			case AL_INVALID_ENUM:
			{
				strcpy( str, "AL_INVALID_ENUM" );
				break;
			}

			case AL_INVALID_VALUE:
			{
				strcpy( str, "AL_INVALID_VALUE" );
				break;
			}

			case AL_INVALID_OPERATION:
			{
				strcpy( str, "AL_INVALID_OPERATION" );
				break;
			}

			case AL_OUT_OF_MEMORY:
			{
				strcpy( str, "AL_OUT_OF_MEMORY" );
				break;
			}
		}
		
		console_print( "[ AL_ERROR ]\nERROR: %s\n", str );
	}
}


/*!
	Function to set the location and direction of the OpenAL listener. You should
	use this function along with your camera to synchronize position and orientation.
	
	\param[in] location Specifies the position of the listener.
	\param[in] direction A normalized vector that specifies the orientation of the listener.
	\param[in] up Specifies the direction of the up vector.
*/
void AUDIO_set_listener( vec3 *location, vec3 *direction, vec3 *up )
{
	float orientation[ 6 ] = { direction->x, direction->y, direction->z,
							   up->x, up->y, up->z };

	alListener3f( AL_POSITION,
				  location->x,
				  location->y,
				  location->z );

	alListenerfv( AL_ORIENTATION, &orientation[ 0 ] );
}


//! OGG callback to read from a binary stream in memory.
size_t AUDIO_ogg_read( void *ptr, size_t size, size_t read, void *memory_ptr )
{
	unsigned int seof,
				 pos;

	MEMORY *memory = ( MEMORY * )memory_ptr;

	seof = memory->size - memory->position;

	pos = ( ( read * size ) < seof ) ?
		  pos = read * size :
		  pos = seof;

	if( pos )
	{
		memcpy( ptr, memory->buffer + memory->position, pos );
		
		memory->position += pos;
	}

	return pos;
}


//! OGG callback to seek through a binary stream in memory.
int AUDIO_ogg_seek( void *memory_ptr, ogg_int64_t offset, int stride )
{
	unsigned int pos;

	MEMORY *memory = ( MEMORY * )memory_ptr;

	switch( stride )
	{
		case SEEK_SET:
		{
			pos = ( memory->size >= offset ) ?
				  pos = ( unsigned int )offset :
				  pos = memory->size;

			memory->position = pos;

			break;
		}

		case SEEK_CUR:
		{
			unsigned int seof = memory->size - memory->position;

			pos = ( offset < seof ) ?
				  pos = ( unsigned int )offset :
				  pos = seof;

			memory->position += pos;

			break;
		}

		case SEEK_END:
		{
			memory->position = memory->size + 1;

			break;
		}
	};

	return 0;
}


//! OGG callback to return the current position of the cursor of a binary stream in memory.
long AUDIO_ogg_tell( void *memory_ptr )
{
	MEMORY *memory = ( MEMORY * )memory_ptr;

	return memory->position;
}


//! Return wheter or not the memory pointer is closed.
int AUDIO_ogg_close( void *memory_ptr )
{ return memory_ptr ? 1 : 0; }
