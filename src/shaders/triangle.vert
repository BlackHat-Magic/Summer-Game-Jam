#version 450

layout(set = 1, binding = 0) uniform PushConstants {
    mat4 rotationMatrix;
} ubo;

// output to fragment shader
layout(location = 0) out vec3 fragColor;

// triangle positions in clip space
vec3 positions[3] = vec3[](
    vec3(0.0, 0.5, 0.0),
    vec3(-0.5, -0.5, 0.0),
    vec3(0.5, -0.5, 0.0)
);

// colors for each vertex
vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    vec4 pos = ubo.rotationMatrix * vec4(positions[gl_VertexIndex], 1.0);
    gl_Position = pos;
    fragColor = colors[gl_VertexIndex];
}