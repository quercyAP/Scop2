/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjLoader.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "define.hpp"
#include "Mesh.hpp"

struct Vertex {
    float x, y, z;
};

struct Normal {
    float nx, ny, nz;
};

struct TextureCoord {
    float u, v;
};

struct Face {
    vector<int> vertexIndices;
    vector<int> textureIndices;
    vector<int> normalIndices;
};

class ObjLoader {
public:
    vector<Vertex> vertices;
    vector<Normal> normals;
    vector<TextureCoord> textureCoords;
    vector<Face> faces;

    bool loadObj(const string& path);
    Mesh createMesh();

private:
    void processVertex(const Face &face, int index, vector<float> &flatVertices, vector<float> &flatNormals, vector<float> &flatTextures);
    void parseVertex(istringstream& iss);
    void parseNormal(istringstream& iss);
    void parseTextureCoord(istringstream& iss);
    void parseFace(istringstream& iss);
};

#endif
