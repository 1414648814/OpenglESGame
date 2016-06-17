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

#ifndef SOUND_H
#define SOUND_H

/*!
	\file sound.h
	
	\brief Function prototypes and definitions to use with SOUND and SOUNDBUFFER structures.
*/


//! Max amount of buffer to stream audio chunks. (Default quad buffered)
#define MAX_BUFFER 4

//! The maximum size for each chunk. (Default 8K)
#define MAX_CHUNK_SIZE 1024 << 3


//! Sound buffer structure definition.
typedef struct
{
	//! Internal name for the sound buffer.
	char			name[ MAX_CHAR ];
	
	//! The OGG Vorbis file pointer.
	OggVorbis_File	*file;

	//! Contains information about the OGG Vorbist stream.
	vorbis_info		*info;
	
	//! The MEMORY structure pointer to use with this SOUNDBUFFER.
	MEMORY			*memory;
	
	//! The buffer ids maintained internally  by OpenAL.
	unsigned int	bid[ MAX_BUFFER ];

} SOUNDBUFFER;


//! Sound source structure definition.
typedef struct
{
	//! Internal name of the sound source.
	char			name[ MAX_CHAR ];
	
	//! The sound source id maintained by OpenAL.
	unsigned int	sid;

	//! Determine if the sound source should loop when the end of the sound buffer is reached.
	int				loop;

	//! SOUNDBUFFER structure pointer to use for this sound source. (Only for streamed sound source.)
	SOUNDBUFFER		*soundbuffer;

} SOUND;


SOUNDBUFFER *SOUNDBUFFER_load( char *name, MEMORY *memory );

SOUNDBUFFER *SOUNDBUFFER_load_stream( char *name, MEMORY *memory );

unsigned char SOUNDBUFFER_decompress_chunk( SOUNDBUFFER *soundbuffer, unsigned int buffer_index );

SOUNDBUFFER *SOUNDBUFFER_free( SOUNDBUFFER *soundbuffer );

SOUND *SOUND_add( char *name, SOUNDBUFFER *soundbuffer );

SOUND *SOUND_free( SOUND *sound );

void SOUND_play( SOUND *sound, int loop );

void SOUND_pause( SOUND *sound );

void SOUND_stop( SOUND *sound );

void SOUND_set_speed( SOUND *sound, float speed );

void SOUND_set_volume( SOUND *sound, float volume );

void SOUND_set_location( SOUND *sound, vec3 *location, float reference_distance );

void SOUND_rewind( SOUND *sound );

float SOUND_get_time( SOUND *sound );

int SOUND_get_state( SOUND *sound );

float SOUND_get_volume( SOUND *sound );

void SOUND_update_queue( SOUND *sound );

#endif
