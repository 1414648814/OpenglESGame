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
	\file gfx.cpp
	
	\brief The main implementation file of the GFX 3d engine.
    
	\details This file is the main implementation file of the GFX framework. It contains
	initialization methods, along with helpers and the necessary functionalities to mimic
	the way standard OpenGL is handling the matrix stack of the modelview, projection and 
	texture matrix. In addition, this file also implement some standar GLU functions such 
	as gluPerspective, gluLookAt, gluProject and Unproject for your convenience, along 
	with other GFX specific functions.
*/

//! Global GFX pointer.
GFX gfx;


#ifndef __IPHONE_4_0

	//! Function pointer for glBindVertexArrayOES on Android.
	PFNGLBINDVERTEXARRAYOESPROC		glBindVertexArrayOES;

	//! Function pointer for glGenVertexArraysOES on Android.
	PFNGLGENVERTEXARRAYSOESPROC		glGenVertexArraysOES;

	//! Function pointer for glDeleteVertexArraysOES on Android.
	PFNGLDELETEVERTEXARRAYSOESPROC	glDeleteVertexArraysOES;
#endif


/*!
	Initialize the global GFX structure and set all the default OpenGLES machine
	states to their default values as expected by the engine. In addition the matrix
	stacks are created and the first index of the model view, projection and texture
	matrix are set to identity.
*/
void GFX_start( void )
{
	memset( &gfx, 0, sizeof( GFX ) );
	
	#ifdef __IPHONE_4_0
	
		printf("\nGL_VENDOR:      %s\n", ( char * )glGetString( GL_VENDOR     ) );
		printf("GL_RENDERER:    %s\n"  , ( char * )glGetString( GL_RENDERER   ) );
		printf("GL_VERSION:     %s\n"  , ( char * )glGetString( GL_VERSION    ) );
		printf("GL_EXTENSIONS:  %s\n"  , ( char * )glGetString( GL_EXTENSIONS ) );
	#else
	
		__android_log_print( ANDROID_LOG_INFO, "", "\nGL_VENDOR:      %s\n", ( char * )glGetString( GL_VENDOR     ) );
		__android_log_print( ANDROID_LOG_INFO, "", "GL_RENDERER:    %s\n"  , ( char * )glGetString( GL_RENDERER   ) );
		__android_log_print( ANDROID_LOG_INFO, "", "GL_VERSION:     %s\n"  , ( char * )glGetString( GL_VERSION    ) );
		__android_log_print( ANDROID_LOG_INFO, "", "GL_EXTENSIONS:  %s\n"  , ( char * )glGetString( GL_EXTENSIONS ) );
	#endif

	glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
	
	glHint( GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES, GL_NICEST );
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE  );
	glDisable( GL_DITHER );
	glDepthMask( GL_TRUE );
	glDepthFunc( GL_LESS );
	glDepthRangef( 0.0f, 1.0f );
	glClearDepthf( 1.0f );
	glCullFace ( GL_BACK );
	glFrontFace( GL_CCW  );
	glClearStencil( 0 );
	glStencilMask( 0xFFFFFFFF );
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	#ifndef __IPHONE_4_0

		glBindVertexArrayOES 	= ( PFNGLBINDVERTEXARRAYOESPROC    ) eglGetProcAddress("glBindVertexArrayOES"  );
		glGenVertexArraysOES 	= ( PFNGLGENVERTEXARRAYSOESPROC    ) eglGetProcAddress("glGenVertexArraysOES"  );
		glDeleteVertexArraysOES = ( PFNGLDELETEVERTEXARRAYSOESPROC ) eglGetProcAddress("glDeleteVertexArraysOES");
	#endif
	
	GFX_set_matrix_mode( TEXTURE_MATRIX );
	GFX_load_identity();

	GFX_set_matrix_mode( PROJECTION_MATRIX );
	GFX_load_identity();

	GFX_set_matrix_mode( MODELVIEW_MATRIX );
	GFX_load_identity();

	GFX_error();
}


