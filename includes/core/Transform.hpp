/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transform.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "define.hpp"

class Transform {
public:
    Vec3 position;
    Vec3 rotation; 
    Vec3 scale;
    Vec3 center;

    Transform();

    Mat4 getTransformationMatrix() const;
    void calculateCenter(const vector<float>& vertices);
    void calculateCenter(const vector<float>& vertices, int sampleRate);
};

#endif 
