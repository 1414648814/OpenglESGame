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

#ifndef PROGRAM_H
#define PROGRAM_H

/*!
	\file program.h
	
	\brief Function prototypes and definitions to use with the PROGRAM structure.
*/


//! Structure to deal with GLSL uniform variables.
typedef struct
{
	//! The name of the uniform.
	char			name[ MAX_CHAR ];
	
	//! The variable type for this uniform.
	unsigned int	type;
	
	//! The location id maintained by GLSL for this uniform.
	int				location;
	
	//! Determine if the uniform is constant or shoud be updated every frame.
	unsigned char	constant;

} UNIFORM;


//! Structure to deal with vertex attribute variables.
typedef struct
{
	//! The name of the vertex attribute.
	char			name[ MAX_CHAR ];
	
	//! The variable type for this vertex attribute.
	unsigned int	type;
	
	//! The location of the id maintained GLSL for this vertex attribute. 
	int				location;
	
} VERTEX_ATTRIB;


//! Draw callback function prototype.
typedef void( PROGRAMDRAWCALLBACK( void * ) );

//! Callback function prototypes for binding vertex attribute before compilation.
typedef void( PROGRAMBINDATTRIBCALLBACK( void * ) );


//! Structure to easily handle GLSL programs.
typedef struct
{
	//! The internal name to use for the shader program.
	char						 name[ MAX_CHAR ];
	
	//! The vertex shader.
	SHADER						 *vertex_shader;
	
	//! The fragment shader.
	SHADER						 *fragment_shader;
	
	//! The internal GLES program id for the shader program.
	unsigned int				 pid;
	
	//! The number of uniforms.
	unsigned char				 uniform_count;
	
	//! Array of UNIFORM variables.
	UNIFORM						 *uniform_array;	
	
	//! The number of vertex attributes.
	unsigned char				 vertex_attrib_count;
	
	//! Array of vertex attributes.
	VERTEX_ATTRIB				 *vertex_attrib_array;
	
	//! The program draw callback.
	PROGRAMDRAWCALLBACK			 *programdrawcallback;
	
	//! The bind attribute callback.
	PROGRAMBINDATTRIBCALLBACK	 *programbindattribcallback;

} PROGRAM;


PROGRAM *PROGRAM_init( char *name );

PROGRAM *PROGRAM_free( PROGRAM *program );

PROGRAM *PROGRAM_create( char *name, char *vertex_shader_filename, char *fragment_shader_filename, unsigned char relative_path, unsigned char debug_shader, PROGRAMBINDATTRIBCALLBACK *programbindattribcallback, PROGRAMDRAWCALLBACK *programdrawcallback );

unsigned char PROGRAM_link( PROGRAM *program, unsigned char debug );

void PROGRAM_set_draw_callback( PROGRAM *program, PROGRAMDRAWCALLBACK *programdrawcallback );

void PROGRAM_set_bind_attrib_location_callback( PROGRAM *program, PROGRAMBINDATTRIBCALLBACK *programbindattribcallback );

char PROGRAM_get_vertex_attrib_location( PROGRAM *program, char *name );

char PROGRAM_get_uniform_location( PROGRAM *program, char *name );

void PROGRAM_delete_id( PROGRAM *program );

void PROGRAM_draw( PROGRAM *program );

void PROGRAM_reset( PROGRAM *program );

unsigned char PROGRAM_load_gfx( PROGRAM *program, PROGRAMBINDATTRIBCALLBACK	*programbindattribcallback, PROGRAMDRAWCALLBACK	*programdrawcallback, char *filename, unsigned char	debug_shader, unsigned char relative_path );

#endif
