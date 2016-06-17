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
	\file sound.cpp
	
    \brief Provide an easy to use interface to deal with OpenAL sound source and sound buffer.
		
	\details By using the SOUND interface you can easily manipulate OpenAL sound source(s) and sound
	buffer(s). The implementation allows you to create and manipulate ambient or positional sounds,
	as well as static (in memory) or streamed (in realtime). 
*/


/*!
	Load and create a new OGG Vorbis SOUNDBUFFER from MEMORY for a static buffer and store
	the buffer into audio memory.
	
	\param[in] name The internal name to use for the new SOUNDBUFFER.
	\param[in] memory The MEMORY stream that contain an OGG file.
	
	\return Return a new SOUNDBUFFER structure pointer if the OGG memory stream have been loaded
	properly, else return a NULL SOUNDBUFFER structure pointer.
*/
SOUNDBUFFER *SOUNDBUFFER_load( char *name, MEMORY *memory )
{
	char ext[ MAX_CHAR ] = {""};
	
	get_file_extension( memory->filename, ext, 1 );
	
	if( !strcmp( ext, "OGG" ) )
	{
		SOUNDBUFFER *soundbuffer = ( SOUNDBUFFER * ) calloc( 1, sizeof( SOUNDBUFFER ) );
		
		unsigned int size;
		
		strcpy( soundbuffer->name, name );
		
		soundbuffer->file = ( OggVorbis_File * ) calloc( 1, sizeof( OggVorbis_File ) );
		
		ov_open_callbacks( memory,
						   soundbuffer->file,
						   NULL,
						   0,
						   audio.callbacks );

		soundbuffer->info = ov_info( soundbuffer->file, -1 );

		if( soundbuffer->info )
		{
			size = ( ( unsigned int )ov_pcm_total( soundbuffer->file, -1 ) *
												   soundbuffer->info->channels << 1 );

			int count,
				bit;

			char *data  = ( char * ) malloc( size ),
				 *start = data;

			while( ( count = ov_read( soundbuffer->file,
									  start,
									  MAX_CHUNK_SIZE,
									  0,
									  2,
									  1,
									  &bit ) ) > 0 ) start += count;

			alGenBuffers( 1, &soundbuffer->bid[ 0 ] );

			alBufferData( soundbuffer->bid[ 0 ],
						  ( soundbuffer->info->channels == 1 ) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
						  data,
						  size,
						  soundbuffer->info->rate );
			
			free( data );
		}

		AUDIO_ogg_close( memory );

		ov_clear( soundbuffer->file );
		
		free( soundbuffer->file );
		
		soundbuffer->file = NULL;
		
		soundbuffer->info = NULL;
		
		return soundbuffer;
	}
	
	return NULL;
}


/*!
	Load and create a new OGG Vorbis SOUNDBUFFER from MEMORY for a streamed buffer but do not store
	the buffer into audio memory and keep it alive into client memory for streaming.
	
	\param[in] name The internal name to use for the new SOUNDBUFFER.
	\param[in] memory The MEMORY stream that contain an OGG file.
	
	\return Return a new SOUNDBUFFER structure pointer if the OGG memory stream have been loaded
	properly, else return a NULL SOUNDBUFFER structure pointer.
*/
SOUNDBUFFER *SOUNDBUFFER_load_stream( char *name, MEMORY *memory )
{
	char ext[ MAX_CHAR ] = {""};
	
	get_file_extension( memory->filename, ext, 1 );
	
	if( !strcmp( ext, "OGG" ) )
	{
		unsigned int i = 0;
		
		SOUNDBUFFER *soundbuffer = ( SOUNDBUFFER * ) calloc( 1, sizeof( SOUNDBUFFER ) );
		
		strcpy( soundbuffer->name, name );
		
		soundbuffer->memory = memory;
		
		soundbuffer->file = ( OggVorbis_File * ) calloc( 1, sizeof( OggVorbis_File ) );
		
		ov_open_callbacks( memory,
						   soundbuffer->file,
						   NULL,
						   0,
						   audio.callbacks );

		soundbuffer->info = ov_info( soundbuffer->file, -1 );
		
		alGenBuffers( MAX_BUFFER, &soundbuffer->bid[ 0 ] );
		
		while( i != MAX_BUFFER )
		{
			SOUNDBUFFER_decompress_chunk( soundbuffer, i );
			++i;
		}
		
		return soundbuffer;
	}

	return NULL;
}


