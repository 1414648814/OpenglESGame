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

#ifndef MD5_H
#define MD5_H


/*!
	\file md5.h
	
	\brief Function prototypes and definitions to use with the MD5 structure.
*/

enum
{
	//! Interpolate frame by frame.
	MD5_METHOD_FRAME = 0,
	
	//! Use linear interpolation for bone rotations.
	MD5_METHOD_LERP  = 1,
	
	//! Use spherical interpolation for bone rotations.
	MD5_METHOD_SLERP = 2
};


//! Structure definition of a single joint.
typedef struct
{
	//! The internal name of the joint.
	char	name[ MAX_CHAR ];
	
	//! The joint parent id in the skeleton MD5JOINT array.
	int		parent;
	
	//! Location of the joint.
	vec3	location;
	
	//! Rotation quaternion of the joint.
	vec4	rotation;
  
} MD5JOINT;

//! Structure definition of a single vertex.
typedef struct
{
	//! UV coordinate for the current vertex.
	vec2			uv;
	
	//! The normal for the current vertex.
	vec3			normal;
	
	//! The tangent for the current vertex.
	vec3			tangent;
	
	//! The start weight value.
	unsigned int	start;
	
	//! How many weights is affecting the vertex.
	unsigned int	count;

} MD5VERTEX;


//! Structure to represent triangle indices.
typedef struct
{
	//! Triangle indices.
	unsigned short indice[ 3 ];

} MD5TRIANGLE;


//! Structure to hold a bone weight information.
typedef struct
{
	//! The joint id for this weight.
	int		joint;

	//! The bias value for this weight.
	float	bias;

	vec3	location;
	
	//! The weighted normal for the current vertex.
	vec3	normal;
	
	//! The weighted tangent for the current vertex.
	vec3	tangent;
	
} MD5WEIGHT;


//! Structure that allow you to draw a mesh from an MD5.
typedef struct
{
	//! The shader name to use for this mesh.
	char			shader[ MAX_CHAR ];
	
	//! The number of vertex the mesh contains.
	unsigned int	n_vertex;

	//! Array of MD5VERTEX which is the size of n_vertex.
	MD5VERTEX		*md5vertex;

	//! The VBO id maintained by OpenGLES.
	unsigned int	vbo;

	//! The size of the vertex data array in bytes.
	unsigned int	size;

	//! The stride size in bytes between each vertex data elements.
	unsigned int	stride;

	//! The VBO buffer offsets.
	unsigned int	offset[ 4 ];

	//! The vertex data array. (POSITION, NORMAL, UV, TANGENT)
	unsigned char	*vertex_data;	
			
	//! The number of triangles.
	unsigned int	n_triangle;

	//! Array of MD5TRIANGLE that form the current MD5MESH.
	MD5TRIANGLE		*md5triangle;

	//! The drawing mode to use. By default GL_TRIANGLES, and if optimized GL_TRIANGLE_STRIPS.
	unsigned int	mode;

	//! The number of indices required to draw the MD5MESH.
	unsigned short	n_indice;
	
	//! Array of indices.
	unsigned short  *indice;

	//! The VBO id for the indices.
	unsigned int	vbo_indice;

	//! The number of weight.
	unsigned int	n_weight;

	//! Array of MD5WEIGHT.
	MD5WEIGHT		*md5weight;
	
	//! The VAO id maintained by OpenGLES.
	unsigned int	vao;
	
	//! Determine if the mesh if visible (1) or invisile (0). Default value 1.
	unsigned char	visible;
	
	//! The OBJMATERIAL to use to draw the MD5MESH.
	OBJMATERIAL		*objmaterial;
	
} MD5MESH;


