/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgam.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
    string vertexCode = readShaderCodeFromFile(vertexPath);
    string fragmentCode = readShaderCodeFromFile(fragmentPath);

    GLuint vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShaderID);
    glAttachShader(ID, fragmentShaderID);
    linkProgram(vertexShaderID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(ID);
}

void ShaderProgram::use() const {
    glUseProgram(ID);
}

void ShaderProgram::setUniform(const string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setUniform(const string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setUniform(const string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::setUniform(const std::string& name, const Mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.m);
}


string ShaderProgram::readShaderCodeFromFile(const char* shaderPath) {
    ifstream shaderFile(shaderPath);
    stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}

GLuint ShaderProgram::compileShader(GLenum type, const string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
    }

    return shader;
}

void ShaderProgram::linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
}
