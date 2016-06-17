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
	\file vector.cpp
	
	\brief Contains various helper function implementation to deal with vec2, vec3 and vec4.
    
	\details You can find in this source file all the necessary function to manipulate 2D, 3D and 4D vectors.
	Most of theses functions are used internally by GFX, but you also have full access to them
	at anytime inside your own C/C++ source.
*/


/*!
	Add two vector 2D.

	\param[in,out] dst The result of the addition operation.
	\param[in] v0 The first vector 2D.
	\param[in] v1 The second vector 2D.
*/
void vec2_add( vec2 *dst, vec2 *v0, vec2 *v1 )
{
	dst->x = v0->x + v1->x;
	dst->y = v0->y + v1->y;
}

/*!
	Subtract two vector 2D.

	\param[in,out] dst The result of the subtract operation.
	\param[in] v0 The first vector 2D.
	\param[in] v1 The second vector 2D.
*/
void vec2_diff( vec2 *dst, vec2 *v0, vec2 *v1 )
{
	dst->x = v0->x - v1->x;
	dst->y = v0->y - v1->y;
}


/*!
	Add two vector 3D.

	\param[in,out] dst The result of the addition operation.
	\param[in] v0 The first vector 3D.
	\param[in] v1 The second vector 3D.
*/
void vec3_add( vec3 *dst, vec3 *v0, vec3 *v1 )
{
	dst->x = v0->x + v1->x;
	dst->y = v0->y + v1->y;
	dst->z = v0->z + v1->z;
}


/*!
	Subtract two vector 3D.

	\param[in,out] dst The result of the subtraction operation.
	\param[in] v0 The first vector 3D.
	\param[in] v1 The second vector 3D.
*/
void vec3_diff( vec3 *dst, vec3 *v0, vec3 *v1 )
{
	dst->x = v0->x - v1->x;
	dst->y = v0->y - v1->y;
	dst->z = v0->z - v1->z;
}


/*!
	Multiply two vector 3D.

	\param[in,out] dst The result of the multiplication operation.
	\param[in] v0 The first vector 3D.
	\param[in] v1 The second vector 3D.
*/
void vec3_mul( vec3 *dst, vec3 *v0, vec3 *v1 )
{
	dst->x = v0->x * v1->x;
	dst->y = v0->y * v1->y;
	dst->z = v0->z * v1->z;
}


/*!
	Calculate the dot product of a vector 3D.
	
	\param[in] v The vector 3D to calculate the dot product for.
	
	\return Return dot product of the vector 3D.
*/
float vec3_dot( vec3 *v )
{
	return ( v->x * v->x ) +
		   ( v->y * v->y ) +
		   ( v->z * v->z );
}


/*!
	Calculate the dot product between two vector 3D.
	
	\param[in] v0 The first vector 3D to use in the dot product operation.
	\param[in] v1 The second vector 3D to use in the dot product operation.
	
	\return Return the result of the dot product operation between the two vector 3D.
*/
float vec3_dot_vec3( vec3 *v0, vec3 *v1 )
{
	return ( v0->x * v1->x ) +
		   ( v0->y * v1->y ) +
		   ( v0->z * v1->z );
}


/*!
	Calculate the lenght of a vector 3D.
	
	\param[in] v The vector 3D to calculate the lenght for.

	\return Return the lenght of the vector 3D.
*/
float vec3_length( vec3 *v )
{ return sqrtf( vec3_dot( v ) ); }


/*!
	Helper function to normalize a vector 3D.
	
	\param[in,out] dst Variable to store the result of the normalization operation.
	\param[in] v The vector 3D to normalize.
	
	\return Return the lenght of the vector 3D.
*/
float vec3_normalize( vec3 *dst, vec3 *v )
{
	float l = vec3_length( v );
	
	if( l )
	{
		float m = 1.0f / l;

		dst->x = v->x * m;
		dst->y = v->y * m;
		dst->z = v->z * m;
	}
	
	return l;
}


/*!
	Calculate the cross product between two vector 3D.
	
	\param[in,out] dst This variable is used to store the result of the cross operation.
	\param[in] v0 The first vector 3D to use in the dot product operation.
	\param[in] v1 The second vector 3D to use in the dot product operation.

*/
void vec3_cross( vec3 *dst, vec3 *v0, vec3 *v1 )
{
	dst->x = ( v0->y * v1->z ) - ( v1->y * v0->z );
	dst->y = ( v0->z * v1->x ) - ( v1->z * v0->x );
	dst->z = ( v0->x * v1->y ) - ( v1->x * v0->y );
}


