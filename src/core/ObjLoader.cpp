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
    if (!file.is_open())
    {
        cerr << "Unable to open OBJ file: " << path << endl;
        return false;
    }

    size_t vertexCount = 0, textureCount = 0, normalCount = 0, faceCount = 0;
    string line;

    while (getline(file, line))
    {
        if (line.substr(0, 2) == "v ")
            vertexCount++;
        else if (line.substr(0, 2) == "vt")
            textureCount++;
        else if (line.substr(0, 2) == "vn")
            normalCount++;
        else if (line.substr(0, 2) == "f ")
            faceCount++;
    }

    file.clear();
    file.seekg(0, ios::beg);

    vertices.reserve(vertexCount);
    textureCoords.reserve(textureCount);
    normals.reserve(normalCount);
    faces.reserve(faceCount);

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

    if (textureCoords.empty())
    {
        cout << "texture coords not found, generating default UVs" << endl;
        generateSphericalUVs();
    }

    return true;
}

void ObjLoader::parseUseMtl(istringstream &iss)
{
    iss >> currentMaterialName;
}

void ObjLoader::parseVertex(istringstream &iss)
{
    Vertex vertex;
    iss >> vertex.x >> vertex.y >> vertex.z;
    vertices.push_back(vertex);
}

void ObjLoader::parseNormal(istringstream &iss)
{
    Normal n;
    iss >> n.nx >> n.ny >> n.nz;
    normals.push_back(n);
}

void ObjLoader::parseTextureCoord(istringstream &iss)
{
    float u, v;
    iss >> u >> v;
    textureCoords.push_back(TextureCoord{u, v});
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
    for (const auto &vertex : vertices)
    {
        sum.x += vertex.x;
        sum.y += vertex.y;
        sum.z += vertex.z;
    }
    return sum / static_cast<float>(vertices.size());
}

Vec3 ObjLoader::calculateCenterSampled(int sampleRate)
{
    Vec3 sum(0.0f, 0.0f, 0.0f);

    int count = 0;
    for (size_t i = 0; i < vertices.size(); i += sampleRate)
    {
        sum.x += vertices[i].x;
        sum.y += vertices[i].y;
        sum.z += vertices[i].z;
        count++;
    }
    return count > 0 ? sum / static_cast<float>(count) : Vec3(0.0f, 0.0f, 0.0f);
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

Mesh ObjLoader::createMesh(const string &texturePath)
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

void ObjLoader::processVertex(const Face &face, int index, vector<float> &flatVertices,
                              vector<float> &flatNormals, vector<float> &flatTextures,
                              const vector<Vec3> &vertexNormals)
{
    int vertexIndex = face.vertexIndices[index] - 1;
    const Vertex &v = vertices[vertexIndex];

    // Ajouter les positions du sommet
    flatVertices.push_back(v.x);
    flatVertices.push_back(v.y);
    flatVertices.push_back(v.z);

    // Ajouter les normales
    flatNormals.push_back(vertexNormals[vertexIndex].x);
    flatNormals.push_back(vertexNormals[vertexIndex].y);
    flatNormals.push_back(vertexNormals[vertexIndex].z);

    // Ajouter les UV
    if (!face.textureIndices.empty())
    {
        cout << "Face Texture Indice trouver" << endl;
        // Si les indices de texture sont présents dans la face, les utiliser
        int texIndex = face.textureIndices[index] - 1;
        const TextureCoord &tc = textureCoords[texIndex];
        flatTextures.push_back(tc.u);
        flatTextures.push_back(tc.v);
    }
    else if (!textureCoords.empty())
    {
        cout << "Texture Coords trouver" << endl;
        // Si les indices ne sont pas présents mais que les coordonnées de texture existent, les utiliser
        const TextureCoord &tc = textureCoords[vertexIndex];
        flatTextures.push_back(tc.u);
        flatTextures.push_back(tc.v);
    }
    else
    {
        cout << "Texture Coords generer" << endl;
        // Générer des UV par défaut si aucune coordonnée de texture n'est présente
        flatTextures.push_back(v.texX);
        flatTextures.push_back(v.texY);
    }
}

void ObjLoader::initializeVertexNormals(vector<Vec3> &vertexNormals)
{
    if (vertexNormals.empty())
    {
        vertexNormals.resize(vertices.size(), Vec3(0, 0, 0));
    }

    // Calcul des normales
    for (const auto &face : faces)
    {
        Vec3 normal = calculateFaceNormal(face.vertexIndices);

        for (int vertexIndex : face.vertexIndices)
        {
            vertexNormals[vertexIndex - 1] += normal;
        }
    }
    // Normalisation des normales
    for (Vec3 &normal : vertexNormals)
    {
        normal.normalize();
    }
}

Vec3 ObjLoader::calculateFaceNormal(const vector<int> &vertexIndices)
{
    const Vertex &v0 = vertices[vertexIndices[0] - 1];
    const Vertex &v1 = vertices[vertexIndices[1] - 1];
    const Vertex &v2 = vertices[vertexIndices[2] - 1];

    Vec3 edge1 = v1.toVec3() - v0.toVec3();
    Vec3 edge2 = v2.toVec3() - v0.toVec3();

    return Vec3::cross(edge1, edge2).normalize();
}

bool ObjLoader::isNormalInverted(const Vec3 &normal, const Vec3 &vertexPosition, const Vec3 &center)
{
    Vec3 toVertex = vertexPosition - center;
    return Vec3::dot(normal, toVertex) < 0;
}

void ObjLoader::generateSphericalUVs()
{
    const float twoPi = 2.0f * M_PI;

    for (auto &vertex : vertices)
    {
        // Normalisation du vecteur de position du sommet
        float length = sqrt(vertex.x * vertex.x + vertex.y * vertex.y + vertex.z * vertex.z);
        if (length == 0) continue; // Éviter la division par zéro

        float normX = vertex.x / length;
        float normY = vertex.y / length;
        float normZ = vertex.z / length;

        // Calcul de theta et phi
        float theta = atan2(normZ, normX);
        float phi = acos(normY); // normY est déjà normalisé, pas besoin de diviser par le rayon

        // Génération des coordonnées UV sphériques
        vertex.texX = (theta + M_PI) / twoPi; // Mapping de theta à [0, 1]
        vertex.texY = phi / M_PI;             // Mapping de phi à [0, 1]

        // Correction des UV pour éviter la couture visible
        if (vertex.texX < 0.0f) vertex.texX += 1.0f;
        else if (vertex.texX > 1.0f) vertex.texX -= 1.0f;
    }
}
