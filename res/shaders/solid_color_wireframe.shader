#shader vertex
#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
void main()
{
	gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
};

#shader fragment
#version 330 core
uniform vec4 u_Color;
layout(location = 0) out vec4 FragColor;
// The mesh line settings
uniform float u_LineWidth;
uniform vec4 u_LineColor;
noperspective in vec3 GEdgeDistance;
void main()
{
	// The shaded surface color.
	vec4 color = u_Color;

	// Find the smallest distance
	float d = min(GEdgeDistance.x, GEdgeDistance.y);

	d = min(d, GEdgeDistance.z);
	// Determine the mix factor with the line color

	float mixVal = smoothstep(u_LineWidth - 1,
		u_LineWidth + 1, d);

	// Mix the surface color with the line color
	FragColor = mix(u_LineColor, color, mixVal );
		
};

#shader geometry
#version 330
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
uniform int u_ScreenWidth;
uniform int u_ScreenHeight;
noperspective out vec3 GEdgeDistance;
void main() {
	 //Transform to screen space
	vec3 p0 = vec3((gl_in[0].gl_Position /
		gl_in[0].gl_Position.w));
	vec3 p1 = vec3( (gl_in[1].gl_Position /
		gl_in[1].gl_Position.w));
	vec3 p2 = vec3( (gl_in[2].gl_Position /
		gl_in[2].gl_Position.w));
	p0 = vec3((p0.x + 1.0) * u_ScreenWidth / 2.0, (p0.y + 1.0) * u_ScreenHeight / 2.0, p0.z);
	p1 = vec3((p1.x + 1.0) * u_ScreenWidth / 2.0, (p1.y + 1.0) * u_ScreenHeight / 2.0, p1.z);
	p2 = vec3((p2.x + 1.0) * u_ScreenWidth / 2.0, (p2.y + 1.0) * u_ScreenHeight / 2.0, p2.z);

	// Find the altitudes (ha, hb and hc)
	float a = length(p1 - p2);
	float b = length(p2 - p0);
	float c = length(p1 - p0);
	float alpha = acos((b * b + c * c - a * a) / (2.0 * b * c));
	float beta = acos((a * a + c * c - b * b) / (2.0 * a * c));
	float ha = abs(c * sin(beta));
	float hb = abs(c * sin(alpha));
	float hc = abs(b * sin(alpha));

	// Send the triangle along with the edge distances
	GEdgeDistance = vec3(ha, 0, 0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	GEdgeDistance = vec3(0, hb, 0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	GEdgeDistance = vec3(0, 0, hc);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}
