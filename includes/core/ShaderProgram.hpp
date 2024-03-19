/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgam.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "define.hpp"
#include "Mat4.hpp"

class ShaderProgram {
public:
    GLuint ID;

    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();

    void use() const;
    void setUniform(const string& name, float value) const;
    void setUniform(const string& name, int value) const;
    void setUniform(const string& name, bool value) const;
    void setUniform(const std::string& name, const Mat4& mat) const;

private:
    string readShaderCodeFromFile(const char* shaderPath);
    GLuint compileShader(GLenum type, const string& source);
    void linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
};

#endif
