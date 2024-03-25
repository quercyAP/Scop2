/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
#define MESH_HPP

#include "define.hpp"
#include "Transform.hpp"
#include "Texture.hpp"

class Mesh {
public:
    Transform transform;
    const Material *material;
    Texture *texture;

    Mesh(const vector<float>& vertices,
         const vector<float>& normals,
         const vector<float>& textureCoords,
         const vector<unsigned int>& indices,
         const Material* material,
         const string& texturePath);
    ~Mesh();

    void draw() const;

private:
    GLuint VAO, VBO, EBO;
    int vertexCount;

    void setupMesh(const std::vector<float> &combinedData, const std::vector<unsigned int> &indices, bool hasNormals, bool hasTextureCoords);

};

#endif 

