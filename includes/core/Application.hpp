/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "ShaderProgram.hpp"
#include "ObjLoader.hpp"
#include "define.hpp"
#include "Camera.hpp"

class Application {
public:
    Application(const string &objPath, const string &texturePath);
    ~Application();

    void run();

private:
    GLFWwindow* window;
    ShaderProgram* shader;
    ObjLoader objLoader;
    Camera camera;
    Mesh *mesh;
    float deltaTime;
    bool firstMouse = true;
    float lastX = 800 / 2.0f;
    float lastY = 600 / 2.0f;
    bool isRotationMode;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void processInput();
    void render();

    void initWindow();
    void initOpenGL();

};

#endif 
