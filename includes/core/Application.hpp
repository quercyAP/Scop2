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
    float lastX = 800 / 2.0f;
    float lastY = 600 / 2.0f;

    bool firstMouse;
    float mouseLastX;
    float mouseLastY;
    bool isMiddleMousePressed;
    bool isPanning;

    bool isRotationMode;
    float textureMixFactor = 0.0f;
    bool textureEnabled = false;
    bool faceMode = false;

    Vec3 lightPos = Vec3(1.2f, 1.0f, 2.0f);
    Vec3 lightColor = Vec3(1.0f, 1.0f, 1.0f);
    Vec3 defaultAmbient = Vec3(0.5f, 0.5f, 0.5f);
    Vec3 defaultDiffuse = Vec3(0.7f, 0.7f, 0.7f);
    Vec3 defaultSpecular = Vec3(1.0f, 1.0f, 1.0f);
    float defaultShininess = 32.0f;

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void handleMouseMovement(double xpos, double ypos);
    void handleMouseButton(int button, int action, int mods);
    void handleMouseScroll(double yoffset);
    void processInput();
    void render();

    void initWindow();
    void initOpenGL();

};

#endif 
