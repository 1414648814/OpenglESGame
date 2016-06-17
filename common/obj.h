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

#ifndef OBJ_H
#define OBJ_H


/*!
	\file obj.h
	
	\brief Function prototypes and definitions to use with the OBJ structure.
*/


//! OBJMATERIAL draw callback function prototype.
typedef void( MATERIALDRAWCALLBACK( void * ) );


//! Wavefront OBJMATERIAL structure definition.
typedef struct
{
	//! The material name.
	char					name[ MAX_CHAR ];				// newmtl
	
	//! The ambient color of the material.
	vec4					ambient;						// Ka
	
	//! The diffuse color of the material.
	vec4					diffuse;						// Kd

	//! The specular color of the material.
	vec4					specular;						// Ks
	
	//! The transmission filter of the material.
	vec3					transmission_filter;			// Tf

	//! The illumination model of the material.
	int						illumination_model;				// illum
	
	//! The material dissolve factor (aka Alpha).
	float					dissolve;						// d
	
	//! The specular exponent of the material (aka Hardness or Shiness).
	float					specular_exponent;				// Ns

	//! The optical density of the material.
	float					optical_density;				// Ni
	
	//! The ambient texture channel filename.
	char					map_ambient[ MAX_CHAR ];		// map_Ka
	
	//! The diffuse texture channel filename.
	char					map_diffuse[ MAX_CHAR ];		// map_Kd
	
	//! The specular texture channel filename.
	char					map_specular[ MAX_CHAR ];		// map_Ks

	//! The translucency texture channel filename.
	char					map_translucency[ MAX_CHAR ];	// map_Tr

	//! The displacement map texture channel filename.
	char					map_disp[ MAX_CHAR ];			// disp or map_disp

	//! The bump map (aka Normal Map) texture channel filename.
	char					map_bump[ MAX_CHAR ];			// bump or map_bump	
	
	//! The ambient TEXTURE pointer. 
	TEXTURE					*texture_ambient;

	//! The diffuse TEXTURE pointer. 
	TEXTURE					*texture_diffuse;

	//! The specular TEXTURE pointer. 
	TEXTURE					*texture_specular;

	//! The translucency TEXTURE pointer. 
	TEXTURE					*texture_translucency;

	//! The displacement TEXTURE pointer. 
	TEXTURE					*texture_disp;

	//! The bumpmap TEXTURE pointer. 
	TEXTURE					*texture_bump;

	//! The shader PROGRAM to use when drawing the material.
	PROGRAM					*program;
	
	//! The material draw callback function pointer to use.
	MATERIALDRAWCALLBACK	*materialdrawcallback;
	
} OBJMATERIAL;


//! Structure definition to hold all the vertex and UV index for a specific triangle.
typedef struct
{
	int vertex_index[ 3 ];
	
	int uv_index[ 3 ];

} OBJTRIANGLEINDEX;


//! Structure definition that represent one triangle list contained inside an OBJMESH.
typedef struct
{
	//! The number of triangle index.
	unsigned int	 n_objtriangleindex;
	
	//! Triangle arrady that contain each triangle index data.
	OBJTRIANGLEINDEX *objtriangleindex;

	//! Flag to determine if the OBJTRIANGLELIST is using UVs.
	unsigned char	 useuvs;
	
	//! The number of indice required to draw the triangle list.
	unsigned short	 n_indice_array;
	
	//! Array of indices.
	unsigned short   *indice_array;
	
	//! Pointer to an OBJMATERIAL to use when drawing the OBJTRIANGLELIST. 
	OBJMATERIAL		 *objmaterial;
	
	//! The drawing mode to use. (Default: GL_TRIANGLES)
	int				 mode;
	
	//! The internal VBO id maintained by GLES to draw the OBJTRIANGLELIST.
	unsigned int	 vbo;

} OBJTRIANGLELIST;


//! Structure definition for a unique vertex data indices.
typedef struct
{
	//! The vertex index of the OBJVERTEXDATA.
	int vertex_index;
	
	//! The uv index of the OBJVERTEXDATA.
	int uv_index;

} OBJVERTEXDATA;


//! Structure definition to represent a single object entry contained in a Wavefront OBJ file.
typedef struct
{
	//! The name of the object.
	char			name[ MAX_CHAR ];  // o
	
	//! Flag to determine if the object is visible for drawing.
	unsigned char	visible;
		
	//! The object group (if any).
	char			group[ MAX_CHAR ]; // g

	//! The number of OBJVERTEXDATA for this mesh.
	unsigned short	n_objvertexdata;
	
	//! Array of OBJVERTEXDATA to be able to construct the mesh.
	OBJVERTEXDATA	*objvertexdata;
	
	//! The number of triangle list for this OBJMESH.
	unsigned char	n_objtrianglelist;
	
	//! Array of triangle list, basically one list for each OBJMATERIAL required to draw the OBJMESH.
	OBJTRIANGLELIST	*objtrianglelist;
	
	//! The current OBJMATERIAL pointer that the OBJ_draw_mesh function is handling (ideal for external callbacks).
	OBJMATERIAL		*current_material;
	
	//! The XYZ location in worldspace of the OBJMESH.
	vec3			location;

	//! The XYZ euler angle rotation of the OBJMESH.
	vec3			rotation;
	
	//! The XYZ scale vector for the current OBJMESH.
	vec3			scale;
	
	//! The bottom left XYZ coordinate in worldspace of the OBJMESH bounding box.
	vec3			min;
	
	//! The upper right XYZ coordinate in worldspace of the OBJMESH bounding box.
	vec3			max;
	
	//! The XYZ dimension of the bounding box.
	vec3			dimension;
	
	//! The bounding sphere radius.
	float			radius;	

	//! The distance of the OBJMESH with the viewer.
	float			distance;
	
	//! The vertex buffer VBO id maintained by GLES.
	unsigned int	vbo;
	
	//! The stride size in bytes to determine where the next chunk of data in the vertex buffer is located.
	unsigned int	stride;
	
	//! The total size of the vertex data array.
	unsigned int	size;
	
	//! The VBO offsets.
	unsigned int	offset[ 5 ];
	
	//! The VAO id maintained by GLES.
	unsigned int	vao;	
	
	//! Pointer a btRigidBody inside a physical world.
	btRigidBody		*btrigidbody;
	
	//! Determine if the OBJMESH is using vertex or face normals.
	unsigned char	use_smooth_normals;

} OBJMESH;