/*!
	Internal function use to decompress audio chunks of an SOUNBUFFER.
	
	\param[in,out] soundbuffer A valid SOUNBUFFER structure pointer.
	\param[in] buffer_index The buffer index to fill with new audio data.
	
	\return Return 1 if the function was able to fill the buffer with data, else return 0 which
	represent that the EOS have been reached.
*/
unsigned char SOUNDBUFFER_decompress_chunk( SOUNDBUFFER *soundbuffer, unsigned int buffer_index )
{
	char buffer[ MAX_CHUNK_SIZE ] = {""};

	int size = 0,
		bit;

	while( size < MAX_CHUNK_SIZE )
	{
		int count = ov_read( soundbuffer->file,
							 buffer + size,
							 MAX_CHUNK_SIZE - size,
							 0,
							 2,
							 1,
							 &bit );
							 
		if( count > 0 ) size += count;
		
		else break;
	}

	if( !size ) return 0;

	alBufferData( soundbuffer->bid[ buffer_index ],
				  ( soundbuffer->info->channels == 1 ) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
				  buffer,
				  size,
				  soundbuffer->info->rate );

	return 1;
}


/*!
	Free a previously initialized SOUNDBUFFER structure pointer.
	
	\param[in,out] soundbuffer A valid SOUNBUFFER structure pointer.
	
	\return Return a NULL SOUNDBUFFER structure pointer.
*/
SOUNDBUFFER *SOUNDBUFFER_free( SOUNDBUFFER *soundbuffer )
{
	unsigned int i = 0;
	
	while( i != MAX_BUFFER )
	{
		if( soundbuffer->bid[ i ] ) alDeleteBuffers( 1, &soundbuffer->bid[ i ] );
		++i;
	}
	
	if( soundbuffer->file )
	{
		ov_clear( soundbuffer->file );
		free( soundbuffer->file );
	}


	if( soundbuffer->memory ) AUDIO_ogg_close( soundbuffer->memory );

	free( soundbuffer );
	return NULL;
}


/*!
	Add a new sound source that will be maintained by OpenAL.
	
	\param[in] name The internal name to use for the new SOUND source.
	\param[in] soundbuffer The SOUNDBUFFER structure pointer to use for the new SOUND source buffer.
	
	\return Return a new SOUND structure pointer.
*/
SOUND *SOUND_add( char *name, SOUNDBUFFER *soundbuffer )
{
	vec3 tmp = { 0.0f, 0.0f, 0.0f };
	
	SOUND *sound = ( SOUND * ) calloc( 1, sizeof( SOUND ) );

	strcpy( sound->name, name );
	
	sound->soundbuffer = soundbuffer;

	alGenSources( 1, &sound->sid );
	
    alSource3f( sound->sid, AL_POSITION, tmp.x, tmp.y, tmp.z );

    alSource3f( sound->sid, AL_VELOCITY, tmp.x, tmp.y, tmp.z );
	
    alSource3f( sound->sid, AL_DIRECTION, tmp.x, tmp.y, tmp.z );
	
    alSourcef( sound->sid, AL_ROLLOFF_FACTOR, 1.0f );
	
    alSourcei( sound->sid, AL_SOURCE_RELATIVE, AL_TRUE );
	
	alSourcef( sound->sid, AL_GAIN, 0.0f );
	
	alSourcef( sound->sid, AL_PITCH, 1.0f );
	
	return sound;
}


/*!
	Free a previously initialized SOUND structure.
	
	\param[in,out] sound A valid SOUND structure pointer.
	
	\return Return A NULL SOUND structure pointer.
*/
SOUND *SOUND_free( SOUND *sound )
{
	if( sound->sid )
	{
		SOUND_stop( sound );
		
		alDeleteSources( 1, &sound->sid );
	}

	free( sound );
	return NULL;
}


/*!
	Start playing a SOUND source.
	
	\param[in,out] sound A valid SOUND structure pointer.
	\param[in] loop Determine if the SOUND source should loop when the EOS is reached.
*/
void SOUND_play( SOUND *sound, int loop )
{
	sound->loop = loop;
	
	if( !sound->soundbuffer->bid[ 1 ] )
	{
		alSourcei( sound->sid, AL_LOOPING, loop );
		
		alSourcei( sound->sid,
				   AL_BUFFER,
				   sound->soundbuffer->bid[ 0 ] );
	}
	else
	{
		alSourceQueueBuffers( sound->sid,
							  MAX_BUFFER,
							  &sound->soundbuffer->bid[ 0 ] );
	}

	alSourcePlay( sound->sid );
}


