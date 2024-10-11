/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vec3.hpp"
#include "Mat4.hpp"
#include "define.hpp"

class Camera {
public:
    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right;
    Vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(Vec3 position, Vec3 up, float yaw, float pitch);

    void processKeyboardMovement(bool forward, bool backward, bool left, bool right, float deltaTime);

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch);

    void processMouseScroll(float yoffset);

    Mat4 getViewMatrix() const;

    Mat4 getProjectionMatrix(float aspectRatio) const;

    void processPanMovement(float xoffset, float yoffset);
    
private:
    void updateCameraVectors();

    float toRadians(float degrees) const;

};

#endif

