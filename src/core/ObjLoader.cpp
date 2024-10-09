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
        else if (prefix == "usemtl")
        {
            parseUseMtl(iss);
        }
        else if (prefix == "mtllib")
        {
            string mtlFilePath;
            iss >> mtlFilePath;
            if (!materialManager.loadMaterials("assets/" + mtlFilePath))
            {
                cerr << "Failed to load MTL file: " << mtlFilePath << endl;
                return false;
            }
        }
    }

    return true;
}

void ObjLoader::parseUseMtl(istringstream &iss)
{
    iss >> currentMaterialName; 
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

Vec3 ObjLoader::calculateCenter()
{
    Vec3 sum(0.0f, 0.0f, 0.0f);
    for (auto &vertex : vertices)
    {
        sum += Vec3(vertex.x, vertex.y, vertex.z);
    }
    return sum / vertices.size();
}

Vec3 ObjLoader::calculateCenterSampled(int sampleRate)
{
    Vec3 sum(0.0f, 0.0f, 0.0f);
    size_t count = 0;

    for (size_t i = 0; i < vertices.size(); i += sampleRate)
    {
        const Vertex& vertex = vertices[i];
        sum += Vec3(vertex.x, vertex.y, vertex.z);
        ++count;
    }

    // Si aucun sommet n'a été échantillonné, retourner le centre d'origine
    if (count == 0)
        return Vec3(0.0f, 0.0f, 0.0f);
    return sum / static_cast<float>(count);
}

void ObjLoader::adjustVerticesToCenter(const Vec3 &center)
{
    for (auto &vertex : vertices)
    {
        vertex.x -= center.x;
        vertex.y -= center.y;
        vertex.z -= center.z;
    }
}

Mesh ObjLoader:: createMesh(const string &texturePath)
{
    vector<float> flatVertices;
    vector<float> flatNormals;
    vector<float> flatTextures;
    vector<unsigned int> indices;

    Vec3 center = calculateCenterSampled(100);
    adjustVerticesToCenter(center);

    vector<Vec3> vertexNormals;
    initializeVertexNormals(vertexNormals);

    for (const auto &face : faces)
    {
        if (face.vertexIndices.size() < 3)
        {
            cerr << "Face with less than 3 vertices detected. Skipping." << endl;
            continue;
        }
        if (face.vertexIndices.size() == 3)
        { // Gestion des triangles
            for (int i = 0; i < 3; ++i)
            {
                processVertex(face, i, flatVertices, flatNormals, flatTextures, vertexNormals);
            }
        }
        else if (face.vertexIndices.size() == 4)
        { // Gestion des quads
            // Diviser le quad en deux triangles (0, 1, 2) et (0, 2, 3)
            for (int i = 0; i < 3; ++i)
            { // Premier triangle
                processVertex(face, i, flatVertices, flatNormals, flatTextures, vertexNormals);
            }
            for (int i : {0, 2, 3})
            { // Deuxième triangle
                processVertex(face, i, flatVertices, flatNormals, flatTextures, vertexNormals);
            }
        }
        else
        { // Gestion des faces avec plus de 4 sommets
            cerr << "Face with more than 4 vertices detected. Skipping." << endl;
            continue;
        }
    }

    for (size_t i = 0; i < flatVertices.size() / 3; ++i)
    {
        indices.push_back(i);
    }
    return Mesh(flatVertices, flatNormals, flatTextures, indices, materialManager.getMaterial(currentMaterialName), texturePath);
}

void ObjLoader::processVertex(const Face &face, int index, vector<float> &flatVertices, vector<float> &flatNormals, vector<float> &flatTextures, const vector<Vec3> &vertexNormals)
{
    int vertexIndex = face.vertexIndices[index] - 1;
    const Vertex& v = vertices[vertexIndex];
    flatVertices.push_back(vertices[vertexIndex].x);
    flatVertices.push_back(vertices[vertexIndex].y);
    flatVertices.push_back(vertices[vertexIndex].z);

    flatNormals.push_back(vertexNormals[vertexIndex].x);
    flatNormals.push_back(vertexNormals[vertexIndex].y);
    flatNormals.push_back(vertexNormals[vertexIndex].z);

    if (!face.textureIndices.empty())
    {
        int texIndex = face.textureIndices[index] - 1;
        flatTextures.push_back(textureCoords[texIndex].u);
        flatTextures.push_back(textureCoords[texIndex].v);
    }
    else
    {
        flatTextures.push_back((v.x + 1.0f) * 0.5f);
        flatTextures.push_back((v.y + 1.0f) * 0.5f);
    }
}

void ObjLoader::initializeVertexNormals(vector<Vec3> &vertexNormals)
{
    vertexNormals.resize(vertices.size(), Vec3(0, 0, 0));
    for (const auto &face : faces)
    {
        Vec3 normal = calculateFaceNormal(face.vertexIndices);
        for (int vertexIndex : face.vertexIndices)
        {
            vertexNormals[vertexIndex - 1] += normal;
        }
    }
    for (Vec3 &normal : vertexNormals)
    {
        normal.normalize();
    }
}

Vec3 ObjLoader::calculateFaceNormal(const vector<int> &vertexIndices)
{
    if (vertexIndices.size() < 3)
        return Vec3(0, 0, 0);

    Vec3 v0 = vertices[vertexIndices[0] - 1].toVec3();
    Vec3 v1 = vertices[vertexIndices[1] - 1].toVec3();
    Vec3 v2 = vertices[vertexIndices[2] - 1].toVec3();

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    Vec3 normal = Vec3::cross(edge1, edge2).normalize();

    Vec3 center = calculateCenter();


    // Vérifier si la normale est inversée
    if (isNormalInverted(normal, v0, center))
    {
        return -normal;
    }

    return normal;
}
bool ObjLoader::isNormalInverted(const Vec3 &normal, const Vec3 &vertexPosition, const Vec3 &center)
{
    Vec3 toVertex = vertexPosition - center;
    return Vec3::dot(normal, toVertex) < 0;
}