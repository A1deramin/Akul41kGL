#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;

out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform vec3 u_LightPos;
void main() {
    mat4 MVMat = u_View * u_Model;
    
    vec3 modelViewVertex = vec3(MVMat * vec4(position, 1.0));
    vec3 modelViewNormal = vec3(MVMat * vec4(normal, 1.0));
    float distance = length(u_LightPos - modelViewVertex);
    vec3 lightDir = normalize(u_LightPos - modelViewVertex);
    float diffuse = max(dot(modelViewNormal, lightDir), 0.1) * (1.0 / (1.0 + (0.25 * distance * distance)));
    v_Color = color * diffuse;
    gl_Position = u_Proj * MVMat * vec4(position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Color;
void main() {
    color = v_Color;
};