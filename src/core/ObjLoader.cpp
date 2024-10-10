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

    bool hasUVs = false;
    // Parsing réel des données
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
            hasUVs = true;
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

    if (!hasUVs)
    {
        cout << "texture coords not found, generating default UVs" << endl;
        Vec3 minBounds, maxBounds;
        calculateBoundingBox(minBounds, maxBounds);
        generateUVsFromBoundingBox3D(minBounds, maxBounds);
    }

    return true;
}

void ObjLoader::parseUseMtl(istringstream &iss)
{
    iss >> currentMaterialName;
}

void ObjLoader::parseVertex(istringstream &iss)
{
    float x, y, z;
    iss >> x >> y >> z;
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
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
        {
            face.textureIndices.push_back(textureIndex);
        } else {
            face.textureIndices.push_back(vertexIndex);
        }
        if (normalIndex != -1)
            face.normalIndices.push_back(normalIndex);
    }
    faces.push_back(face);
}

Vec3 ObjLoader::calculateCenter()
{
    Vec3 sum(0.0f, 0.0f, 0.0f);
    size_t vertexCount = vertices.size() / 3;

    for (size_t i = 0; i < vertices.size(); i += 3)
    {
        sum.x += vertices[i];
        sum.y += vertices[i + 1];
        sum.z += vertices[i + 2];
    }

    return sum / static_cast<float>(vertexCount);
}

Vec3 ObjLoader::calculateCenterSampled(int sampleRate)
{
    Vec3 sum(0.0f, 0.0f, 0.0f);
    size_t count = 0;

    for (size_t i = 0; i < vertices.size(); i += 3 * sampleRate)
    {
        sum.x += vertices[i];
        sum.y += vertices[i + 1];
        sum.z += vertices[i + 2];
        ++count;
    }

    return count > 0 ? sum / static_cast<float>(count) : Vec3(0.0f, 0.0f, 0.0f);
}

