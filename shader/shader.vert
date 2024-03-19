#version 330 core
layout (location = 0) in vec3 aPos; // Position du sommet
layout (location = 1) in vec3 aNormal; // Normale du sommet
layout (location = 2) in vec2 aTexCoords; // Coordonnées de texture du sommet

out vec3 Normal; // Normale passée au fragment shader
out vec2 TexCoords; // Coordonnées de texture passées au fragment shader

uniform mat4 model; // Matrice de modèle pour transformer les coordonnées du modèle en coordonnées du monde
uniform mat4 view; // Matrice de vue pour transformer les coordonnées du monde en coordonnées de vue
uniform mat4 projection; // Matrice de projection pour transformer les coordonnées de vue en coordonnées de l'espace de clip

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    Normal = aNormal; // Ici, on passe simplement la normale sans transformation
    TexCoords = aTexCoords; // Passer simplement les coordonnées de texture
}
