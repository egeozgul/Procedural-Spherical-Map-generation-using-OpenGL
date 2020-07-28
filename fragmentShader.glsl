#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
in vec3 normal; // the input variable from the vertex shader (same name and same type)  
in vec3 pixelPos;

vec3 lightPos = vec3(50,0,0);
vec3 lightColor = vec3(1,3,3);
vec3 objectColor = vec3(1,0,0);

void main()
{
    FragColor = vertexColor;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos-pixelPos);

    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*lightColor*1;
    
    float ambientStrength = 0.001;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse)*objectColor;
    FragColor = vec4(result,1.0);
}