void ObjLoader::adjustVerticesToCenter(const Vec3 &center)
{
    for (size_t i = 0; i < vertices.size(); i += 3)
    {
        vertices[i] -= center.x;
        vertices[i + 1] -= center.y;
        vertices[i + 2] -= center.z;
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

void ObjLoader::processVertex(const Face &face, int index, vector<float> &flatVertices, vector<float> &flatNormals, vector<float> &flatTextures, const vector<Vec3> &vertexNormals)
{
    int vertexIndex = (face.vertexIndices[index] - 1) * 3;
    flatVertices.push_back(vertices[vertexIndex]);
    flatVertices.push_back(vertices[vertexIndex + 1]);
    flatVertices.push_back(vertices[vertexIndex + 2]);

    flatNormals.push_back(vertexNormals[vertexIndex / 3].x);
    flatNormals.push_back(vertexNormals[vertexIndex / 3].y);
    flatNormals.push_back(vertexNormals[vertexIndex / 3].z);

    int textureIndex = face.textureIndices[index] - 1;
    const TextureCoord &tc = textureCoords[textureIndex];
    flatTextures.push_back(tc.u);
    flatTextures.push_back(tc.v);
}

void ObjLoader::initializeVertexNormals(vector<Vec3> &vertexNormals)
{
    if (vertexNormals.empty())
    {
        vertexNormals.resize(vertices.size() / 3, Vec3(0, 0, 0));
    }

    // Calcul des normales
    for (const auto &face : faces)
    {
        Vec3 normal = calculateFaceNormal(face.vertexIndices);

        for (int vertexIndex : face.vertexIndices)
        {
            vertexIndex = (vertexIndex - 1) * 3;
            vertexNormals[vertexIndex / 3] += normal;
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
    Vec3 v0 = Vec3(vertices[(vertexIndices[0] - 1) * 3], vertices[(vertexIndices[0] - 1) * 3 + 1], vertices[(vertexIndices[0] - 1) * 3 + 2]);
    Vec3 v1 = Vec3(vertices[(vertexIndices[1] - 1) * 3], vertices[(vertexIndices[1] - 1) * 3 + 1], vertices[(vertexIndices[1] - 1) * 3 + 2]);
    Vec3 v2 = Vec3(vertices[(vertexIndices[2] - 1) * 3], vertices[(vertexIndices[2] - 1) * 3 + 1], vertices[(vertexIndices[2] - 1) * 3 + 2]);

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    return Vec3::cross(edge1, edge2).normalize();
}

bool ObjLoader::isNormalInverted(const Vec3 &normal, const Vec3 &vertexPosition, const Vec3 &center)
{
    Vec3 toVertex = vertexPosition - center;
    return Vec3::dot(normal, toVertex) < 0;
}

void ObjLoader::calculateBoundingBox(Vec3 &minBounds, Vec3 &maxBounds)
{
    // Initialiser minBounds et maxBounds avec des valeurs extrêmes
    minBounds = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    maxBounds = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    // Parcourir tous les sommets pour trouver les valeurs minimales et maximales
    for (size_t i = 0; i < vertices.size(); i += 3)
    {
        // Comparer chaque coordonnée x, y, z avec les bornes actuelles et mettre à jour si nécessaire
        minBounds.x = std::min(minBounds.x, vertices[i]);     // Coordonnée x
        minBounds.y = std::min(minBounds.y, vertices[i + 1]); // Coordonnée y
        minBounds.z = std::min(minBounds.z, vertices[i + 2]); // Coordonnée z

        maxBounds.x = std::max(maxBounds.x, vertices[i]);     // Coordonnée x
        maxBounds.y = std::max(maxBounds.y, vertices[i + 1]); // Coordonnée y
        maxBounds.z = std::max(maxBounds.z, vertices[i + 2]); // Coordonnée z
    }
}

void ObjLoader::generateDefaultUVs()
{
    // Si nous avons des sommets mais pas de coordonnées UV, les générer
    Vec3 minBounds, maxBounds;
    calculateBoundingBox(minBounds, maxBounds);

    // Calculer les facteurs d'échelle pour normaliser les coordonnées UV
    float scaleX = 1.0f / (maxBounds.x - minBounds.x);
    float scaleY = 1.0f / (maxBounds.y - minBounds.y);

    // Générer des UV pour chaque sommet
    for (size_t i = 0; i < vertices.size(); i += 3) {
        float u = (vertices[i] - minBounds.x) * scaleX;     // Coordonnée x normalisée
        float v = (vertices[i + 1] - minBounds.y) * scaleY; // Coordonnée y normalisée

        // Ajouter les coordonnées UV générées
        textureCoords.push_back(TextureCoord{u, v});
    }
}

void ObjLoader::generateUVsFromBoundingBox3D(const Vec3& minBounds, const Vec3& maxBounds)
{
    float scaleX = 1.0f / (maxBounds.x - minBounds.x);
    float scaleY = 1.0f / (maxBounds.y - minBounds.y);
    float scaleZ = 1.0f / (maxBounds.z - minBounds.z);

    for (size_t i = 0; i < vertices.size(); i += 3)
    {
        float u, v;

        // Choisir le plan de projection en fonction de la taille de la bounding box
        if ((maxBounds.x - minBounds.x) >= (maxBounds.y - minBounds.y) &&
            (maxBounds.x - minBounds.x) >= (maxBounds.z - minBounds.z)) {
            // Projeter sur le plan YZ
            u = (vertices[i + 1] - minBounds.y) * scaleY;  // Coordonnée y normalisée
            v = (vertices[i + 2] - minBounds.z) * scaleZ;  // Coordonnée z normalisée
        }
        else if ((maxBounds.y - minBounds.y) >= (maxBounds.z - minBounds.z)) {
            // Projeter sur le plan XZ
            u = (vertices[i] - minBounds.x) * scaleX;      // Coordonnée x normalisée
            v = (vertices[i + 2] - minBounds.z) * scaleZ;  // Coordonnée z normalisée
        }
        else {
            // Projeter sur le plan XY
            u = (vertices[i] - minBounds.x) * scaleX;      // Coordonnée x normalisée
            v = (vertices[i + 1] - minBounds.y) * scaleY;  // Coordonnée y normalisée
        }

        // Ajouter les coordonnées UV générées
        textureCoords.push_back(TextureCoord{u, v});
    }
}