//! Structure that allows you to load and manipulate an MD5 action.
typedef struct
{
	//! The internal name for the action.
	char			name[ MAX_CHAR ];
	
	//! The number of frame this action contains.
	unsigned int	n_frame;
	
	//! Array of MD5JOINT for each frame of the action.
	MD5JOINT		**frame;
	
	//! The current pose for the current frame.
	MD5JOINT		*pose;
	
	//! Current frame index.
	int				curr_frame;
	
	//! Next frame index.
	int				next_frame;
	
	//! The current state of the action, either PLAY, PAUSE or STOP.
	unsigned char	state;
	
	//! The interpolation method to use when switching frames.
	unsigned char	method;
	
	//! Determine if the MD5ACTION should loop back to the first frame when the last frame is reached.
	unsigned char	loop;
	
	//! The current frame time.
	float			frame_time;
	
	//! The action frame per second.
	float			fps;

} MD5ACTION;


//! The main MD5 structure that allow you to load and manipulate .md5mesh and .md5anim files.
typedef struct
{
	//! The internal name to use for the MD5.
	char			name[ MAX_CHAR ];

	//! Determine if the MD5 is visible for drawing.
	unsigned char	visible;
	
	//! The number of joints that should be used as skeleton.
	unsigned int	n_joint;

	//! The bind pose skeleton.
	MD5JOINT		*bind_pose;

	//! The number of mesh this MD5 consist of.
	unsigned int	n_mesh;

	//! Array of all the MD5MESH contained in the MD5. 
	MD5MESH			*md5mesh;
	
	//! The number of action currently loaded.
	unsigned int	n_action;
	 
	//! Array of MD5ACTIONs for the current MD5.
	MD5ACTION		*md5action;	
	
	//! The XYZ location of the MD5 in world coordinates.
	vec3			location;
	
	//! The XYZ rotation angle of the MD5.
	vec3			rotation;
	
	//! The XYZ scale vector of the MD5.
	vec3			scale;
	
	//! The bottom left corner of the bounding box. (in bind pose)
	vec3			min;
	
	//! The up right corner of the bounding box. (in bind pose)
	vec3			max;
	
	//! The dimension of the bounding box. (in bind pose)
	vec3			dimension;
	
	//! The bounding sphere.
	float			radius;	
	
	//! The current distance of the MD5 from the viewer.
	float			distance;
	
	//! btRigidBody pointer of the current MD5 (if used in physics simulation).
	btRigidBody		*btrigidbody;
	
} MD5;


MD5 *MD5_load_mesh( char *filename, unsigned char relative_path );

int MD5_load_action( MD5 *md5, char *name, char *filename, unsigned char relative_path );

MD5 *MD5_free( MD5 *md5 );

void MD5_free_mesh_data( MD5 *md5 );

MD5ACTION *MD5_get_action( MD5 *md5, char *name, unsigned char exact_name );

MD5MESH *MD5_get_mesh( MD5 *md5, char *name, unsigned char exact_name );

void MD5_action_play( MD5ACTION *md5action, unsigned char frame_interpolation_method, unsigned char loop );

void MD5_action_pause( MD5ACTION *md5action );

void MD5_action_stop( MD5ACTION *md5action );

void MD5_set_action_fps( MD5ACTION *md5action, float fps );

void MD5_set_mesh_attributes( MD5MESH *md5mesh );

void MD5_set_mesh_visibility( MD5MESH *md5mesh, unsigned char visible );

void MD5_set_mesh_material( MD5MESH *md5mesh, OBJMATERIAL *objmaterial );

void MD5_optimize( MD5 *md5, unsigned int vertex_cache_size );

void MD5_build_vbo( MD5 *md5, unsigned int mesh_index );

void MD5_build_bind_pose_weighted_normals_tangents( MD5 *md5 );

void MD5_set_pose( MD5 *md5, MD5JOINT *pose );

void MD5_blend_pose( MD5 *md5, MD5JOINT *final_pose, MD5JOINT *pose0, MD5JOINT *pose1, unsigned char joint_interpolation_method, float blend );

void MD5_add_pose( MD5 *md5, MD5JOINT *final_pose, MD5ACTION *action0, MD5ACTION *action1, unsigned char joint_interpolation_method, float action_weight );

void MD5_build( MD5 *md5 );

void MD5_build2( MD5 *md5 );

unsigned char MD5_draw_action( MD5 *md5, float time_step );

unsigned int MD5_draw( MD5 *md5 );

#endif
