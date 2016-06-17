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

#ifndef GFX_H
#define GFX_H

/*! \mainpage GFX 3D Engine

GFX is a cross platform OpenGL ES v2.0 based 2D and 3D Game Engine for 
the next generation of mobile operating system such as iOS and Android. 
It is coded in pure C and C++ and allow you to create state of the art 
2D and 3D games for your favorite mobile platform. GFX is extremely small, 
fast, scalable, flexible and is extremely easy to use.

Please take note that the GFX engine is NOT a as complete as SIO2, and 
it only provide a subsets of the API provided by the SIO2 engine. The 
GFX engine is basically just a VERY light derived version of SIO2 that 
I create from the bunch of functions that I ended up with while 
integrating OpenGLES v2.0 to the SIO2 engine.

\htmlonly
<center><img border="0" src="../xtinction2.jpg" border="0"><br><font size="1">FPS created using GFX with realtime dynamic lights and shadows.</font></center>
\endhtmlonly

Contrarily to SIO2 in GFX you have to code everything by yourself. GFX only 
provides you a generic framework and a bunch of helper functions for you to 
create your games and 3d app. As a result GFX is more targeted to advanced 
programmers, however if you are looking for a more easy to use and 
complete solution for your game and graphics applications visit 
http://sio2interactive.com for more information about what the SIO2 
framework can provide.


\author http://gfx.sio2interactive.com
\author sio2interactive@gmail.com

[ v1.0.22 ]

- First initial public release
- Repackaging of the API to deliver them as a framework instead of just a bunch of functions.
- Initial documentation
- Cross platform C/C++ API (iOS and Android).
- Full GLSL ES shader support.
- Built-in matrix system similar as in OpenGL.
- Full Wavefront OBJ and MT Support.
- Skeletal Animation (MD5 version 10)
- Full PNG Texture support
- Automatic 16bits texture conversion
- Mipmapping, Anisotropic Filtering
- PVR Texture Compression
- Full Physics support for Rigid and Dynamic bodies
- Automatic triangles to triangle strips optimization
- Audio support through OpenAL and OGG Vorbis
- Full OpenAL support
- OGG Vorbis support
- Built-in functions to handle dynamic lights.
- Matrix and Vector math implementation.
- Memory system to deal with files that support .ZIP decompression.
- Built-in Recast and Detour support for AI and pathfinding.
- Built-in Bullet Physics Support.
- Cross platform thread interface.
- Frustum and advanced clipping support.
- TTF font file support for automatic font texture generation.
- Matrix stack mechanism that mimic standard OpenGL way of dealing with matrices.
- Implementation of the most popular GLU functions.
- A lot more...


[ v1.0.23 ]
- Report/Prevent initialization error(s) for OpenGLES and OpenAL
- Draw function (OBJ,MD5) now return the number of indices sent for drawing (if any).
- New TEXTURE_scale function.

*/


/*!
	\file gfx.h
	
	\brief Function prototypes and definitions to use with the GFX structure.
*/


#define GFX_COPYRIGHT		"Copyright (C) 2011 Romain Marucchi-Foino. All Rights Reserved."
#define GFX_RENDERER		"GFX"
#define GFX_VERSION_MAJOR	1
#define GFX_VERSION_MINOR	0
#define GFX_VERSION_PATCH	23


#ifdef __IPHONE_4_0

	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>

	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
	
	#include "png.h"
	#include "zlib.h"
	#include "unzip.h"
	#include "NvTriStrip.h"
	
	#include "btBulletDynamicsCommon.h"
	#include "btSoftRigidDynamicsWorld.h"
	#include "btSoftBodyRigidBodyCollisionConfiguration.h"
	#include "btShapeHull.h"
	#include "btSoftBodyHelpers.h"
	#include "btSoftBody.h"
	#include "btGImpactShape.h"
	#include "btGImpactCollisionAlgorithm.h"
	#include "btBulletWorldImporter.h"
	
	#include "Recast.h"
	#include "DetourDebugDraw.h"
	#include "DetourNavMesh.h"
	#include "DetourNavMeshBuilder.h"
	
	#include "stb_truetype.h"

	#include "vorbisfile.h"

