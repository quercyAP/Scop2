#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasTexture;
};

uniform Material material;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = material.ambient;
    vec3 diffuse = material.diffuse * max(dot(normalize(Normal), normalize(-FragPos)), 0.0);
    vec3 specular = material.specular * pow(max(dot(viewPos, reflect(-normalize(FragPos), normalize(Normal))), 0.0), material.shininess);
    
    vec3 result = ambient + diffuse + specular;
    if(material.hasTexture)
    {
        result *= texture(0, TexCoords).rgb;
    }
    FragColor = vec4(result, 1.0);
}
