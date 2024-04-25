#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 uMatrix;

out vec2 vTexCoord;

void main(){

    gl_Position = uMatrix * (gl_in[0].gl_Position + vec4(-2, -2, 0, 0));
    vTexCoord = vec2(0, 0);
    EmitVertex();

    gl_Position = uMatrix * (gl_in[0].gl_Position + vec4(2, -2, 0, 0));
    vTexCoord = vec2(1, 0);
    EmitVertex();

    gl_Position = uMatrix * (gl_in[0].gl_Position + vec4(-2, 2, 0, 0));
    vTexCoord = vec2(0, 1);
    EmitVertex();

    gl_Position = uMatrix * (gl_in[0].gl_Position + vec4(2, 2, 0, 0));
    vTexCoord = vec2(1, 1);
    EmitVertex();

    EndPrimitive();

    // gl_Position = uMatrix * gl_in[0].gl_Position + vec4(-2, -2, 0, 0);
    // vTexCoord = vec2(0, 0);
    // EmitVertex();

    // gl_Position = uMatrix * gl_in[0].gl_Position + vec4(2, -2, 0, 0);
    // vTexCoord = vec2(1, 0);
    // EmitVertex();

    // gl_Position = uMatrix * gl_in[0].gl_Position + vec4(-2, 2, 0, 0);
    // vTexCoord = vec2(0, 1);
    // EmitVertex();

    // gl_Position = uMatrix * gl_in[0].gl_Position + vec4(2, 2, 0, 0);
    // vTexCoord = vec2(1, 1);
    // EmitVertex();

    // EndPrimitive();
}