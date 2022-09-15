#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
void main()
{
	v_Color = color;
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;
void main()
{
	color = v_Color;
};