/*!
	Helper function check if the current GLES driver report an error. In case an error
	is generated it will printed on the Console (XCode) or LogChat (Eclispe).
*/
void GFX_error( void )
{
	unsigned int error;

	while( ( error = glGetError() ) != GL_NO_ERROR )
	{
		char str[ MAX_CHAR ] = {""};

		switch( error )
		{
			case GL_INVALID_ENUM:
			{
				strcpy( str, "GL_INVALID_ENUM" );
				break;
			}

			case GL_INVALID_VALUE:
			{
				strcpy( str, "GL_INVALID_VALUE" );
				break;
			}

			case GL_INVALID_OPERATION:
			{
				strcpy( str, "GL_INVALID_OPERATION" );
				break;
			}

			case GL_OUT_OF_MEMORY:
			{
				strcpy( str, "GL_OUT_OF_MEMORY" );
				break;
			}
		}
		
		console_print( "[ GL_ERROR ]\nERROR: %s\n", str );
	}
}


/*!
	Set the current matrix mode that you want to work with. Only the MODELVIEW_MATRIX,
	PROJECTION_MATRIX and TEXTURE_MATRIX are supported. Take note that contrairly to the default OpenGL behavior
	the TEXTURE_MATRIX is not implemented on a per channel basis, but on a global basis) contain in gfx.h.
	
	If you wish to tweak the default matrix stack depth you can modify the following values: MAX_MODELVIEW_MATRIX,
	MAX_PROJECTION_MATRIX, MAX_TEXTURE_MATRIX. 
	
	\param[in] mode The matrix mode that will be use as target.
*/
void GFX_set_matrix_mode( unsigned int mode )
{ gfx.matrix_mode = mode; }


/*!
	Set the current matrix set as target by the GFX_set_matrix_mode to the
	identity matrix.
*/
void GFX_load_identity( void )
{
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_identity( GFX_get_modelview_matrix() );
			
			break;
		}
		
		case PROJECTION_MATRIX:
		{
			mat4_identity( GFX_get_projection_matrix() );
			
			break;
		}
		
		case TEXTURE_MATRIX:
		{
			mat4_identity( GFX_get_texture_matrix() );
			
			break;
		}		
	}
}


/*!
	Push the current matrix stack set as target by the GFX_set_matrix_mode function.
*/
void GFX_push_matrix( void )
{
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_copy_mat4( &gfx.modelview_matrix[ gfx.modelview_matrix_index + 1 ],
							&gfx.modelview_matrix[ gfx.modelview_matrix_index	  ] );
		
			++gfx.modelview_matrix_index;
			
			break;
		}

		case PROJECTION_MATRIX:
		{
			mat4_copy_mat4( &gfx.projection_matrix[ gfx.projection_matrix_index + 1 ],
							&gfx.projection_matrix[ gfx.projection_matrix_index	    ] );
			
			++gfx.projection_matrix_index;
			
			break;
		}
		
		case TEXTURE_MATRIX:
		{
			mat4_copy_mat4( &gfx.texture_matrix[ gfx.texture_matrix_index + 1 ],
							&gfx.texture_matrix[ gfx.texture_matrix_index     ] );
			
			++gfx.texture_matrix_index;
			
			break;
		}		
	}
}


/*!
	Pop the current matrix stack set as target by the GFX_set_matrix_mode function.
*/

void GFX_pop_matrix( void )
{
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			--gfx.modelview_matrix_index;
			
			break;
		}
			
		case PROJECTION_MATRIX:
		{
			--gfx.projection_matrix_index;
			
			break;
		}
		
		case TEXTURE_MATRIX:
		{
			--gfx.texture_matrix_index;
			
			break;
		}		
	}
}


