/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ObjLoader.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ObjLoader.hpp"

bool ObjLoader::loadObj(const string &path)
{
    ifstream file(path);
    string line;
    if (!file.is_open())
    {
        cerr << "Unable to open OBJ file: " << path << endl;
        return false;
    }

    while (getline(file, line))
    {
        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            parseVertex(iss);
        }
        else if (prefix == "vt")
        {
            parseTextureCoord(iss);
        }
        else if (prefix == "vn")
        {
            parseNormal(iss);
        }
        else if (prefix == "f")
        {
            parseFace(iss);
        }
    }

    return true;
}

void ObjLoader::parseVertex(istringstream &iss)
{
    Vertex v;
    iss >> v.x >> v.y >> v.z;
    vertices.push_back(v);
}

void ObjLoader::parseNormal(istringstream &iss)
{
    Normal n;
    iss >> n.nx >> n.ny >> n.nz;
    normals.push_back(n);
}

void ObjLoader::parseTextureCoord(istringstream &iss)
{
    TextureCoord tc;
    iss >> tc.u >> tc.v;
    textureCoords.push_back(tc);
}

void ObjLoader::parseFace(istringstream &iss)
{
    Face face;
    string vertexSpec;
    while (iss >> vertexSpec)
    {
        istringstream vertexStream(vertexSpec);
        string part;
        int vertexIndex = -1, textureIndex = -1, normalIndex = -1;

        // Lire l'indice du sommet
        getline(vertexStream, part, '/');
        if (!part.empty())
            vertexIndex = stoi(part);

        // Vérifier s'il y a des coordonnées de texture ou des indices normaux à lire
        if (!vertexStream.eof())
        {
            getline(vertexStream, part, '/');
            if (!part.empty())
                textureIndex = stoi(part);
        }

        // Vérifier s'il y a des indices normaux à lire
        if (!vertexStream.eof())
        {
            getline(vertexStream, part);
            if (!part.empty())
                normalIndex = stoi(part);
        }

        // Ajouter les indices à la face. Notez que -1 est utilisé pour indiquer l'absence d'un indice.
        if (vertexIndex != -1)
            face.vertexIndices.push_back(vertexIndex);
        if (textureIndex != -1)
            face.textureIndices.push_back(textureIndex);
        if (normalIndex != -1)
            face.normalIndices.push_back(normalIndex);
    }
    faces.push_back(face);
}

Mesh ObjLoader::createMesh()
{
    vector<float> flatVertices;
    vector<float> flatNormals;
    vector<float> flatTextures;
    vector<unsigned int> indices;

    for (const auto &face : faces)
    {
        if (face.vertexIndices.size() == 3)
        { // Gestion des triangles
            for (int i = 0; i < 3; ++i)
            {
                processVertex(face, i, flatVertices, flatNormals, flatTextures);
            }
        }
        else if (face.vertexIndices.size() == 4)
        { // Gestion des quads
            // Diviser le quad en deux triangles (0, 1, 2) et (0, 2, 3)
            for (int i = 0; i < 3; ++i)
            { // Premier triangle
                processVertex(face, i, flatVertices, flatNormals, flatTextures);
            }
            for (int i : {0, 2, 3})
            { // Deuxième triangle
                processVertex(face, i, flatVertices, flatNormals, flatTextures);
            }
        }
        // Extension possible pour des polygones à plus de 4 sommets
    }

    // Création du Mesh avec les données aplaties
    for (size_t i = 0; i < flatVertices.size() / 3; ++i)
    {
        indices.push_back(i);
    }
    return Mesh(flatVertices, flatNormals, flatTextures, indices);
}

void ObjLoader::processVertex(const Face &face, int index, vector<float> &flatVertices, vector<float> &flatNormals, vector<float> &flatTextures)
{
    int vertexIndex = face.vertexIndices[index] - 1;
    flatVertices.push_back(vertices[vertexIndex].x);
    flatVertices.push_back(vertices[vertexIndex].y);
    flatVertices.push_back(vertices[vertexIndex].z);

    if (!face.textureIndices.empty())
    {
        int texIndex = face.textureIndices[index] - 1; // Les coordonnées de texture sont optionnelles
        flatTextures.push_back(textureCoords[texIndex].u);
        flatTextures.push_back(textureCoords[texIndex].v);
    }

    if (!face.normalIndices.empty())
    {
        int normalIndex = face.normalIndices[index] - 1; // Les normales sont optionnelles
        flatNormals.push_back(normals[normalIndex].nx);
        flatNormals.push_back(normals[normalIndex].ny);
        flatNormals.push_back(normals[normalIndex].nz);
    }
}