/*!
	Pause a SOUND source.
	
	\param[in,out] sound A valid SOUND structure pointer.
*/
void SOUND_pause( SOUND *sound )
{ alSourcePause( sound->sid ); }


/*!
	Stop a SOUND source.
	
	\param[in,out] sound A valid SOUND structure pointer.
*/
void SOUND_stop( SOUND *sound )
{ alSourceStop( sound->sid ); }


/*!
	Set the playback speed of a SOUND source.
	
	\param[in,out] sound A valid SOUND structure pointer.
	\param[in] speed The speed factor that should be applied to the pitch property of the SOUND source.
*/
void SOUND_set_speed( SOUND *sound, float speed )
{ alSourcef( sound->sid, AL_PITCH, speed ); }


/*!
	Set the volume of a SOUND source.
	
	\param[in,out] sound A valid SOUND structure pointer.
	\param[in] volume A value in the range of 0 to 1 to affect the current volume of the SOUND source.
*/						
void SOUND_set_volume( SOUND *sound, float volume )
{ alSourcef( sound->sid, AL_GAIN, volume ); }


/*!
	Set the 3D location of a SOUND source in space.
	
	\param[in,out] sound A valid SOUND structure pointer.
	\param[in] location The location of the SOUND source in world space coordinate.
	\param[in] reference_distance The distance that the SOUND can be heard from.
*/
void SOUND_set_location( SOUND *sound, vec3 *location, float reference_distance )
{
    alSourcei( sound->sid, AL_SOURCE_RELATIVE, AL_FALSE );
	
	alSourcef( sound->sid, AL_REFERENCE_DISTANCE, reference_distance );
	
	alSource3f( sound->sid,
				AL_POSITION,
				location->x,
				location->y,
				location->z );
}


/*!
	Rewind a SOUND source.
	
	\param[in,out] sound A valid SOUND structure pointer.
*/
void SOUND_rewind( SOUND *sound )
{ alSourceRewind( sound->sid ); }


/*!
	Get the current plaback time of a SOUND source.
	
	\param[in] sound A valid SOUND structure pointer.
	
	\return Return the playback time in seconds (where 1.0f = 1 sec).
*/
float SOUND_get_time( SOUND *sound )
{
	float playback_time = 0.0f;
	
	alGetSourcef( sound->sid, AL_SEC_OFFSET, &playback_time );
	
	return playback_time;
}


/*!
	Get the current playback state of a SOUND source.
	
	\param[in] sound A valid SOUND structure pointer.
	
	\return Return the AL_STATE for the current SOUND source.
*/
int SOUND_get_state( SOUND *sound )
{
	int state = 0;
	
	alGetSourcei( sound->sid, AL_SOURCE_STATE, &state );
	
	return state;
}


/*!
	Get the current volume of a SOUND source.
	
	\param[in] sound A valid SOUND structure pointer.
	
	\return Return the volume of a SOUND source.
*/
float SOUND_get_volume( SOUND *sound )
{
	float volume = 0.0f;
	
	alGetSourcef( sound->sid, AL_GAIN, &volume );
	
	return volume;
}


/*!
	Function to update the queue of a streamed SOUND source.
	
	\param[in] sound A valid SOUND structure pointer.	
*/
void SOUND_update_queue( SOUND *sound )
{
	unsigned int i = 0;

	int p,
		q;

    alGetSourcei( sound->sid, AL_BUFFERS_PROCESSED, &p );

	alGetSourcei( sound->sid, AL_BUFFERS_QUEUED, &q );
	
	while( p-- )
    {
		unsigned int bid;
		
        alSourceUnqueueBuffers( sound->sid,
								1,
								&bid );

        while( i != MAX_BUFFER )
        {
        	if( bid == sound->soundbuffer->bid[ i ] ) break;

        	++i;
        }

		if( SOUNDBUFFER_decompress_chunk( sound->soundbuffer, i ) )
		{
			alSourceQueueBuffers( sound->sid,
								  1,
								  &bid );
		}
    }
	

	if( !q && sound->loop )
	{
		AUDIO_ogg_seek( sound->soundbuffer->file->datasource,
						0,
						SEEK_SET );

		while( i != MAX_BUFFER )
		{
			SOUNDBUFFER_decompress_chunk( sound->soundbuffer, i );
			++i;
		}

		SOUND_play( sound, sound->loop );
	}
}
