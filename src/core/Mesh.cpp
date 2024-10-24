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

Mesh::Mesh(const vector<float> &vertices,
           const vector<float> &normals,
           const vector<float> &textureCoords,
           const vector<unsigned int> &indices,
           const Material *material,
           const string &texturePath) 
{
    vector<float> combinedData;
    bool hasNormals = !normals.empty();
    bool hasTextureCoords = !textureCoords.empty();
    this->material = material;
    texture = new Texture(texturePath);

    for (size_t i = 0; i < vertices.size() / 3; i++)
    {
        // Ajouter les données de vertex
        combinedData.insert(combinedData.end(), {vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]});

        // Conditionnellement ajouter les données de normale
        if (hasNormals)
        {
            combinedData.insert(combinedData.end(), {normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]});
        }

        // Conditionnellement ajouter les données de texture
        if (hasTextureCoords)
        {
            combinedData.insert(combinedData.end(), {textureCoords[i * 2], textureCoords[i * 2 + 1]});
        }
    }

    vertexCount = static_cast<int>(indices.size());
    setupMesh(combinedData, indices, hasNormals, hasTextureCoords);
}

void Mesh::setupMesh(const vector<float> &combinedData, const vector<unsigned int> &indices,
                     bool hasNormals, bool hasTextureCoords)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, combinedData.size() * sizeof(float), combinedData.data(), GL_STATIC_DRAW);

    // Le stride reflète la présence ou l'absence des données de normale et de texture
    size_t stride = 3 * sizeof(float); // Composants de position
    if (hasNormals)
        stride += 3 * sizeof(float); // Composants de normale
    if (hasTextureCoords)
        stride += 2 * sizeof(float); // Composants de texture

    // Configuration des attributs de vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);

    if (hasNormals)
    {// Configuration des attributs de normale
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    if (hasTextureCoords)
    { // Configuration des attributs de texture
        size_t textureOffset = hasNormals ? 6 * sizeof(float) : 3 * sizeof(float);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)textureOffset);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete texture;
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (texture->hasTexture)
        texture->unbind();
}