/*!
	Assign a 4 by 4 matrix to the current matrix on top of the stack for the current
	mode set as target by the GFX_set_matrix_mode function.
	
	\param[in] m A valid 4x4 matrix pointer.
*/
void GFX_load_matrix( mat4 *m )
{
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_copy_mat4( GFX_get_modelview_matrix(), m );
			
			break;
		}
	
		case PROJECTION_MATRIX:
		{
			mat4_copy_mat4( GFX_get_projection_matrix(), m );
			
			break;
		}
		
		case TEXTURE_MATRIX:
		{
			mat4_copy_mat4( GFX_get_texture_matrix(), m );
			
			break;
		}		
	}
}


/*!
	Multiply a 4 by 4 matrix to the current matrix on top of the stack for the current
	mode set as target by the GFX_set_matrix_mode function.

	\param[in] m A valid 4x4 matrix pointer.
*/
void GFX_multiply_matrix( mat4 *m )
{
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_multiply_mat4( GFX_get_modelview_matrix(), GFX_get_modelview_matrix(), m );

			break;
		}
			
		case PROJECTION_MATRIX:
		{
			mat4_multiply_mat4( GFX_get_projection_matrix(), GFX_get_projection_matrix(), m );
			
			break;
		}
		
		case TEXTURE_MATRIX:
		{
			mat4_multiply_mat4( GFX_get_texture_matrix(), GFX_get_texture_matrix(), m );
			
			break;
		}		
	}
}


/*!
	Translate the current matrix pointed for the current mode set
	as target by the GFX_set_matrix_mode function.
	
	\param[in] x Specify the x coordinate of a translation vector.
	\param[in] y Specify the y coordinate of a translation vector.
	\param[in] z Specify the z coordinate of a translation vector.
	
	\sa http://www.opengl.org/sdk/docs/man/xhtml/glTranslate.xml
*/
void GFX_translate( float x, float y, float z )
{
	vec3 v = { x, y, z };
	
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_translate( GFX_get_modelview_matrix(), GFX_get_modelview_matrix(), &v );
			
			break;
		}
	
		case PROJECTION_MATRIX:
		{
			mat4_translate( GFX_get_projection_matrix(), GFX_get_projection_matrix(), &v );
			
			break;
		}

		case TEXTURE_MATRIX:
		{
			mat4_translate( GFX_get_texture_matrix(), GFX_get_texture_matrix(), &v );
			
			break;
		}		
	}
}


/*!
	Rotate the current matrix pointed for the current mode set
	as target by the GFX_set_matrix_mode function.

	\param[in] angle Specifies the angle of rotation, in degrees.
	\param[in] x Specify the x coordinate of a vector.
	\param[in] y Specify the y coordinate of a vector.
	\param[in] z Specify the z coordinate of a vector.
	
	\sa http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
*/
void GFX_rotate( float angle, float x, float y, float z )
{
	if( !angle ) return;
	
	vec4 v = { x, y, z, angle };
	
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_rotate( GFX_get_modelview_matrix(), GFX_get_modelview_matrix(), &v );
			
			break;
		}
	
		case PROJECTION_MATRIX:
		{
			mat4_rotate( GFX_get_projection_matrix(), GFX_get_projection_matrix(), &v );
			
			break;
		}

		case TEXTURE_MATRIX:
		{
			mat4_rotate( GFX_get_texture_matrix(), GFX_get_texture_matrix(), &v );
			
			break;
		}		
	}
}


/*!
	Scale the current matrix pointed for the current mode set
	as target by the GFX_set_matrix_mode function.
	
	\param[in] x Specify scale factor along the x axis.
	\param[in] y Specify scale factor along the y axis.
	\param[in] z Specify scale factor along the z axis.	
	
	\sa http://www.opengl.org/sdk/docs/man/xhtml/glScale.xml
*/
void GFX_scale( float x, float y, float z )
{
	static vec3 scale = { 1.0f, 1.0f, 1.0f };
	
	vec3 v = { x, y, z };
	
	if( !memcmp( &v, &scale, sizeof( vec3 ) ) ) return;
	
	
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_scale( GFX_get_modelview_matrix(), GFX_get_modelview_matrix(), &v );
			
			break;
		}
	
		case PROJECTION_MATRIX:
		{
			mat4_scale( GFX_get_projection_matrix(), GFX_get_projection_matrix(), &v );
			
			break;
		}

		case TEXTURE_MATRIX:
		{
			mat4_scale( GFX_get_texture_matrix(), GFX_get_texture_matrix(), &v );
			
			break;
		}		
	}
}


