#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

flat in int faceIndex;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasTexture;
    sampler2D texture_diffuse;
};

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

uniform float textureMixFactor;

uniform bool faceMode;

void main()
{
    vec3 result;

    if (faceMode) {
        float r = float((faceIndex * 37) % 255) / 255.0;
        float g = float((faceIndex * 53) % 255) / 255.0;
        float b = float((faceIndex * 101) % 255) / 255.0;
        result = vec3(r, g, b);
    } else {
        // Calcul de l'ambiance
        vec3 ambient = material.ambient * light.color;
        
        // Calcul de la lumière diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = material.diffuse * diff * light.color;
        
        vec3 specular = vec3(0.0);

        // Calcul de la lumière spéculaire uniquement si shininess > 0
        if (material.shininess > 0.0) {
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            specular = material.specular * spec * light.color;
        }
        
        result = ambient + diffuse + specular;
        if(material.hasTexture) {
            vec3 textureColor = texture(material.texture_diffuse, TexCoords).rgb;

            result = mix(result, textureColor * (ambient + diffuse), textureMixFactor);
        }
    }
    FragColor = vec4(result, 1.0);
}

