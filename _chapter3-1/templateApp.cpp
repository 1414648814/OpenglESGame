/*

Book:      	Game and Graphics Programming for iOS and Android with OpenGL(R) ES 2.0
Author:    	Romain Marucchi-Foino
ISBN-10: 	1119975913
ISBN-13: 	978-1119975915
Publisher: 	John Wiley & Sons	

Copyright (C) 2011 Romain Marucchi-Foino

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of
this software. Permission is granted to anyone who either own or purchase a copy of
the book specified above, to use this software for any purpose, including commercial
applications subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that
you wrote the original software. If you use this software in a product, an acknowledgment
in the product would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be misrepresented
as being the original software.

3. This notice may not be removed or altered from any source distribution.

*/

#include "templateApp.h"

//负责控制在加载时由浏览器加载的文件（3d文件）
#define OBJ_FILE ( char * )"model.obj"

#define VERTEX_SHADER ( char * )"vertex.glsl"

#define FRAGMENT_SHADER ( char * )"fragment.glsl"

#define DEBUG_SHADERS 1

//为了从文件中加载复杂几何图形而使用的主结构
OBJ *obj = NULL;

/*
指向obj结构中网格的指针，通过该指针，可以访问组成几何图形的所有顶点数据
以及为网格使用的每个材质所创建的三角形列表
*/
OBJMESH *objmesh = NULL;

PROGRAM *program = NULL;


TEMPLATEAPP templateApp = { templateAppInit,
							templateAppDraw,
							templateAppToucheBegan,
							templateAppToucheMoved,
                            templateAppToucheEnded,
                            templateAppToucheCancelled};

//作为供Program结构使用的回调，每次调用Program_draw时都会自动触发该函数；
//循环检查着色器中所有的uniform变量，并获取对每个变量的控制，可将回调链接到
//一个或者多个着色器，并在统一位置处理所有的uniform变量更新
void program_draw_callback( void *ptr ) {

	PROGRAM *curr_program = ( PROGRAM * )ptr;

	unsigned int i = 0;

	while( i != curr_program->uniform_count ) {

		if( !strcmp( curr_program->uniform_array[ i ].name, "MODELVIEWPROJECTIONMATRIX" ) ) {

			glUniformMatrix4fv( curr_program->uniform_array[ i ].location,
								1,
								GL_FALSE,
								( float * )GFX_get_modelview_projection_matrix() ); 
		}


		++i;
	}
}