/*!
	Return the modelview matrix pointer on the top of the modelview matrix stack.
	
	\return Return a 4x4 matrix pointer of the top most modelview matrix.
*/
mat4 *GFX_get_modelview_matrix( void )
{ return &gfx.modelview_matrix[ gfx.modelview_matrix_index ]; }


/*!
	Return the projection matrix pointer on the top of the projection matrix stack.
	
	\return  Return a 4x4 matrix pointer of the top most projection matrix index.
*/
mat4 *GFX_get_projection_matrix( void )
{ return &gfx.projection_matrix[ gfx.projection_matrix_index ]; }


/*!
	Return the texture matrix pointer on the top of the texture matrix stack.
	
	\return  Return a 4x4 matrix pointer of the top most texture matrix index.
*/
mat4 *GFX_get_texture_matrix( void )
{ return &gfx.texture_matrix[ gfx.texture_matrix_index ]; }


/*!
	Return the result of the of the top most modelview matrix multiplied by the top
	most projection matrix.
	
	\return Return the 4x4 matrix pointer of the projection matrix index.	
*/
mat4 *GFX_get_modelview_projection_matrix( void )
{
	mat4_multiply_mat4( &gfx.modelview_projection_matrix, 
						GFX_get_projection_matrix(),
						GFX_get_modelview_matrix() );
	
	return &gfx.modelview_projection_matrix; 
}


/*!
	Return the result of the inverse and transposed operation of the top most modelview matrix applied
	on the rotation part of the matrix.
	
	\return Return the 3x3 matrix pointer that represent the invert and transpose
	result of the top most model view matrix.
*/
mat3 *GFX_get_normal_matrix( void )
{
	mat4 mat;
	
	mat4_copy_mat4( &mat, GFX_get_modelview_matrix() );

	mat4_invert_full( &mat );

	mat4_transpose( &mat );
	
	mat3_copy_mat4( &gfx.normal_matrix, &mat );

	return &gfx.normal_matrix;
}


/*!
	Multiply the current matrix with an orthographic matrix.
	
	\param[in] left Specify the coordinates for the left vertical clipping planes.
	\param[in] right Specify the coordinates for the right vertical clipping planes.
	\param[in] bottom Specify the coordinates for the bottom horizontal clipping planes.
	\param[in] top Specify the coordinates for the top horizontal clipping planes.
	\param[in] clip_start Specify the distance to the nearer depth clipping planes. This value is negative if the plane is to be behind the viewer.	
	\param[in] clip_end Specify the distance to the farther depth clipping planes. This value is negative if the plane is to be behind the viewer.	

	\sa http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
*/
void GFX_ortho( float left, float right, float bottom, float top, float clip_start, float clip_end )
{
	switch( gfx.matrix_mode )
	{
		case MODELVIEW_MATRIX:
		{
			mat4_ortho( GFX_get_modelview_matrix(), left, right, bottom, top, clip_start, clip_end ); 
			
			break;
		}
	
		case PROJECTION_MATRIX:
		{
			mat4_ortho( GFX_get_projection_matrix(), left, right, bottom, top, clip_start, clip_end );
			
			break;
		}

		case TEXTURE_MATRIX:
		{
			mat4_ortho( GFX_get_texture_matrix(), left, right, bottom, top, clip_start, clip_end );
			
			break;
		}		
	}
}


/*!
	Define a 2D orthographic projection matrix.
	
	\param[in] left Specify the coordinates for the left vertical clipping planes.
	\param[in] right Specify the coordinates for the right vertical clipping planes.
	\param[in] bottom Specify the coordinates for the bottom horizontal clipping planes.
	\param[in] top Specify the coordinates for the top horizontal clipping planes.
	
	\sa http://www.opengl.org/sdk/docs/man/xhtml/gluOrtho2D.xml
*/
void GFX_set_orthographic_2d( float left, float right, float bottom, float top )
{ GFX_ortho( left, right, bottom, top, -1.0f, 1.0f ); }