/*!
	Calculate the distance between two vertex.
	
	\param[in] v0 The first vertex 3D.
	\param[in] v1 The second vertex 3D.
	
	\return Return the distance between the two vertex 3D.
*/
float vec3_dist( vec3 *v0, vec3 *v1 )
{
	vec3 v;
	
	vec3_diff( &v, v0, v1 );
	
	return vec3_length( &v );
}


/*!
	Calculate the middle pointe between two vertex 3D.
	
	\param[in,out] dst Variable used to store the middle pointer between the two vertex.
	\param[in] v0 The first vertex 3D.
	\param[in] v1 The second vertex 3D.
*/
void vec3_mid( vec3 *dst, vec3 *v0, vec3 *v1 )
{
	dst->x = ( v0->x + v1->x ) * 0.5f;
	dst->y = ( v0->y + v1->y ) * 0.5f;
	dst->z = ( v0->z + v1->z ) * 0.5f;
}


/*!
	Inverse a vector 3D.

	\param[in,out] dst Variable used to store inverse vector 3D.
	\param[in] v The vector 3D to inverse.
*/
void vec3_invert( vec3 *dst, vec3 *v )
{
	dst->x = -v->x;
	dst->y = -v->y;
	dst->z = -v->z;
}


/*!
	Linearly interpolate between two vertex 3D.
	
	\param[in,out] dst Variable used to store final interpolated vertex 3D.
	\param[in] v0 The first vertex 3D.
	\param[in] v1 The second vertex 3D.
	\param[in] t Value ranging from the range of 0 to 1 to interpolate the XYZ value of the two vertex 3D.
*/
void vec3_lerp( vec3 *dst, vec3 *v0, vec3 *v1, float t )
{
	if( t == 1.0f )
	{
		dst->x = v1->x;
		dst->y = v1->y;
		dst->z = v1->z;
		
		return;
	}
	else if( t == 0.0f )
	{
		dst->x = v0->x;
		dst->y = v0->y;
		dst->z = v0->z;
	
		return;
	}

	dst->x = v0->x + t * ( v1->x - v0->x );
	dst->y = v0->y + t * ( v1->y - v0->y );
	dst->z = v0->z + t * ( v1->z - v0->z );
}


/*!
	Alternate method to linearly interpolate between two vertex 3D.
	
	\param[in,out] dst Variable used to store final interpolated vertex 3D.
	\param[in] v0 The first vertex 3D.
	\param[in] v1 The second vertex 3D.
	\param[in] t Value ranging from the range of 0 to 1 to interpolate the XYZ value of the two vertex 3D.
*/
void vec3_lerp2( vec3 *dst, vec3 *v0, vec3 *v1, float t )
{
	dst->x = v0->x * t + v1->x * ( 1.0f - t );
	dst->y = v0->y * t + v1->y * ( 1.0f - t );
	dst->z = v0->z * t + v1->z * ( 1.0f - t );
}


/*!
	Rotate a vertex 3D by a quaternion (vector 4D).
	
	\param[in,out] dst Variable used to store the result of the rotation.
	\param[in] v0 The vertex 3D.
	\param[in] v1 The quaternion to use to rotate the vertex.
*/
void vec3_rotate_vec4( vec3 *dst, vec3 *v0, vec4 *v1 )
{
	vec4 i, t, f;

	vec4_conjugate( &i, v1 );
	
	vec4_normalize( &i, &i );
	
	vec4_multiply_vec3( &t, v1, v0 );
	
	vec4_multiply_vec4( &f, &t, &i );
	
	memcpy( dst, &f, sizeof( vec3 ) );
}

/*!
	Helper that convert a vertex 3D to the Recast default coordinate system (up axis Y to up axis Z).

	\param[in,out] v The vertex 3D to convert.
*/
void vec3_to_recast( vec3 *v )
{
	vec3 tmp = {  v->x,
				  v->z,
			     -v->y };
	
	memcpy( v, &tmp, sizeof( vec3 ) );
}


/*!
	Helper function that convert a Recast based coordinate system vector 3D to an up axis Z coordinate system. 

	\param[in,out] v The vertex 3D to convert.	
*/
void recast_to_vec3( vec3 *v )
{
	vec3 tmp = {  v->x,
			     -v->z,
			      v->y };
	
	memcpy( v, &tmp, sizeof( vec3 ) );
}


/*!
	Quaternion addition.
	
	\param[in,out] dst The result of the addition operation.
	\param[in] v0 The first quaternion.
	\param[in] v1 The second quaternion.
*/
void vec4_add( vec4 *dst, vec4 *v0, vec4 *v1 )
{
	dst->x = v0->x + v1->x;
	dst->y = v0->y + v1->y;
	dst->z = v0->z + v1->z;
	dst->w = v0->w;// + v1->w;
}


