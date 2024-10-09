/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Texture.hpp"
Texture::Texture(const std::string &filePath) : textureID(0) {
    if (filePath.empty()) {
        hasTexture = false;
        return;
    }
    loadBMP(filePath);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        hasTexture = true;
        delete[] data;
    } else {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        hasTexture = false;
    }
}

void Texture::loadBMP(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open BMP file: " << filePath << std::endl;
        return;
    }

    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    file.read(reinterpret_cast<char *>(&bmpHeader), sizeof(bmpHeader));
    file.read(reinterpret_cast<char *>(&dibHeader), sizeof(dibHeader));

    if (bmpHeader.fileType != 0x4D42 || dibHeader.bitCount != 24) {
        std::cerr << "Unsupported BMP format: " << filePath << std::endl;
        return;
    }

    width = dibHeader.width;
    height = dibHeader.height;
    channels = 3;

    file.seekg(bmpHeader.offsetData, file.beg);

    size_t dataSize = width * height * channels;
    data = new unsigned char[dataSize];

    file.read(reinterpret_cast<char *>(data), dataSize);

    for (int y = 0; y < height / 2; ++y) {
        for (int x = 0; x < width * channels; ++x) {
            std::swap(data[y * width * channels + x], data[(height - 1 - y) * width * channels + x]);
        }
    }

    file.close();
}

Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
