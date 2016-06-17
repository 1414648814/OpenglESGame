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

#ifndef NAVIGATION_H
#define NAVIGATION_H

/*!
	\file navigation.h
	
	\brief Function prototypes and definitions to use with the NAVIGATION structure.
*/

//! The maximum amount of node for the navigation mesh.
#define NAVIGATION_MAX_NODE			1024

//! The maximum amount of poly that can be used as query.
#define NAVIGATION_MAX_POLY			512

//! The maximum amount of poly that can be used with a path.
#define NAVIGATION_MAX_PATH_POLY	256


//! Structure representing the different configuration parameters when building a navigation mesh.
typedef struct
{
	//! The size of each cell (in GL units).
	float cell_size;

	//! The height of the cell (in GL units).
	float cell_height;

	//! The size of the player or moving entity (in GL units).
	float agent_height;

	//! The radius of the player or moving entity (in GL units).
	float agent_radius;
	
	//! The maximum height that a player can climb (in GL units).
	float agent_max_climb;

	//! The maximum slope that the player can walk on (in degree).
	float agent_max_slope;

	//! The minimum region size to include in the navigation mesh (in GL units).
	float region_min_size;

	//! The size to use for mergin region (in GL units).
	float region_merge_size;

	//! The maximum lenght of an edge (in GL units).
	float edge_max_len;

	//! The maximum amount of "tolerable" errors to use for the edges.
	float edge_max_error;

	//! The number of vertex per polygons.
	float vert_per_poly;

	//! The number of details samples based on the distance.
	float detail_sample_dst;

	//! The maximum amount of "tolerable" error on the sample calculation.
	float detail_sample_max_error;

} NAVIGATIONCONFIGURATION;


//! Structure to use to query the navigation mesh.
typedef struct
{
	//! The start location.
	vec3			start_location;

	//! The end location.
	vec3			end_location;

	//! (Optional) The bit mask filter to use while querying.
	dtQueryFilter	path_filter;

	//! Upon query the start reference polygon index will be filled.
	dtPolyRef		start_reference;

	//! Upon query the end reference polygon index will be filled.	
	dtPolyRef		end_reference;
	
	//! Upon query the total number of polygon that the path or ray traverse will be filled.
	unsigned int	poly_count;

	//! Upon query the polygon index information will be filled.	
	dtPolyRef		poly_array[ NAVIGATION_MAX_POLY ];

} NAVIGATIONPATH;


//! If a navigation path query is successful the result will be store in this structure.
typedef struct
{
	//! The number of point that form the navigation path.
	unsigned int	path_point_count;

	//! The flags for each polygon.
	unsigned char	path_flags_array[ NAVIGATION_MAX_PATH_POLY ];

	//! The XYZ position is world coordinates for each navigation points.
	vec3			path_point_array[ NAVIGATION_MAX_PATH_POLY ];
	
	//! The polygon index for each point.
	dtPolyRef		path_poly_array[ NAVIGATION_MAX_PATH_POLY ];

} NAVIGATIONPATHDATA;


//! Main structure to initialize to gain pathfinding functionalities.
typedef struct
{
	//! The internal name for the navigation structure.
	char					name[ MAX_CHAR ];
	
	//! The default configuration to use for the navigation.
	NAVIGATIONCONFIGURATION navigationconfiguration;

	//! The tolerance level on the XYZ axis, when a pathfinding query is emitted.
	vec3					tolerance;

	//! The triangle flags to use for navigation query. With this you can mark water, ground, grass etc.  
	unsigned char			*triangle_flags;

	//! The Detour navigation mesh data.
	dtNavMesh				*dtnavmesh;
	
	//! The shader program auto-generated the first time you call NAVIGATION_draw for debugging. 
	PROGRAM					*program;	

} NAVIGATION;


NAVIGATION *NAVIGATION_init( char *name );

NAVIGATION *NAVIGATION_free( NAVIGATION *navigation );

void NAVIGATION_set_default_configuration( NAVIGATION *navigation );

unsigned char NAVIGATION_build( NAVIGATION *navigation, OBJ *obj, unsigned int mesh_index );

unsigned char NAVIGATION_get_path( NAVIGATION *navigation, NAVIGATIONPATH *navigationpath, NAVIGATIONPATHDATA *navigationpathdata );

void NAVIGATION_draw( NAVIGATION *navigation );

#endif