/*!
	Quaternion subtraction.
	
	\param[in,out] dst The result of the subtract operation.
	\param[in] v0 The first quaternion.
	\param[in] v1 The second quaternion.
*/
void vec4_diff( vec4 *dst, vec4 *v0, vec4 *v1 )
{
	dst->x = v0->x - v1->x;
	dst->y = v0->y - v1->y;
	dst->z = v0->z - v1->z;
	dst->w = v0->w;// - v1->w;
}


/*!
	Helper function to re-calculate the W component of a quaternion.
	
	\param[in,out] v The quaternion to use in order to re-calculate the W component.
*/
void vec4_build_w( vec4 * v )
{
	float l = 1.0f - ( v->x * v->x ) -
					 ( v->y * v->y ) -
					 ( v->z * v->z );

	v->w = ( l < 0.0f ) ? 0.0f : -sqrtf( l );
}


/*!
	Calculate the dot product of a vector 4D.
	
	\param[in] v The vector 4D to calculate the dot product for.
	
	\return Return the dot product of the vector 4D.
*/
float vec4_dot( vec4 *v )
{
	return ( v->x * v->x ) + 
		   ( v->y * v->y ) + 
		   ( v->z * v->z ) + 
		   ( v->w * v->w );
}


/*!
	Calculate the dot product between two vector 4D.
	
	\param[in] v0 The first vector 4D to use in the dot product operation.
	\param[in] v1 The second vector 4D to use in the dot product operation.
	
	\return Return the result of the dot product operation between the two vector 4D.
*/
float vec4_dot_vec4( vec4 *v0, vec4 *v1 )
{
	return ( ( v0->x * v1->x ) +
			 ( v0->y * v1->y ) +
			 ( v0->z * v1->z ) + 
			 ( v0->w * v1->w ) );
}


/*!
	Calculate the lenght of a vector 4D.
	
	\param[in] v The vector 4D to calculate the lenght for.

	\return Return the lenght of the vector 4D.
*/
float vec4_length( vec4 *v )
{
	return sqrtf( vec4_dot( v ) );
}


/*!
	Helper function to normalize a vector 4D.
	
	\param[in,out] dst Variable to store the result of the normalization operation.
	\param[in] v The vector 4D to normalize.
	
	\return Return the lenght of the vector 4D.
*/
float vec4_normalize( vec4 *dst, vec4 *v )
{
	float l = vec4_length( v ),
		  m = l ? 1.0f / l: 0.0f;

	dst->x = v->x * m;
	dst->y = v->y * m;
	dst->z = v->z * m;
	dst->w = v->w * m;
	
	return l;
}


/*!
	Multiply a quaternion by a vector 3D.
	
	\param[in,out] dst The result of the multiplication operation.
	\param[in] v0 The quaternion to use for the multiplication.
	\param[in] v1 The vector 3D to use for the multiplication.
*/
void vec4_multiply_vec3( vec4 *dst, vec4 *v0, vec3 *v1 )
{
	vec4 v;

	v.x =  ( v0->w * v1->x ) + ( v0->y * v1->z ) - ( v0->z * v1->y );
	v.y =  ( v0->w * v1->y ) + ( v0->z * v1->x ) - ( v0->x * v1->z );
	v.z =  ( v0->w * v1->z ) + ( v0->x * v1->y ) - ( v0->y * v1->x );
	v.w = -( v0->x * v1->x ) - ( v0->y * v1->y ) - ( v0->z * v1->z );

	memcpy( dst, &v, sizeof( vec4 ) );	
}


/*!
	Multiply two quaternions.
	
	\param[in,out] dst The result of the multiplication operation.
	\param[in] v0 The first quaternion to use for the multiplication.
	\param[in] v1 The second quaternion to use for the multiplication.
*/
void vec4_multiply_vec4( vec4 *dst, vec4 *v0, vec4 *v1 )
{
	vec4 v;

	v.x = ( v0->x * v1->w ) + ( v0->w * v1->x ) + ( v0->y * v1->z ) - ( v0->z * v1->y );
	v.y = ( v0->y * v1->w ) + ( v0->w * v1->y ) + ( v0->z * v1->x ) - ( v0->x * v1->z );
	v.z = ( v0->z * v1->w ) + ( v0->w * v1->z ) + ( v0->x * v1->y ) - ( v0->y * v1->x );
	v.w = ( v0->w * v1->w ) - ( v0->x * v1->x ) - ( v0->y * v1->y ) - ( v0->z * v1->z );

	memcpy( dst, &v, sizeof( vec4 ) );
}


