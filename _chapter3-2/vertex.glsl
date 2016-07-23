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

uniform mediump mat4 MODELVIEWMATRIX;  //处理模型视图
uniform mediump mat4 PROJECTIONMATRIX;  //处理投影矩阵
uniform mediump mat3 NORMALMATRIX;  //当前的法线矩阵
uniform mediump vec3 LIGHTPOSITION;  //当前光源位置的uniform变量

// 用于连接片段着色器和顶点的光的RGB颜色
varying lowp vec3 lightcolor;

//用来保存顶点位置的顶点属性
attribute mediump vec3 POSITION;

//为顶点法线声明新的顶点属性，以及用来建立该属性值与片段着色器之间的桥梁的相应的varying变量；
attribute lowp vec3 NORMAL;

lowp vec3 normal;

void main( void ) { 
    mediump vec3 position = vec3(MODELVIEWMATRIX * vec4(POSITION, 1.0));
    normal = normalize(NORMALMATRIX * NORMAL);
    
    // 计算当前顶点位置的光线方向
    mediump vec3 lightdirection = normalize(LIGHTPOSITION - position);
    
    // 根据光线的方向向量计算当前顶点位置的光强度
    lowp float ndotl = max(dot(normal, lightdirection), 0.0);
    
    // 将光强度乘以光色
    lightcolor = ndotl * vec3(1.0);
    
    gl_Position = PROJECTIONMATRIX * vec4(position, 1.0);
}
