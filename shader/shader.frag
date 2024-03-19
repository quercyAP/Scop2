#version 330 core
out vec4 FragColor;

in vec3 Normal; // Normale reçue du vertex shader
in vec2 TexCoords; // Coordonnées de texture reçues du vertex shader

// Texture sampler pour la texture du modèle
uniform sampler2D texture_diffuse;

void main() {
    // Exemple simple de coloration utilisant la texture
    // FragColor = texture(texture_diffuse, TexCoords);
    
    // Si vous voulez voir les normales comme couleur (pour débogage)
    float intensity = max(dot(normalize(Normal), normalize(vec3(0.0, 0.0, 1.0))), 0.0); 
    vec3 color = mix(vec3(0.2, 0.2, 0.2), vec3(1.0, 1.0, 1.0), intensity); // Nuances de gris basées sur l'intensité
    FragColor = vec4(color, 1.0);
}
