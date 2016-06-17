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
	\file light.cpp
	
	\brief Contains multiple function to initialize different type of LIGHT.
    
	\details Helper functions allowing you to create different type of lights such as directional,
	point light, point light with attenuation, point sphere and spot light. Each create function
	find in this implementation will return you a LIGHT structure pointer that you can then
	use inside your application to provide realtime lighting functionalities.
*/


/*!
	Create a new directional light.
	
	\param[in] name The internal name to use for the new light.
	\param[in] color The RGBA color component to use as diffuse light color.
	\param[in] rotx The X rotation angle in degree of the light direction vector.
	\param[in] roty The Y rotation angle in degree of the light direction vector.
	\param[in] rotz The Z rotation angle in degree of the light direction vector.
	
	\return Return a new LIGHT structure pointer set to handle dynamic directional lighting.
*/
LIGHT *LIGHT_create_directional( char *name, vec4 *color, float rotx, float roty, float rotz )
{
	vec3 up_axis = { 0.0f, 0.0f, 1.0f };
	
	LIGHT *light = ( LIGHT * ) calloc( 1, sizeof( LIGHT ) );
	
	strcpy( light->name, name );

	memcpy( &light->color, color, sizeof( vec4 ) );

	light->type = 0;
	
	create_direction_vector( &light->direction, &up_axis, rotx, roty, rotz );	

	return light;
}


/*!
	Create a new point light.

	\param[in] name The internal name to use for the new light.
	\param[in] color The RGBA color component to use as diffuse light color.
	\param[in] position The XYZ position of the light in world space coordinates.
	
	\return Return a new LIGHT structure pointer set to handle omni directional dynamic lighting.
*/
LIGHT *LIGHT_create_point( char *name, vec4 *color, vec3 *position )
{
	LIGHT *light = ( LIGHT * ) calloc( 1, sizeof( LIGHT ) );
	
	strcpy( light->name, name );

	memcpy( &light->color, color, sizeof( vec4 ) );

	memcpy( &light->position, position, sizeof( vec3 ) );
	light->position.w = 1.0f;

	light->type = 1;

	return light;
}


/*!
	Basically the same as LIGHT_create_point at the exception that this type
	of light also include linear and quadratic attenuation factors based on
	the intensity of the light ray distance. 

	\param[in] name The internal name to use for the new light.
	\param[in] color The RGBA color component to use as diffuse light color.
	\param[in] position The XYZ position of the light in world space coordinates.
	\param[in] distance The light is at half of its intensity at this distance.
	\param[in] linear_attenuation The linear distance attenuation factor.
	\param[in] quadratic_attenuation The quadratic distance attenuation factor.

	\return Return a new LIGHT structure pointer of a point light with attenuation.
*/
LIGHT *LIGHT_create_point_with_attenuation( char *name, vec4 *color, vec3 *position, float distance, float linear_attenuation, float quadratic_attenuation )
{
	LIGHT *light = LIGHT_create_point( name, color, position );
		
	light->distance = distance * 2.0f;

	light->linear_attenuation = linear_attenuation;
	
	light->quadratic_attenuation = quadratic_attenuation;

	light->type = 2;

	return light;
}

/*!
	Also similar as point light, the type created by this function use the distance
	parameter to avoid processing fragment beyond the value received in parameter.

	\param[in] name The internal name to use for the new light.
	\param[in] color The RGBA color component to use as diffuse light color.
	\param[in] position The XYZ position of the light in world space coordinates.
	\param[in] distance Each fragment that have a distance greater than this value should be set to the scene ambient color.

	\return Return a new LIGHT structure pointer for a point light that only illuminate the fragment within the distance received in parameter.
*/
LIGHT *LIGHT_create_point_sphere( char *name, vec4 *color, vec3 *position, float distance )
{
	LIGHT *light = LIGHT_create_point( name, color, position );

	light->distance = distance;

	light->type = 3;
	
	return light;
}


/*!
	Create a new spot light.
	
	\param[in] name The internal name to use for the new light.
	\param[in] color The RGBA color component to use as diffuse light color.
	\param[in] position The XYZ position of the light in world space coordinates.
	\param[in] rotx The X rotation angle in degree of the light direction vector.
	\param[in] roty The Y rotation angle in degree of the light direction vector.
	\param[in] rotz The Z rotation angle in degree of the light direction vector.
	\param[in] fov The field of view of the spot in degree.
	\param[in] spot_blend A value in the range of 0 to 1 to control the blend factor of the area affected by the spot light.

	\return Return a new LIGHT structure pointer to use as a spot light in realtime.
*/
LIGHT *LIGHT_create_spot( char *name, vec4 *color, vec3 *position, float rotx, float roty, float rotz, float fov, float spot_blend ) {
	
	static vec3 up_axis = { 0.0f, 0.0f, 1.0f };

	LIGHT *light = ( LIGHT * ) calloc( 1, sizeof( LIGHT ) );

	strcpy( light->name, name );

	memcpy( &light->color, color, sizeof( vec4 ) );

	light->spot_fov = fov;

	light->spot_cos_cutoff = cosf( ( fov * 0.5f ) * DEG_TO_RAD );

	light->spot_blend = CLAMP( spot_blend, 0.001, 1.0f );

	memcpy( &light->position, position, sizeof( vec3 ) );
	light->position.w = 1.0f;

	light->type = 4;

	create_direction_vector( &light->spot_direction,
							 &up_axis,
							 rotx,
							 roty,
							 rotz );
	return light;
}


/*!
	Calculate the direction vector of a light based on an arbitrary matrix. The
	result will then be stored in the direction parameter received by the function.
	
	\param[in] light A valid LIGHT structure pointer.
	\param[in] m A valid 4x4 matrix pointer.
	\param[in,out] direction Variable to store the direction vector of the light in object space.
*/
void LIGHT_get_direction_in_object_space( LIGHT *light, mat4 *m, vec3 *direction )
{
	mat4 invert;
	
	mat4_copy_mat4( &invert, m );
	
	mat4_invert( &invert );
	
	vec3_multiply_mat4( direction,
						&light->spot_direction,
						m );
						
	vec3_normalize( direction,
					direction );
	
	vec3_invert( direction,
				 direction );
}


/*!
	Calculate the direction vector of a light in eye space. The
	result will then be stored in the direction parameter received by the function.
	
	\param[in] light A valid LIGHT structure pointer.
	\param[in] m A valid 4x4 matrix pointer.
	\param[in,out] direction Variable to store the direction vector of the light in eye space.
*/
void LIGHT_get_direction_in_eye_space( LIGHT *light, mat4 *m, vec3 *direction )
{
	vec3_multiply_mat4( direction,
						&light->direction,
						m );
		
	vec3_invert( direction, direction );
}


/*!
	Calculate the position of the light in eye space. The
	result will then be stored in the position parameter
	received by the function.
	
	\param[in] light A valid LIGHT structure pointer.
	\param[in] m A valid 4x4 matrix pointer.
	\param[in,out] position Variable to store the position of the light in eye space.
*/
void LIGHT_get_position_in_eye_space( LIGHT *light, mat4 *m, vec4 *position )
{
	vec4_multiply_mat4( position,
						&light->position,
						m );
}


/*!
	Free a LIGHT structure pointer previous initialized with one of the create function
	provided by this implementation.
	
	\param[in] light A valid LIGHT structure pointer.
	
	\return Return a NULL LIGHT structure pointer.
*/
LIGHT *LIGHT_free( LIGHT *light )
{
	free( light );
	return NULL;
}
