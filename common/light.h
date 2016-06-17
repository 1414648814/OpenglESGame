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


#ifndef LIGHT_H
#define LIGHT_H

/*!
	\file light.h
	
	\brief Function prototypes and definitions to use with the LIGHT structure.
*/


enum
{
	//! Directional light type identifier.
	LIGHT_DIRECTIONAL = 0,

	//! Point light type identifier.
	LIGHT_POINT = 1,

	//! Point light with attenuation type identifier.
	LIGHT_POINT_WITH_ATTENUATION = 2,
	
	//! Point sphere type identifier.	
	LIGHT_POINT_SPHERE = 3,
	
	//! Spot light type identifier.
	LIGHT_SPOT = 4
};


//! Structure to create a directionl, point, point with attenuation, point sphere or spot light.
typedef struct
{
	//! The internal name of the light.
	char	name[ MAX_CHAR ];

	//! The diffuse color of the light.
	vec4	color;

	//! Used to store the direction of the light in eye space (LIGHT_DIRECTIONAL only)
	vec3	direction;
	
	//! The position of the light in world coordinate.
	vec4	position;

	//! The linear attenuation value of the light. (Only in use when the light type is LIGHT_POINT_WITH_ATTENUATION)
	float	linear_attenuation;

	//! The quadratic attenuation value of the light. (Only in use when the light type is LIGHT_POINT_WITH_ATTENUATION)
	float	quadratic_attenuation;
	
	//! The light distance. It can either be used as radius (LIGHT_POINT_SPHERE) or as the distance factor (LIGHT_POINT_WITH_ATTENUATION).
	float	distance;

	//! The light field of view in degree. (LIGHT_SPOT only)
	float	spot_fov;

	//! The halved value of the spot_fov converted in (cos) radiant. (LIGHT_SPOT only)
	float	spot_cos_cutoff;

	//! The spot blending factor. (LIGHT_SPOT only)
	float	spot_blend;
	
	//! Used to store the direction of the light in object space (LIGHT_SPOT only)
	vec3	spot_direction;

	//! The type of light can be: LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_POINT_WITH_ATTENUATION, LIGHT_POINT_SPHERE or LIGHT_SPOT.
	unsigned char type;
	
} LIGHT;


LIGHT *LIGHT_create_directional( char *name, vec4 *color, float rotx, float roty, float rotz );

LIGHT *LIGHT_create_point( char *name, vec4 *color, vec3 *position );

LIGHT *LIGHT_create_point_with_attenuation( char *name, vec4 *color, vec3 *position, float distance, float linear_attenuation, float quadratic_attenuation );

LIGHT *LIGHT_create_point_sphere( char *name, vec4 *color, vec3 *position, float distance );

LIGHT *LIGHT_create_spot( char *name, vec4 *color, vec3 *position, float rotx, float roty, float rotz, float fov, float spot_blend );

void LIGHT_get_direction_in_object_space( LIGHT *light, mat4 *m, vec3 *direction );

void LIGHT_get_direction_in_eye_space( LIGHT *light, mat4 *m, vec3 *direction );

void LIGHT_get_position_in_eye_space( LIGHT *light, mat4 *m, vec4 *position );

LIGHT *LIGHT_free( LIGHT *light );

#endif
