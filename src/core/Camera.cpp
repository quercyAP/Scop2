/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(Vec3 position, Vec3 up, float yaw, float pitch)
    : front(Vec3(0.0f, 0.0f, 1.0f)), movementSpeed(20.0f), mouseSensitivity(0.1f), zoom(45.0f)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::processKeyboardMovement(bool forward, bool backward, bool left, bool right, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (forward)
        position = position + front * velocity;
    if (backward)
        position = position - front * velocity;
    if (left)
        position = position - this->right * velocity;
    if (right)
        position = position + this->right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw -= xoffset;
    pitch -= yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}

Mat4 Camera::getViewMatrix() const
{
    return Mat4::lookAt(position, position + front, up);
}

Mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return Mat4::perspective(zoom, aspectRatio, 0.1f, 1000.0f);
}

void Camera::updateCameraVectors()
{
    Vec3 newFront;
    newFront.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
    newFront.y = sin(toRadians(pitch));
    newFront.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
    front = newFront.normalize();

    right = Vec3::cross(front, worldUp).normalize();
    up = Vec3::cross(right, front).normalize();
}

float Camera::toRadians(float degrees) const
{
    return degrees * M_PI / 180.0f;
}

void Camera::processPanMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    Vec3 panRight = right * xoffset;
    Vec3 panUp = up * yoffset;

    position = position - panRight - panUp;
}