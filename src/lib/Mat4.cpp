/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mat4.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mat4.hpp"

Mat4::Mat4()
{
    std::memset(m, 0, sizeof(m));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.m[i * 4 + j] =
                m[i * 4] * other.m[j] +
                m[i * 4 + 1] * other.m[j + 4] +
                m[i * 4 + 2] * other.m[j + 8] +
                m[i * 4 + 3] * other.m[j + 12];
        }
    }
    return result;
}

Mat4 Mat4::translate(const Vec3 &vec)
{
    Mat4 result;
    result.m[12] = vec.x;
    result.m[13] = vec.y;
    result.m[14] = vec.z;
    return result;
}

Mat4 Mat4::rotateY(float angle)
{
    Mat4 result;
    float rad = angle * M_PI / 180.0f;
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    result.m[0] = cosA;
    result.m[2] = -sinA;
    result.m[8] = sinA;
    result.m[10] = cosA;

    return result;
}

Mat4 Mat4::rotateX(float angle) {
    Mat4 result;
    float rad = angle * M_PI / 180.0f;
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    result.m[5] = cosA;
    result.m[6] = sinA;
    result.m[9] = -sinA;
    result.m[10] = cosA;

    return result;
}

Mat4 Mat4::rotateZ(float angle) {
    Mat4 result;
    float rad = angle * M_PI / 180.0f;
    float cosA = std::cos(rad);
    float sinA = std::sin(rad);

    result.m[0] = cosA;
    result.m[1] = sinA;
    result.m[4] = -sinA;
    result.m[5] = cosA;

    return result;
}

Mat4 Mat4::scale(const Vec3 &vec)
{
    Mat4 result;
    result.m[0] = vec.x;
    result.m[5] = vec.y;
    result.m[10] = vec.z;
    return result;
}

Mat4 Mat4::perspective(float fov, float aspect, float near, float far)
{
    Mat4 result;
    float tanHalfFOV = std::tan(fov / 2.0f);
    float zRange = near - far;

    result.m[0] = 1.0f / (aspect * tanHalfFOV);
    result.m[5] = 1.0f / tanHalfFOV;
    result.m[10] = (-near - far) / zRange;
    result.m[11] = 1.0f;
    result.m[14] = 2.0f * far * near / zRange;
    result.m[15] = 0.0f;

    return result;
}

Mat4 Mat4::lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    Vec3 f = (center - eye).normalize(); // Direction de vue
    Vec3 s = Vec3::cross(f, up).normalize(); // Vecteur droit
    Vec3 u = Vec3::cross(s, f); // Vecteur haut réel

    Mat4 result;
    result.m[0] = s.x;
    result.m[4] = s.y;
    result.m[8] = s.z;

    result.m[1] = u.x;
    result.m[5] = u.y;
    result.m[9] = u.z;

    result.m[2] = -f.x;
    result.m[6] = -f.y;
    result.m[10] = -f.z;

    result.m[12] = -Vec3::dot(s, eye);
    result.m[13] = -Vec3::dot(u, eye);
    result.m[14] = Vec3::dot(f, eye);
    result.m[15] = 1.0f;

    return result;
}