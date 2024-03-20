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

void Transform::calculateCenter(const std::vector<Vertex>& vertices) {
    Vec3 sum(0, 0, 0);
    for (auto& vertex : vertices) {
        sum += Vec3(vertex.x, vertex.y, vertex.z);
    }
    center = sum / vertices.size();
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