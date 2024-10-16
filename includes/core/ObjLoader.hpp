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
#include "MaterialManager.hpp"

struct Normal
{
    float nx, ny, nz;
};

struct TextureCoord
{
    float u, v;
};

struct Face
{
    vector<int> vertexIndices;
    vector<int> textureIndices;
    vector<int> normalIndices;
};

class ObjLoader
{
public:
    vector<Normal> normals;
    vector<TextureCoord> textureCoords;
    vector<Face> faces;
    MaterialManager materialManager;
    string currentMaterialName;
    vector<Vertex> vertices;


    bool loadObj(const string &path);
    Mesh createMesh(const string &texturePath);

private:
    void processVertex(const Face &face, int index, vector<float> &flatVertices, vector<float> &flatNormals, vector<float> &flatTextures, const vector<Vec3> &vertexNormals);
    void parseVertex(istringstream &iss);
    void parseNormal(istringstream &iss);
    void parseTextureCoord(istringstream &iss);
    void parseFace(istringstream &iss);
    void parseUseMtl(istringstream& iss);
    Vec3 calculateCenter();
    void generateSphericalUVs();
    Vec3 calculateCenterSampled(int sampleRate);
    void adjustVerticesToCenter(const Vec3 &center);
    void initializeVertexNormals(vector<Vec3> &vertexNormals);
    Vec3 calculateFaceNormal(const vector<int> &vertexIndices);
    void calculateBoundingBox(Vec3 &minBounds, Vec3 &maxBounds);
    bool isNormalInverted(const Vec3 &normal, const Vec3 &vertexPosition, const Vec3 &center);
    void generateUVsBasedOnBoundingBox(const Vec3& minBounds, const Vec3& maxBounds);
    void generateUVsFromBoundingBox3D(const Vec3& minBounds, const Vec3& maxBounds);
};

#endif