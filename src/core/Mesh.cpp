/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.hpp"

Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<float> &normals,
           const std::vector<float> &textureCoords,
           const std::vector<unsigned int> &indices)
{
    // Calculez la taille correcte pour le buffer en fonction des vertices, normales et coordonnées de texture
    std::vector<float> combinedData;
    for (size_t i = 0; i < vertices.size() / 3; ++i)
    {
        // Ajouter les données de vertex
        combinedData.push_back(vertices[i * 3]);
        combinedData.push_back(vertices[i * 3 + 1]);
        combinedData.push_back(vertices[i * 3 + 2]);

        // Ajouter les données de normale
        combinedData.push_back(normals[i * 3]);
        combinedData.push_back(normals[i * 3 + 1]);
        combinedData.push_back(normals[i * 3 + 2]);

        // Ajouter les données de texture
        combinedData.push_back(textureCoords[i * 2]);
        combinedData.push_back(textureCoords[i * 2 + 1]);
    }

    vertexCount = static_cast<int>(indices.size());
    cout << "Vertex count: " << vertexCount << endl;
    setupMesh(combinedData, indices);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh(const std::vector<float> &combinedData, const std::vector<unsigned int> &indices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, combinedData.size() * sizeof(float), combinedData.data(), GL_STATIC_DRAW);

    // La taille d'un vertex est maintenant la somme des tailles de position, normale et texture
    size_t stride = (3 + 3 + 2) * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);

    // Les normales commencent après les positions
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Les coordonnées de texture commencent après les positions et les normales
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0); // Unbind VAO
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}