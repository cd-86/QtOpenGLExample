#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 uMatrix;

void main(){

    vec3 edge1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;  
    vec3 edge2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 normal = normalize(cross(edge1, edge2));

    vec3 pos = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz) / 3.0f;

    gl_Position = uMatrix * vec4(pos, 1.0f);
    EmitVertex();
    gl_Position = uMatrix * vec4(pos + normal, 1.0f);  
    EmitVertex(); 
    EndPrimitive();

}