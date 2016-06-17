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
	\file thread.cpp
	
    \brief Bridge interface between pthread and GFX.
		
	\details The THREAD structure provide an easy to use mechanism to deal with pthreads. Using the
	THREAD interface you can easily manage the creation of new thread, set their function
	callback, priorities and easy manage their states. Please take note that the implementation
	doesn't use a shared OpenGLES context, so every GFX functions that directly access some
	OpenGLES calls cannot inside the thread function callback cannot be used and should be
	called from the main thread.
*/


/*!
	The internal thread callback used by the THREAD structure. This function is the default
	function used by every new THREAD created and contain a loop that will dispatch to execution
	pointer to the appropriate THREADCALLBACK function linked to each specific thread.
	
	\param[in] ptr Used internally to dispatch the current THREAD pointer to the callback (see code below).
*/
void *THREAD_run( void *ptr )
{
	THREAD *thread = ( THREAD * )ptr;

	struct sched_param param;

	pthread_setschedparam( thread->thread, SCHED_RR, &param );

	param.sched_priority = thread->priority;

	while( thread->state )
	{
		if( thread->state == PLAY && thread->threadcallback )
		{ thread->threadcallback( thread ); }
		
		usleep( thread->timeout * 1000 );
	}
	
	pthread_exit( NULL );
	
	return NULL;
}


/*!
	Function to create a new THREAD structure. By default each created thread state will
	be set to the default PAUSE state until the THREAD_play function is called.
	
	\param[in] threadcallback The newly created thread callback function.
	\param[in] userdata You can use this variable to attach any other type of information to the THREAD structure and then get access to it from your THREADCALLBACK function.
	\param[in] priority The thread priority.
	\param[in] timeout The thread timeout value in millisecond. This can be use to adjust the thread FPS.

	\return Return a THREAD structure pointer.
*/
THREAD *THREAD_create( THREADCALLBACK *threadcallback,
					   void			  *userdata,
					   int			   priority,
					   unsigned int	   timeout )
{
	THREAD *thread = ( THREAD * ) calloc( 1, sizeof( THREAD ) );

	thread->threadcallback = threadcallback;
	
	thread->priority = priority;
	thread->userdata = userdata;
	thread->timeout  = timeout;

	THREAD_pause( thread );

	thread->thread_hdl = pthread_create( &thread->thread,
										 NULL,
										 THREAD_run,
										 ( void * )thread );
	return thread;
}


/*!
	Free a previously initialize THREAD. This function will automatically stop the thread,
	and join it to the main process before getting freed.
	
	\param[in] thread A valid THREAD structure pointer.
	
	\return A NULL THREAD structure pointer.
*/
THREAD *THREAD_free( THREAD *thread )
{
	THREAD_stop( thread );
	
	pthread_join( thread->thread, NULL );
	
	free( thread );
	return NULL;
}


/*!
	Set the THREAD structure callback.
	
	\param[in] thread A valid THREAD structure pointer.
	\param[in] threadcallback The THREAD callback function to use.
	
*/
void THREAD_set_callback( THREAD *thread, THREADCALLBACK *threadcallback )
{ thread->threadcallback = threadcallback; }


/*!
	Set the THREAD state to PLAY.
	
	\param[in] thread A valid THREAD structure pointer.
*/
void THREAD_play( THREAD *thread )
{ thread->state = PLAY; }


/*!
	Set the THREAD state to PAUSE.
	
	\param[in] thread A valid THREAD structure pointer.
*/
void THREAD_pause( THREAD *thread )
{
	thread->state = PAUSE;
	
	usleep( thread->timeout * 1000 );
}


/*!
	Set the THREAD state to STOP. Take note that this will also release the thread
	handle and the THREAD state cannot be set to PLAY again.
	
	\param[in] thread A valid THREAD structure pointer.
*/

void THREAD_stop( THREAD *thread )
{
	thread->state = STOP;

	usleep( thread->timeout * 1000 );
}
