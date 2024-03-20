/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vec3.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>

class Vec3
{
public:
    float x, y, z;

    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator+(const Vec3 &other) const;
    Vec3 operator-(const Vec3 &other) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;
    Vec3 operator-() const;
    Vec3 operator+=(const Vec3 &other);
    Vec3 operator/=(float scalar);

    Vec3 &normalize();

    float length() const;

    // Produit scalaire
    static float dot(const Vec3 &a, const Vec3 &b);

    // Produit vectoriel
    static Vec3 cross(const Vec3 &a, const Vec3 &b);
};

#endif
