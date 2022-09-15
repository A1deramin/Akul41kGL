//#shader vertex
//#version 330 core
//layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;
//
//out vec3 v_FragPos;
//uniform mat4 u_Model;
//uniform mat4 u_View;
//uniform mat4 u_Proj;
//
//void main() {
//    gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
//    v_FragPos = gl_Position;
//    v_Normal = normal;
//};
//
//#shader fragment
//#version 330 core
//
//struct Material {
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    float shininess
//};
//
//struct Light {
//    vec3 position;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};
//
//layout(location = 0) out vec4 color;
//in vec3 v_Normal;
//in vec3 v_FragPos;
//uniform vec3 u_ViewPos;
//uniform Material u_Material;
//uniform Light u_Light;
//void main() {
//     
//    vec3 ambient = u_Light.ambient * u_Material.ambient;
//     
//    
//    vec3 norm = normalize(v_Normal);
//    vec3 lightDir = normalize(u_Light.position - v_FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = u_Light.diffuse * diff * u_Material.diffuse;
//
//    
//    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
//    vec3 reflectDir = reflect(-u_LightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
//    vec3 specular = u_Light.specular * spec * u_Material.specular;
//
//    vec3 lighting = ambient + diffuse + specular;
//
//    color = vec4(lighting, 1.0f);
//};