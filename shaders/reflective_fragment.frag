#version 330 core

out vec4 FragColor;
  
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    float refractiveIndex = 1.0 / 1.33; // Adjust as necessary for your scene
    vec3 viewDir = normalize(Position - cameraPos);
    //vec3 reflectDir = reflect(viewDir, normalize(Normal));
    vec3 refractDir = refract(viewDir, normalize(Normal), refractiveIndex);
    FragColor = vec4(texture(skybox, refractDir).rgb, 1.0f);
}