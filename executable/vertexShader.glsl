#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

flat out vec3 normal; // specify a color output to the fragment shader
out vec3 pixelPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //normal = (projection * view * model * vec4(aNormal, 1.0)).xyz;

    normal.x = aNormal.x;
    normal.y = aNormal.y;
    normal.z = aNormal.z;

    //normal = mat3(transpose(inverse(model))) * aNormal;  
    pixelPos = vec3(model * vec4(aPos,1.0));
}