/*!
	Multiply the current matrix with a scaled orthographic matrix that respect the current
	screen and and aspect ratio.  
	
	\param[in] screen_ratio Specifies the screen ratio that determines the field of view in the Y direction. The screen ratio is the ratio of y (height) to y (width).
	\param[in] scale Determine that scale value that should be applied to the matrix.
	\param[in] aspect_ratio Specifies the aspect ratio that determines the field of view in the X direction. The screen ratio is the ratio of x (width) to y (height).
	\param[in] clip_start Specify the distance to the nearer depth clipping planes. This value is negative if the plane is to be behind the viewer.	
	\param[in] clip_end Specify the distance to the farther depth clipping planes. This value is negative if the plane is to be behind the viewer.	
	\param[in] screen_orientation A value in degree that rotate the matrix. Should be synchronized with your device orientation.
*/
void GFX_set_orthographic( float screen_ratio, float scale, float aspect_ratio, float clip_start, float clip_end, float screen_orientation )
{
	scale = ( scale * 0.5f ) * aspect_ratio;

	GFX_ortho( -1.0f, 
			    1.0f, 
			   -screen_ratio,
			    screen_ratio, 
			    clip_start,
				clip_end );
	
	GFX_scale( 1.0f / scale, 1.0f / scale, 1.0f );
		
	if( screen_orientation ) GFX_rotate( screen_orientation, 0.0f, 0.0f, 1.0f );
}


/*!
	Set up a perspective projection matrix.
	
	\param[in] fovy Specifies the field of view angle, in degrees, in the y direction.
	\param[in] aspect_ratio Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
	\param[in] clip_start Specifies the distance from the viewer to the near clipping plane (always positive).
	\param[in] clip_end Specifies the distance from the viewer to the far clipping plane (always positive).
	\param[in] screen_orientation A value in degree that rotate the matrix. Should be synchronized with your device orientation.

	\sa http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
*/
void GFX_set_perspective( float fovy, float aspect_ratio, float clip_start, float clip_end, float screen_orientation )
{
	mat4 mat;
	
	float d = clip_end - clip_start,
		  r = ( fovy * 0.5f ) * DEG_TO_RAD,
		  s = sinf( r ),
		  c = cosf( r ) / s;

	mat4_identity( &mat );
	
    mat.m[ 0 ].x = c / aspect_ratio;
    mat.m[ 1 ].y = c;
    mat.m[ 2 ].z = -( clip_end + clip_start ) / d;
    mat.m[ 2 ].w = -1.0f;
    mat.m[ 3 ].z = -2.0f * ( clip_start * clip_end ) / d;
    mat.m[ 3 ].w =  0.0f;
	
	GFX_multiply_matrix( &mat );
	
	if( screen_orientation ) GFX_rotate( screen_orientation, 0.0f, 0.0f, 1.0f );
}


/*!
	Define a viewing transformation.
	
	\param[in] eye Specifies the position of the eye point.
	\param[in] center Specifies the position of the reference point.
	\param[in] up Specifies the direction of the up vector.
	
	\sa http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
*/
void GFX_look_at( vec3 *eye, vec3 *center, vec3 *up )
{
	vec3 f,
		 s,
		 u;

	mat4 mat;

	mat4_identity( &mat );

	vec3_diff( &f, center, eye );

	vec3_normalize( &f, &f );

	vec3_cross( &s, &f, up );

	vec3_normalize( &s, &s );

	vec3_cross( &u, &s, &f );

	mat.m[ 0 ].x = s.x;
	mat.m[ 1 ].x = s.y;
	mat.m[ 2 ].x = s.z;

	mat.m[ 0 ].y = u.x;
	mat.m[ 1 ].y = u.y;
	mat.m[ 2 ].y = u.z;

	mat.m[ 0 ].z = -f.x;
	mat.m[ 1 ].z = -f.y;
	mat.m[ 2 ].z = -f.z;

	GFX_multiply_matrix( &mat );

	GFX_translate( -eye->x, -eye->y, -eye->z );
}


