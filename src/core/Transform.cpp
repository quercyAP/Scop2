/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transform.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Transform.hpp"

Transform::Transform() : position(Vec3(0, 0, 0)), rotation(Vec3(0, 0, 0)), scale(Vec3(1, 1, 1)) {}

void Transform::calculateCenter(const vector<float>& vertices) {
    Vec3 sum(0, 0, 0);
    size_t vertexCount = vertices.size() / 3; // 3 floats par sommet (x, y, z)

    // Parcourir chaque sommet
    for (size_t i = 0; i < vertices.size(); i += 3) {
        sum.x += vertices[i];     // Coordonnée x
        sum.y += vertices[i + 1]; // Coordonnée y
        sum.z += vertices[i + 2]; // Coordonnée z
    }

    // Calculer le centre
    if (vertexCount > 0) {
        center = sum / static_cast<float>(vertexCount);
    } else {
        center = Vec3(0, 0, 0); // Retourner un centre nul si aucun sommet n'est présent
    }
}

void Transform::calculateCenter(const vector<Vertex>& vertices, int sampleRate) {
    Vec3 sum(0, 0, 0);
    size_t count = 0;

    // Échantillonner un sommet tous les "sampleRate" sommets (chaque sommet a 3 floats)
    for (size_t i = 0; i < vertices.size(); i += 3 * sampleRate) {
        sum.x += vertices[i].x;     // Coordonnée x
        sum.y += vertices[i].y; // Coordonnée y
        sum.z += vertices[i].z; // Coordonnée z
        ++count;
    }

    // Si aucun sommet n'a été échantillonné, éviter la division par zéro
    if (count > 0) {
        center = sum / static_cast<float>(count);
    } else {
        center = Vec3(0, 0, 0); // Valeur par défaut si aucune donnée
    }
}


Mat4 Transform::getTransformationMatrix() const {
    // Première translation pour recentrer l'objet à son origine
    Mat4 initialTranslation = Mat4::translate(-center);
    // Rotation autour de l'origine (centre initial de l'objet)
    Mat4 rotXMatrix = Mat4::rotateX(rotation.x);
    Mat4 rotYMatrix = Mat4::rotateY(rotation.y);
    Mat4 rotZMatrix = Mat4::rotateZ(rotation.z);
    // Seconde translation pour déplacer l'objet à sa position dans la scène
    Mat4 finalTranslation = Mat4::translate(position);
    // Mise à l'échelle
    Mat4 scaleMatrix = Mat4::scale(scale);

    // Combinez les transformations dans le bon ordre
    return finalTranslation * rotZMatrix * rotYMatrix * rotXMatrix * scaleMatrix * initialTranslation;
}