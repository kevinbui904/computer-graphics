#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform SceneUniforms {
    mat4 camera;
    vec3 uLight;
    vec4 cLight;

} scene;
layout(binding = 1) uniform BodyUniforms {
    mat4 modeling;
    uvec4 texIndices;
} body;

layout(location = 0) in vec3 attrXYZ;
layout(location = 1) in vec2 attrST;
layout(location = 2) in vec3 attrNOP;

layout(location = 0) out vec2 st;
layout(location = 1) out vec3 dNormal;

void main() {
    vec4 pWorld = body.modeling * vec4(attrNOP, 0.0);
    dNormal = vec3(pWorld);
    gl_Position = scene.camera * (body.modeling * vec4(attrXYZ, 1.0));
    st = attrST;
}

