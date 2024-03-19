/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat4.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT4_HPP
#define MAT4_HPP

#include <cstring>
#include "Vec3.hpp"

class Mat4 {
public:
    float m[16];

    // Constructeur par défaut - identité
    Mat4();

    // Multiplication de matrices
    Mat4 operator*(const Mat4& other) const;

    // Génère une matrice de translation
    static Mat4 translate(const Vec3& vec);

    // Génère une matrice de rotation autour de l'axe Y
    static Mat4 rotateY(float angle);

    // Génère une matrice de rotation autour de l'axe X
    static Mat4 rotateX(float angle);

    // Génère une matrice de rotation autour de l'axe Z
    static Mat4 rotateZ(float angle);

    // Génère une matrice de mise à l'échelle
    static Mat4 scale(const Vec3& vec);

    // Génère une matrice de perspective
    static Mat4 perspective(float fov, float aspect, float near, float far);

    // Génère une matrice de vue
    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
};

#endif
