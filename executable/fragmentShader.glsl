#version 330 core
out vec4 FragColor;
  
flat in vec3 normal; // the input variable from the vertex shader (same name and same type)  
in vec3 pixelPos;

vec3 lightPos = vec3(90,0,0);
vec3 lightColor = vec3(1,1,1);
uniform vec3 objectColor;

void main()
{
    vec3 dFdxPos = dFdx( pixelPos );
	vec3 dFdyPos = dFdy( pixelPos );
	vec3 norm = normalize( cross(dFdxPos,dFdyPos ));
    
    vec3 lightDir = normalize(lightPos-pixelPos);

    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff*normalize(lightColor);
    
    float ambientStrength = 0.10;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient*1.5 + diffuse)*objectColor*2;
    
    
    FragColor = vec4(result,1.0);
}
