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

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse;

void main()
{
    // Calcul de l'ambiance
    vec3 ambient = material.ambient * light.color;
    
    // Calcul de la lumière diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * light.color;
    
    // Calcul de la lumière spéculaire
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.color;
    
    vec3 result = ambient + diffuse + specular;
    if(material.hasTexture) {
        result *= texture(texture_diffuse, TexCoords).rgb;
    }
    FragColor = vec4(result, 1.0);
}

