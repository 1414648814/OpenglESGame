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
#define OBJ_FILE (char *) "scene.obj"

OBJ * obj = NULL;

/* The main structure of the template. This is a pure C struct, you initialize the structure
   as demonstrated below. Depending on the type of your type of app simply comment / uncomment
   which event callback you want to use. */

TEMPLATEAPP templateApp = {
							/* Will be called once when the program start. */
							templateAppInit,
							
							/* Will be called every frame. This is the best location to plug your drawing. */
							templateAppDraw,
							
							/* This function will be triggered when a new touche is recorded on screen. */
							//templateAppToucheBegan,
							
							/* This function will be triggered when an existing touche is moved on screen. */
							//templateAppToucheMoved,
							
							/* This function will be triggered when an existing touche is released from the the screen. */
							//templateAppToucheEnded,
							
							/* This function will be called everytime the accelerometer values are refreshed. Please take
							not that the accelerometer can only work on a real device, and not on the simulator. In addition
							you will have to turn ON the accelerometer functionality to be able to use it. This will be
							demonstrated in the book later on. */
							//templateAppAccelerometer // Turned off by default.
						  };


void program_bind_attrib_location(void *ptr) {
    PROGRAM * program = (PROGRAM *)ptr;
    // 告诉着色器程序将要使用的属性位置
    glBindAttribLocation(program->pid, 0, "POSITION");
    glBindAttribLocation(program->pid, 2, "TEXCOORD0");
}

void material_draw_callback(void *ptr) {
    OBJMATERIAL * objmaterial = (OBJMATERIAL *)ptr;
    PROGRAM * program = objmaterial->program;
    unsigned int i = 0;
    while (i != program->uniform_count) {
        if (!strcmp(program->uniform_array[i].name, "DIFFUSE")) {
            glUniform1i(program->uniform_array[i].location, 1);
        }
        else if (!strcmp(program->uniform_array[i].name, "MODELVIEWPROJECTIONMATRIX")) {
            glUniformMatrix4fv(program->uniform_array[i].location, 1, GL_FALSE, (float *)GFX_get_modelview_projection_matrix());
        }
        
        i++;
    }
}

void templateAppInit( int width, int height )
{
	// Setup the exit callback function.
	atexit( templateAppExit );
	
	// Initialize GLES.
	GFX_start();
	
	// Setup a GLES viewport using the current width and height of the screen.
	glViewport( 0, 0, width, height );
	
	/* Insert your initialization code here */
    GFX_set_matrix_mode(PROJECTION_MATRIX);
    GFX_load_identity();
    // 旋转投影矩阵90度
    GFX_set_perspective(45.0f, float(width)/float(height), 0.1f, 100.0f, -90.0f);
    
    obj = OBJ_load(OBJ_FILE, 1);
    
    // 为每个网格对象生成VBO和VAO
    unsigned int i = 0;
    while (i != obj->n_objmesh) {
        OBJ_build_mesh(obj, i);
        // 释放顶点相关联的数组
        OBJ_free_mesh_vertex_data(obj, i);
        i++;
    }
    
    // 加载mtl文件中链接的所有纹理，并生成纹理ID
    i = 0;
    while (i != obj->n_texture) {
        OBJ_build_texture(obj, i, obj->texture_path, TEXTURE_MIPMAP, TEXTURE_FILTER_2X, 0.0f);
        ++i;
    }
    
    // 将所有纹理连接到它们各自的材质上，并使用前面创建的两个着色器文件手动为每个纹理创建一个着色器程序
    i = 0;
    MEMORY * vertex_shader = mopen((char *)"vertex.glsl", 1);
    while (i != obj->n_objmaterial) {
        // 对于将要在循环中创建的每种材质，都需要访问片段着色器的一个干净副本
        MEMORY * fragment_shader = mopen((char *)"fragment.glsl", 1);
        
        OBJMATERIAL * objmaterial = &obj->objmaterial[i];
        if (objmaterial->dissolve == 1.0f) {
            minsert(fragment_shader, (char *)"#define SOLID_OBJECT\n", 0);
        }
        else if (!objmaterial->dissolve) {
            minsert(fragment_shader, (char *)"#define ALPHA_TESTED_OBJECT\n", 0);
        }
        else {
            minsert(fragment_shader, (char *)"#define TRANSPARENT_OBJECT\n", 0);
        }
        
        // 编译当前材质的着色器
        objmaterial->program = PROGRAM_init(objmaterial->name);
        objmaterial->program->vertex_shader = SHADER_init((char *) "vertex", GL_VERTEX_SHADER);
        objmaterial->program->fragment_shader = SHADER_init((char *) "fragment", GL_FRAGMENT_SHADER);
        
        SHADER_compile(objmaterial->program->vertex_shader, (char *)vertex_shader->buffer, 1);
        SHADER_compile(objmaterial->program->fragment_shader, (char *)fragment_shader->buffer, 1);
        
        // 绑定回调函数
        PROGRAM_set_bind_attrib_location_callback(objmaterial->program, program_bind_attrib_location);
        
        PROGRAM_link(objmaterial->program, 1);
        
        OBJ_set_draw_callback_material(obj, i, material_draw_callback);
        
        mclose(fragment_shader);
        i++;
    }
    
    mclose(vertex_shader);
    
}