/*!
	Conjugate a quaterion.
	
	\param[in,out] dst The result conjugate operation.
	\param[in] v The quaternion to conjugate.	
*/
void vec4_conjugate( vec4 *dst, vec4 *v )
{
	dst->x = -v->x;
	dst->y = -v->y;
	dst->z = -v->z;
	dst->w =  v->w;
}


/*!
	Inverse a quaternion.

	\param[in,out] dst Variable used to store the inverse quaternion.
	\param[in] v The quaternion to inverse.
*/
void vec4_invert( vec4 *dst, vec4 *v )
{
	dst->x = -v->x;
	dst->y = -v->y;
	dst->z = -v->z;
	dst->w = -v->w;
}


/*!
	Linearly interpolate between two quaternions.
	
	\param[in,out] dst Variable used to store final interpolated quaternion.
	\param[in] v0 The first quaternion.
	\param[in] v1 The second quaternion.
	\param[in] t Value ranging from the range of 0 to 1 to interpolate the XYZ value of the two quaternion.
*/
void vec4_lerp( vec4 *dst, vec4 *v0, vec4 *v1, float t )
{
	float dot = vec4_dot_vec4( v0, v1 ),
		  k0,
		  k1;

	vec4 tmp = { v1->x,
				 v1->y,
				 v1->z,
				 v1->w };
	
	if( t == 1.0f )
	{
		dst->x = v1->x;
		dst->y = v1->y;
		dst->z = v1->z;
		dst->w = v1->w;
		
		return;
	}
	else if( t == 0.0f )
	{
		dst->x = v0->x;
		dst->y = v0->y;
		dst->z = v0->z;
		dst->w = v0->w;
	
		return;
	}
	
	
	if( dot < 0.0f )
	{
		tmp.x = -tmp.x;
		tmp.y = -tmp.y;
		tmp.z = -tmp.z;
		tmp.w = -tmp.w;
		
		dot = -dot;
	}


	if( dot > 0.999999f )
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float s  = sqrtf( 1.0f - ( dot * dot ) ),
			  o  = atan2f( s, dot ),
			  o1 = 1.0f / s;

		k0 = sinf( ( 1.0f - t ) * o ) * o1;
		k1 = sinf( t * o ) * o1;
	}
	
	dst->x = ( k0 * v0->x ) + ( k1 * tmp.x );
	dst->y = ( k0 * v0->y ) + ( k1 * tmp.y );
	dst->z = ( k0 * v0->z ) + ( k1 * tmp.z );		
	dst->w = ( k0 * v0->w ) + ( k1 * tmp.w );
}


/*!
	Spherically interpolate between two quaternions.
	
	\param[in,out] dst Variable used to store final interpolated quaternion.
	\param[in] v0 The first quaternion.
	\param[in] v1 The second quaternion.
	\param[in] t Value ranging from the range of 0 to 1 to interpolate the XYZ value of the two quaternion.
*/
void vec4_slerp( vec4 *dst, vec4 *v0, vec4 *v1, float t )
{
	float c = vec4_dot_vec4( v0, v1 ),
		  k0,
		  k1;

	vec4 tmp = { v1->x,
				 v1->y,
				 v1->z,
				 v1->w };
	
	if( t == 1.0f )
	{
		dst->x = v1->x;
		dst->y = v1->y;
		dst->z = v1->z;
		dst->w = v1->w;
		
		return;
	}
	else if( t == 0.0f )
	{
		dst->x = v0->x;
		dst->y = v0->y;
		dst->z = v0->z;
		dst->w = v0->w;
	
		return;
	}	
	
	
	if( c < 0.0f )
	{
		tmp.x = -tmp.x;
		tmp.y = -tmp.y;
		tmp.z = -tmp.z;
		tmp.w = -tmp.w;
		
		c = -c;
	}


	if( c > 0.999999f )
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float s  = sqrtf( 1.0f - ( c * c ) ),
			  o  = atan2f( s, c ),
			  o1 = 1.0f / s;

		k0 = sinf( ( 1.0f - t ) * o ) * o1;
		k1 = sinf( t * o ) * o1;
	}
	
	dst->x = ( k0 * v0->x ) + ( k1 * tmp.x );
	dst->y = ( k0 * v0->y ) + ( k1 * tmp.y );
	dst->z = ( k0 * v0->z ) + ( k1 * tmp.z );		
	dst->w = ( k0 * v0->w ) + ( k1 * tmp.w );
}