/*!
	Map object coordinates to window coordinates.
	
	\param[in] objx Specify the object X coordinate.
	\param[in] objy Specify the object Y coordinate.
	\param[in] objz Specify the object Z coordinate.
	\param[in] modelview_matrix Specifies the current modelview matrix.
	\param[in] projection_matrix Specifies the current projection matrix.
	\param[in] viewport_matrix Specifies the current viewport matrix.
	\param[in] winx Return the computed X window coordinate.
	\param[in] winy Return the computed Y window coordinate.
	\param[in] winz Return the computed Z window coordinate.

	\sa http://www.opengl.org/sdk/docs/man/xhtml/gluProject.xml
*/
int GFX_project( float objx, float objy, float objz, mat4 *modelview_matrix, mat4 *projection_matrix, int *viewport_matrix, float *winx, float *winy, float *winz )
{
	vec4 vin,
		 vout;
		 
	vin.x = objx;
	vin.y = objy;
	vin.z = objz;
	vin.w = 1.0f;
	
	vec4_multiply_mat4( &vout, &vin, modelview_matrix );

	vec4_multiply_mat4( &vin, &vout, projection_matrix );

	if( !vin.w ) return 0;
		
	vin.x /= vin.w;
	vin.y /= vin.w;
	vin.z /= vin.w;
	
	vin.x = vin.x * 0.5f + 0.5f;
	vin.y = vin.y * 0.5f + 0.5f;
	vin.z = vin.z * 0.5f + 0.5f;

	vin.x = vin.x * viewport_matrix[ 2 ] + viewport_matrix[ 0 ];
	vin.y = vin.y * viewport_matrix[ 3 ] + viewport_matrix[ 1 ];

	*winx = vin.x;
	*winy = vin.y;
	*winz = vin.z;

	return 1;
}


/*!
	Map window coordinates to object coordinates.

	\param[in] winx Specify the window X coordinate.
	\param[in] winy Specify the window Y coordinate.
	\param[in] winz Specify the window Z coordinate.
	\param[in] modelview_matrix Specifies the current modelview matrix.
	\param[in] projection_matrix Specifies the current projection matrix.
	\param[in] viewport_matrix Specifies the current viewport matrix.
	\param[in] objx Return the computed X object coordinate. 
	\param[in] objy Return the computed Y object coordinate.
	\param[in] objz Return the computed Z object coordinate.
	
	\sa http://www.opengl.org/sdk/docs/man/xhtml/gluUnProject.xml
*/
int GFX_unproject( float winx, float winy, float winz, mat4 *modelview_matrix, mat4 *projection_matrix, int *viewport_matrix, float *objx, float *objy, float *objz )
{
	mat4 final;

	vec4 vin,
		 vout;

	mat4_multiply_mat4( &final,
						projection_matrix,
						modelview_matrix );

	mat4_invert_full( &final );

	vin.x = winx;
	vin.y = winy;
	vin.z = winz;
	vin.w = 1.0f;

	vin.x = ( vin.x - viewport_matrix[ 0 ] ) / viewport_matrix[ 2 ];
	vin.y = ( vin.y - viewport_matrix[ 1 ] ) / viewport_matrix[ 3 ];

	vin.x = vin.x * 2.0f - 1.0f;
	vin.y = vin.y * 2.0f - 1.0f;
	vin.z = vin.z * 2.0f - 1.0f;

	vec4_multiply_mat4( &vout, &vin, &final );
	
	if( !vout.w ) return 0;
	
	vout.x /= vout.w;
	vout.y /= vout.w;
	vout.z /= vout.w;
	
	*objx = vout.x;
	*objy = vout.y;
	*objz = vout.z;
	
	return 1;
}
