/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Application.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Application.hpp"

Application::Application(const string &objPath, const string &texturePath) : mesh(nullptr), shader(nullptr), window(nullptr),
                             deltaTime(0.0f), camera(Vec3(5.0f, 0.0f, 5.0f), Vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.0f),
                             isRotationMode(false)
{
    initWindow();
    initOpenGL();

    if (objLoader.loadObj(objPath) == false)
    {
        cerr << "Failed to load OBJ file" << endl;
        exit(-1);
    }
    mesh = new Mesh(objLoader.createMesh(texturePath));
    mesh->transform.calculateCenter(objLoader.vertices);
    shader = new ShaderProgram("shader/shader.vert", "shader/shader.frag");
}

Application::~Application()
{
    glfwTerminate();
    delete shader;
    delete mesh;
}

void Application::run()
{
    float lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        processInput();
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Scop", nullptr, nullptr);
    if (window == nullptr)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void Application::initOpenGL()
{
    glEnable(GL_DEPTH_TEST);
}

void Application::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Gestion des mouvements de la caméra
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardMovement(true, false, false, false, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardMovement(false, true, false, false, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardMovement(false, false, true, false, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardMovement(false, false, false, true, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        isRotationMode = !isRotationMode; // Toggle rotation mode
        usleep(100000);
    }

    if (isRotationMode)
    {
        // Gestion de la rotation du Mesh actuellement sélectionné
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            mesh->transform.rotation.x += 1.0f; // Pivoter vers le haut
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            mesh->transform.rotation.x -= 1.0f; // Pivoter vers le bas
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            mesh->transform.rotation.y += 1.0f; // Pivoter vers la gauche
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            mesh->transform.rotation.y -= 1.0f; // Pivoter vers la droite
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
            mesh->transform.rotation.z -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
            mesh->transform.rotation.z += 1.0f;
    }
    else
    {
        // Gestion des mouvements du Mesh actuellement sélectionné
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            mesh->transform.center += Vec3(0.1f, 0.0f, 0.0f); // Droite
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            mesh->transform.center += Vec3(-0.1f, 0.0f, 0.0f); // Gauche
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            mesh->transform.center += Vec3(0.0f, 0.0f, -0.1f); // Avancer
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            mesh->transform.center += Vec3(0.0f, 0.0f, 0.1f); // Reculer
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
            mesh->transform.center += Vec3(0.0f, -0.1f, 0.0f); // Monter
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
            mesh->transform.center += Vec3(0.0f, 0.1f, 0.0f); // Descendre
    }

    // Gestion de la texture
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        textureEnabled = !textureEnabled;
        usleep(100000);
    }

    // Face mode
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        faceMode = !faceMode;
        usleep(100000);
    }
}

void Application::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Mat4 modelMatrix = mesh->transform.getTransformationMatrix();

    shader->use();
    shader->setUniform("model", modelMatrix);
    shader->setUniform("view", camera.getViewMatrix());
    shader->setUniform("projection", camera.getProjectionMatrix(800.0f / 600.0f));
    shader->setUniform("light.position", lightPos);
    shader->setUniform("light.color", lightColor);

    shader->setUniform("faceMode", faceMode);

    const float transitionSpeed = 0.01f;

    if (textureEnabled)
    {
        textureMixFactor += transitionSpeed;
        if (textureMixFactor > 1.0f)
            textureMixFactor = 1.0f;
    }
    else
    {
        textureMixFactor -= transitionSpeed;
        if (textureMixFactor < 0.0f)
            textureMixFactor = 0.0f;
    }

    shader->setUniform("textureMixFactor", textureMixFactor);

    bool hasTexture = mesh->texture && mesh->texture->hasTexture;
    shader->setUniform("material.hasTexture", hasTexture);

    if (hasTexture)
    {
        mesh->texture->bind(0);
        shader->setUniform("texture_diffuse", 0);
    }

    if (mesh->material != nullptr)
    {
        shader->setUniform("material.ambient", mesh->material->ambient);
        shader->setUniform("material.diffuse", mesh->material->diffuse);
        shader->setUniform("material.specular", mesh->material->specular);
        shader->setUniform("material.shininess", mesh->material->shininess);
    }
    else
    {
        shader->setUniform("material.ambient", defaultAmbient);
        shader->setUniform("material.diffuse", defaultDiffuse);
        shader->setUniform("material.specular", defaultSpecular);
        shader->setUniform("material.shininess", defaultShininess);
    }

    mesh->draw();
}