typedef struct
{
	//! The texture path (By default relative to the location of the .mtl file).
	char			texture_path[ MAX_PATH ];

	//! The shader program path (By default relative to the location of the .mtl file).
	char			program_path[ MAX_PATH ];
	
	//! The number of OBJMESH.
	unsigned int	n_objmesh;
	
	//! Array of OBJMESH for each object entry of the .obj file.
	OBJMESH			*objmesh;
	
	//! The number of OBJMATERIAL.
	unsigned int	n_objmaterial;
	
	//! Array of OBJMATERIAL for each material entry inside a .mtl file.
	OBJMATERIAL		*objmaterial;
	
	//! The number of texture that the current OBJ is using.
	unsigned int	n_texture;
	
	//! Array of unique TEXTURE entry used by the OBJ.
	TEXTURE			**texture;

	//! The number of shader PROGRAM the OBJ is using.
	unsigned int	n_program;

	//! Array of unique PROGRAM entry used by the OBJ.
	PROGRAM			**program;
	
	//! The number of indexed vertex (one unique entry for each vertex/normal/fnormal/tangent)
	unsigned int	n_indexed_vertex;
	
	//! Array of indexed vertices.
	vec3			*indexed_vertex;	// v

	//! Array of indexed normal.
	vec3			*indexed_normal;

	//! Array of indexed face normals.
	vec3			*indexed_fnormal;

	//! Array of indexed tangents.
	vec3			*indexed_tangent;

	//! Number of indexed UVs.
	unsigned int	n_indexed_uv;
	
	//! Array of indexed UVs.
	vec2			*indexed_uv;		// vt

} OBJ;


void OBJ_build_texture( OBJ *obj, unsigned int texture_index, char *texture_path, unsigned int flags, unsigned char filter, float anisotropic_filter );

void OBJ_build_program( OBJ	*obj, unsigned int program_index, PROGRAMBINDATTRIBCALLBACK *programbindattribcallback, PROGRAMDRAWCALLBACK *programdrawcallback, unsigned char debug_shader, char *program_path );

void OBJ_build_material( OBJ *obj, unsigned int material_index, PROGRAM	*program );

void OBJ_set_draw_callback_material( OBJ *obj, unsigned int material_index, MATERIALDRAWCALLBACK *materialdrawcallback );

void OBJ_update_bound_mesh( OBJ *obj, unsigned int mesh_index );

void OBJ_build_vbo_mesh( OBJ *obj, unsigned int mesh_index );

void OBJ_set_attributes_mesh( OBJ *obj, unsigned int mesh_index );

void OBJ_build_mesh( OBJ *obj, unsigned int mesh_index );

void OBJ_build_mesh2( OBJ *obj, unsigned int mesh_index );

void OBJ_optimize_mesh( OBJ *obj, unsigned int mesh_index, unsigned int vertex_cache_size );

OBJMESH *OBJ_get_mesh( OBJ *obj, const char *name, unsigned char exact_name );

int OBJ_get_mesh_index( OBJ *obj, const char *name, unsigned char exact_name );

PROGRAM *OBJ_get_program( OBJ *obj, const char *name, unsigned char exact_name );

OBJMATERIAL *OBJ_get_material( OBJ *obj, const char *name, unsigned char exact_name );

TEXTURE *OBJ_get_texture( OBJ *obj, const char *name, unsigned char exact_name );

void OBJ_draw_material( OBJMATERIAL *objmaterial );

unsigned int OBJ_draw_mesh( OBJ *obj, unsigned int mesh_index );

unsigned int OBJ_draw_mesh2( OBJ *obj, OBJMESH *objmesh );

unsigned int OBJ_draw_mesh3( OBJ *obj, OBJMESH *objmesh );

void OBJ_free_mesh_vertex_data( OBJ *obj, unsigned int mesh_index );

unsigned char OBJ_load_mtl( OBJ *obj, char *filename, unsigned char relative_path );

OBJ *OBJ_load( char *filename, unsigned char relative_path );

void OBJ_free_vertex_data( OBJ *obj );

OBJ *OBJ_free( OBJ *obj );

#endif