void templateAppInit( int width, int height ) {

	atexit( templateAppExit );

	GFX_start();

	glViewport( 0.0f, 0.0f, width, height );

	GFX_set_matrix_mode( PROJECTION_MATRIX );
	GFX_load_identity();
	GFX_set_perspective( 45.0f,
						 ( float )width / ( float )height,
						 0.1f,
						 100.0f,
						 0.0f );
    //新建一个可自动加载、编译和链接着色器程序的着色器程序
	program = PROGRAM_create( ( char * )"default",
							  VERTEX_SHADER,
							  FRAGMENT_SHADER,
							  1,
							  DEBUG_SHADERS,
							  NULL,
							  program_draw_callback );

	obj = OBJ_load( OBJ_FILE, 1 );   


	unsigned char *vertex_array = NULL,
				  *vertex_start = NULL;

	unsigned int i	    = 0,
				 index  = 0,
				 stride = 0,
				 size   = 0;

    //获取结构指针中第一个网格的指针，
	objmesh = &obj->objmesh[ 0 ];

    //计算顶点数据数组的总大小，以便可以分配为VBO构建GLES友好的顶点数据数组所需的内存大小
	size = objmesh->n_objvertexdata * sizeof( vec3 ) * sizeof( vec3 );

	vertex_array = ( unsigned char * ) malloc( size );
	vertex_start = vertex_array;

    //根据索引的顶点位置以及objmesh结构中包含的顶点法线构建顶点数据数组
	while( i != objmesh->n_objvertexdata ) { 

		index = objmesh->objvertexdata[ i ].vertex_index;

		memcpy( vertex_array,
				&obj->indexed_vertex[ index ],
				sizeof( vec3 ) );

		vertex_array += sizeof( vec3 );


		memcpy( vertex_array,
				&obj->indexed_normal[ index ],
				sizeof( vec3 ) );

		vertex_array += sizeof( vec3 );

		++i;
	}
    
    //请求驱动程序为VBO创建一个新的缓冲区索引，并使其处于活动状态
	glGenBuffers( 1, &objmesh->vbo ); 
	glBindBuffer( GL_ARRAY_BUFFER, objmesh->vbo );

    //将顶点数据数组从本地存储器转移到视频存储器
	glBufferData( GL_ARRAY_BUFFER,
				  size,
				  vertex_start,
				  GL_STATIC_DRAW );

	free( vertex_start );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

    //为第一个objmesh三角形列表创建一个新id，并使当前索引处于活动状态
	glGenBuffers( 1, &objmesh->objtrianglelist[ 0 ].vbo );
	
    
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 
				  objmesh->objtrianglelist[ 0 ].vbo );

    //以类似发送数组缓冲区的方式，将该索引数组发送到GPU
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
				  objmesh->objtrianglelist[ 0 ].n_indice_array *
				  sizeof( unsigned short ),
				  objmesh->objtrianglelist[ 0 ].indice_array,
				  GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );


    //创建VAO
    //计算不同顶点数据之间的顶点数组跨度
	unsigned char attribute;
				  stride = sizeof( vec3 )+
						   sizeof( vec3 );

    //创建一个新的VAO索引，并使其保持活动状态
	glGenVertexArraysOES( 1, &objmesh->vao );

	glBindVertexArrayOES( objmesh->vao );

    //开始构建VAO列表绑定，其中包括相关的调用来设置数组缓冲区
	glBindBuffer( GL_ARRAY_BUFFER, objmesh->vbo );

    //在VAO列表中包括POSITION顶点属性调用
	attribute = PROGRAM_get_vertex_attrib_location( program, ( char * )"POSITION" );

	glEnableVertexAttribArray( attribute );

	glVertexAttribPointer( attribute,
						   3,
						   GL_FLOAT,
						   GL_FALSE,
						   stride,
						   ( void * )NULL );
    
    //以处理顶点位置相同的方式处理顶点法线，但是顶点属性指针调用的最后一个参数稍有不同，必须指定距离下一个
    //数据类型的偏移量
	attribute = PROGRAM_get_vertex_attrib_location( program, ( char * )"NORMAL" );

	glEnableVertexAttribArray( attribute );

	glVertexAttribPointer( attribute,
						   3,
						   GL_FLOAT,
						   GL_FALSE,
						   stride,
						   BUFFER_OFFSET( sizeof( vec3 ) ) );
    
    //在关闭VAO列表之前绑定数组元素缓冲区（索引）
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
				  objmesh->objtrianglelist[ 0 ].vbo );					   

    //停用当前的VAO，其次对前面所调用的类似数组的所有命令进行编译，然后将它们与VAO索引相关亮
	glBindVertexArrayOES( 0 );
}


void templateAppDraw( void ) {

	glClearColor( 0.5f, 0.5f, 0.5f, -1.0f );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );


	GFX_set_matrix_mode( MODELVIEW_MATRIX );
    //创建一个关键矩阵，并在Y轴上设置一个向后的偏移量，以便能够看到模型
	GFX_load_identity(); {
	
		vec3 e = { 0.0f,  -4.0f, 0.0f },
			 c = { 0.0f,  0.0f, 0.0f },
			 u = { 0.0f,  0.0f, 1.0f };

		GFX_look_at( &e, &c, &u ); 
	}

    //绑定VAO
	glBindVertexArrayOES( objmesh->vao );

    //设置绘制的着色器程序（自动使用glUseProgram绑定到着色器程序，然后发送program_draw_callback中执行指针，调用该函数）
	PROGRAM_draw( program );

    //绘图语句
	glDrawElements( GL_TRIANGLES,
					objmesh->objtrianglelist[ 0 ].n_indice_array,
					GL_UNSIGNED_SHORT,
					( void * )NULL );
}

//触屏动作
void templateAppToucheBegan( float x, float y, unsigned int tap_count ) {
}


void templateAppToucheMoved( float x, float y, unsigned int tap_count ) {
}

void templateAppToucheEnded( float x, float y, unsigned int tap_count ) {
}

void templateAppToucheCancelled( float x, float y, unsigned int tap_count ) {
    
}

//清理一切已经初始化的内容
void templateAppExit( void ) {

	SHADER_free( program->vertex_shader );
	
	SHADER_free( program->fragment_shader );
	
	PROGRAM_free( program );
	
	OBJ_free( obj );
}