#else // Android

	#include <jni.h>
	#include <android/log.h>

	#include <EGL/egl.h>
	#include "GLES2/gl2.h"
	#include "GLES2/gl2ext.h"

	#include "png/png.h"

	#include "zlib/zlib.h"
	#include "zlib/unzip.h"

	#include "nvtristrip/NvTriStrip.h"

	#include "bullet/btAlignedAllocator.h"
	#include "bullet/btBulletDynamicsCommon.h"
	#include "bullet/btSoftRigidDynamicsWorld.h"
	#include "bullet/btSoftBodyRigidBodyCollisionConfiguration.h"
	#include "bullet/btShapeHull.h"
	#include "bullet/btSoftBodyHelpers.h"
	#include "bullet/btSoftBody.h"
	#include "bullet/btGImpactShape.h"
	#include "bullet/btGImpactCollisionAlgorithm.h"
	#include "bullet/btBulletWorldImporter.h"

	#include "recast/Recast.h"
	#include "detour/DetourDebugDraw.h"
	#include "detour/DetourNavMesh.h"
	#include "detour/DetourNavMeshBuilder.h"

	#include "ttf/stb_truetype.h"

	#include "openal/al.h"
	#include "openal/alc.h"

	#include "vorbis/vorbisfile.h"


	extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
	extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
	extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;

#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/time.h>
#include <unistd.h>

#include "thread.h"
#include "types.h"
#include "matrix.h"
#include "vector.h"
#include "utils.h"
#include "memory.h"
#include "shader.h"
#include "program.h"
#include "texture.h"
#include "obj.h"
#include "navigation.h"
#include "font.h"
#include "audio.h"
#include "sound.h"
#include "light.h"
#include "md5.h"

//! The depth of the modelview matrix stack.
#define MAX_MODELVIEW_MATRIX	8

//! The depth of the projection matrix stack.
#define MAX_PROJECTION_MATRIX	2

//! The depth of the texture matrix stack.
#define MAX_TEXTURE_MATRIX		2

enum
{
	//! The modelview matrix identifier.
	MODELVIEW_MATRIX = 0,

	//! The projection matrix identifier.
	PROJECTION_MATRIX = 1,
	
	//! The texture matrix identifier.	
	TEXTURE_MATRIX	  = 2
};


//! The definition of the global GFX structure. This structure maintain the matrix stacks and current indexes. 
typedef struct
{
	//! The current matrix mode (either MODELVIEW_MATRIX (default), PROJECTION_MATRIX, TEXTURE_MATRIX).
	unsigned char	matrix_mode;
	
	//! The current modelview matrix index in the stack.
	unsigned char	modelview_matrix_index;

	//! The current projection matrix index in the stack.
	unsigned char	projection_matrix_index;
	
	//! The current texture matrix index in the stack.	
	unsigned char	texture_matrix_index;

	//! Array of 4x4 matrix that represent the modelview matrix stack.
	mat4			modelview_matrix[ MAX_MODELVIEW_MATRIX ];

	//! Array of 4x4 matrix that represent the projection matrix stack.
	mat4			projection_matrix[ MAX_PROJECTION_MATRIX ];

	//! Array of 4x4 matrix that represent the texture matrix stack.
	mat4			texture_matrix[ MAX_TEXTURE_MATRIX ];
	
	//! Used to store the result of the modelview matrix multiply by the projection matrix. \sa GFX_get_modelview_projection_matrix
	mat4			modelview_projection_matrix;
	
	//! Used to store the result of the inverse, tranposed modelview matrix. \sa GFX_get_normal_matrix
	mat3			normal_matrix;

} GFX;

//! Global GFX structure. Declared as extern in gfx.h and implemented in gfx.cpp
extern GFX gfx;


void GFX_start( void );

void GFX_error( void );

void GFX_set_matrix_mode( unsigned int mode );

void GFX_load_identity( void );

void GFX_push_matrix( void );

void GFX_pop_matrix( void );

void GFX_load_matrix( mat4 *m );

void GFX_multiply_matrix( mat4 *m );

void GFX_translate( float x, float y, float z );

void GFX_rotate( float angle, float x, float y, float z );

void GFX_scale( float x, float y, float z );

mat4 *GFX_get_modelview_matrix( void );

mat4 *GFX_get_projection_matrix( void );

mat4 *GFX_get_texture_matrix( void );

mat4 *GFX_get_modelview_projection_matrix( void );

mat3 *GFX_get_normal_matrix( void );

void GFX_ortho( float left, float right, float bottom, float top, float clip_start, float clip_end );

void GFX_set_orthographic_2d( float left, float right, float bottom, float top );

void GFX_set_orthographic( float screen_ratio, float scale, float aspect_ratio, float clip_start, float clip_end, float screen_orientation );

void GFX_set_perspective( float fovy, float aspect_ratio, float clip_start, float clip_end, float screen_orientation );

void GFX_look_at( vec3 *eye, vec3 *center, vec3 *up );

void GFX_clear_color( float r, float g, float b, float a );

int GFX_project( float objx, float objy, float objz, mat4 *modelview_matrix, mat4 *projection_matrix, int *viewport_matrix, float *winx, float *winy, float *winz );

int GFX_unproject( float winx, float winy, float winz, mat4 *modelview_matrix, mat4 *projection_matrix, int *viewport_matrix, float *objx, float *objy, float *objz );

#endif
