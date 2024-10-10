/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MaterialManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MaterialManager.hpp"

bool MaterialManager::loadMaterials(const string& mtlFilePath) {
    ifstream file(mtlFilePath);
    if (!file.is_open()) {
        cerr << "Failed to open MTL file: " << mtlFilePath << endl;
        return false;
    }

    parseMtlFile(file);
    file.close();
    return true;
}

const Material* MaterialManager::getMaterial(const string& name) const {
    auto it = materials.find(name);
    if (it != materials.end()) {
        return &it->second;
    }
    return nullptr;
}

void MaterialManager::parseMtlFile(ifstream& file) {
    string line;
    Material currentMaterial;
    string materialName;  

    while (getline(file, line)) {
        istringstream lineStream(line);
        string prefix;
        lineStream >> prefix;

        if (prefix == "newmtl") {
            if (!materialName.empty()) {
                materials[materialName] = currentMaterial;
                currentMaterial = Material();
            }
            lineStream >> materialName;
        } else if (prefix == "Ka") {
            lineStream >> currentMaterial.ambient.x >> currentMaterial.ambient.y >> currentMaterial.ambient.z;
        } else if (prefix == "Kd") {
            lineStream >> currentMaterial.diffuse.x >> currentMaterial.diffuse.y >> currentMaterial.diffuse.z;
        } else if (prefix == "Ks") {
            lineStream >> currentMaterial.specular.x >> currentMaterial.specular.y >> currentMaterial.specular.z;
        } else if (prefix == "Ns") {
            lineStream >> currentMaterial.shininess;
        }
    }

    if (!materialName.empty()) {
        materials[materialName] = currentMaterial;
    }
}