void templateAppDraw( void )
{
	// Clear the depth, stencil and colorbuffer.
	glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	/* Insert your drawing code here */
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    GFX_set_matrix_mode(MODELVIEW_MATRIX);
    GFX_load_identity();
    // 摄像头的位置
    vec3 e = {0.0f, -6.0f, 1.35f},
    c = {0.0f, -5.0f, 1.35f},
    u = {0.0f, 0.0f, 1.0f};
    
    GFX_look_at(&e, &c, &u);
    
    // 遍历所有对象，并在屏幕上绘制出来
    unsigned int i = 0;
    while (i != obj->n_objmesh) {
        OBJMATERIAL * objmaterial = obj->objmesh[i].objtrianglelist[0].objmaterial;
        // 判断是否是固体对象
        if (objmaterial->dissolve == 1.0f) {
            GFX_push_matrix();
            GFX_translate(obj->objmesh[i].location.x, obj->objmesh[i].location.y, obj->objmesh[i].location.z);
            OBJ_draw_mesh(obj, i);
            GFX_pop_matrix();
            i++;
        }
    }
    
    i = 0;
    while (i != obj->n_objmesh) {
        OBJMATERIAL * objmaterial = obj->objmesh[i].objtrianglelist[0].objmaterial;
        // 判断是否是一个Alpha测试对象(dissvole = 0)
        if (!objmaterial->dissolve) {
            GFX_push_matrix();
            GFX_translate(obj->objmesh[i].location.x, obj->objmesh[i].location.y, obj->objmesh[i].location.z);
            OBJ_draw_mesh(obj, i);
            GFX_pop_matrix();
            i++;
        }
    }
    
    // 必须告诉显卡启用混合以及使用纹理的Alpha值，从而对每个可视像素进行混合操作
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    i = 0;
    while (i != obj->n_objmesh) {
        OBJMATERIAL * objmaterial = obj->objmesh[i].objtrianglelist[0].objmaterial;
        if (objmaterial->dissolve > 0.0f && objmaterial->dissolve < 1.0f) {
            GFX_push_matrix();
            GFX_translate(obj->objmesh[i].location.x, obj->objmesh[i].location.y, obj->objmesh[i].location.z);
            OBJ_draw_mesh(obj, i);
            GFX_pop_matrix();
            i++;
        }
    }
    glDisable(GL_BLEND);
    
}


void templateAppToucheBegan( float x, float y, unsigned int tap_count )
{
	/* Insert code to execute when a new touche is detected on screen. */
}


void templateAppToucheMoved( float x, float y, unsigned int tap_count )
{
	/* Insert code to execute when a touche move on screen. */
}


void templateAppToucheEnded( float x, float y, unsigned int tap_count )
{
	/* Insert code to execute when a touche is removed from the screen. */
}


void templateAppAccelerometer( float x, float y, float z )
{
	/* Insert code to execute with the accelerometer values ( when available on the system ). */
}


void templateAppExit( void )
{
	/* Code to run when the application exit, perfect location to free everything. */
    unsigned i = 0;
    while (i != obj->n_objmaterial) {
        SHADER_free(obj->objmaterial[i].program->vertex_shader);
        SHADER_free(obj->objmaterial[i].program->fragment_shader);
        PROGRAM_free(obj->objmaterial[i].program);
    }
    OBJ_free(obj);
}
