/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "define.hpp"

#pragma pack(push, 1)

struct BMPHeader {
    uint16_t fileType{0x4D42}; // "BM" en ASCII
    uint32_t fileSize{0}; // Taille du fichier en octets
    uint16_t reserved1{0}; // Réservé, doit être 0
    uint16_t reserved2{0}; // Réservé, doit être 0
    uint32_t offsetData{0}; // Offset des données d'image (54 octets pour BMP standard)
};

struct DIBHeader {
    uint32_t size{0}; // Taille du DIB header
    int32_t width{0}; // Largeur de l'image en pixels
    int32_t height{0}; // Hauteur de l'image en pixels
    uint16_t planes{1}; // Nombre de plans de couleur (doit être 1)
    uint16_t bitCount{0}; // Bits par pixel (24 pour RGB)
    uint32_t compression{0}; // Type de compression (0 pour BMP non compressé)
    uint32_t imageSize{0}; // Taille des données de l'image
    int32_t xPixelsPerMeter{0}; // Résolution horizontale (pixels par mètre)
    int32_t yPixelsPerMeter{0}; // Résolution verticale (pixels par mètre)
    uint32_t colorsUsed{0}; // Nombre de couleurs dans la palette (0 pour images RGB)
    uint32_t importantColors{0}; // Nombre de couleurs importantes (0 = toutes)
};

#pragma pack(pop)

class Texture {
public:
    Texture(const std::string &filePath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    unsigned char *data = nullptr;
    int width = 0, height = 0, channels = 0;
    bool hasTexture = false;

private:
    unsigned int textureID;
    void loadBMP(const std::string &filePath);
};

#endif
